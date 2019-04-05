//
// Copyright 2015 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <grpc/support/port_platform.h>

#include "src/core/ext/filters/client_channel/service_config.h"

#include <string.h>

#include <grpc/impl/codegen/grpc_types.h>
#include <grpc/support/alloc.h>
#include <grpc/support/log.h>
#include <grpc/support/string_util.h>

#include "src/core/lib/gpr/string.h"
#include "src/core/lib/json/json.h"
#include "src/core/lib/slice/slice_hash_table.h"
#include "src/core/lib/slice/slice_internal.h"
#include "src/core/lib/slice/slice_string_helpers.h"

namespace grpc_core {

ServiceConfig::ServiceConfigParserList* ServiceConfig::registered_parsers_;
RefCountedPtr<ServiceConfig> ServiceConfig::Create(const char* json,
                                                   grpc_error** error) {
  UniquePtr<char> service_config_json(gpr_strdup(json));
  UniquePtr<char> json_string(gpr_strdup(json));
  GPR_DEBUG_ASSERT(error != nullptr);
  grpc_json* json_tree = grpc_json_parse_string(json_string.get());
  if (json_tree == nullptr) {
    *error = GRPC_ERROR_CREATE_FROM_STATIC_STRING(
        "failed to parse JSON for service config");
    return nullptr;
  }
  auto return_value = MakeRefCounted<ServiceConfig>(
      std::move(service_config_json), std::move(json_string), json_tree, error);
  return *error == GRPC_ERROR_NONE ? return_value : nullptr;
}

ServiceConfig::ServiceConfig(UniquePtr<char> service_config_json,
                             UniquePtr<char> json_string, grpc_json* json_tree,
                             grpc_error** error)
    : service_config_json_(std::move(service_config_json)),
      json_string_(std::move(json_string)),
      json_tree_(json_tree) {
  GPR_DEBUG_ASSERT(error != nullptr);
  if (json_tree->type != GRPC_JSON_OBJECT || json_tree->key != nullptr) {
    *error = GRPC_ERROR_CREATE_FROM_STATIC_STRING(
        "Malformed service Config JSON object");
    return;
  }
  *error = ParseGlobalParams(json_tree);
  *error = grpc_error_add_child(*error, ParsePerMethodParams(json_tree));
}

grpc_error* ServiceConfig::ParseGlobalParams(const grpc_json* json_tree) {
  GPR_DEBUG_ASSERT(json_tree_->type == GRPC_JSON_OBJECT);
  GPR_DEBUG_ASSERT(json_tree_->key == nullptr);
  grpc_error* error = GRPC_ERROR_NONE;
  for (size_t i = 0; i < registered_parsers_->size(); i++) {
    grpc_error* parser_error = GRPC_ERROR_NONE;
    auto parsed_obj =
        (*registered_parsers_)[i]->ParseGlobalParams(json_tree, &parser_error);
    error = grpc_error_add_child(error, parser_error);
    parsed_global_service_config_objects_.push_back(std::move(parsed_obj));
  }
  return error;
}

grpc_error* ServiceConfig::ParseJsonMethodConfigToServiceConfigObjectsTable(
    const grpc_json* json,
    SliceHashTable<const ServiceConfigObjectsVector*>::Entry* entries,
    size_t* idx) {
  auto objs_vector = MakeUnique<ServiceConfigObjectsVector>();
  grpc_error* error = GRPC_ERROR_NONE;
  for (size_t i = 0; i < registered_parsers_->size(); i++) {
    grpc_error* parser_error = GRPC_ERROR_NONE;
    auto parsed_obj =
        (*registered_parsers_)[i]->ParsePerMethodParams(json, &error);
    error = grpc_error_add_child(error, parser_error);
    objs_vector->push_back(std::move(parsed_obj));
  }
  const auto* vector_ptr = objs_vector.get();
  service_config_objects_vectors_storage_.push_back(std::move(objs_vector));
  // Construct list of paths.
  InlinedVector<UniquePtr<char>, 10> paths;
  for (grpc_json* child = json->child; child != nullptr; child = child->next) {
    if (child->key == nullptr) continue;
    if (strcmp(child->key, "name") == 0) {
      if (child->type != GRPC_JSON_ARRAY) {
        error = grpc_error_add_child(error,
                                     GRPC_ERROR_CREATE_FROM_STATIC_STRING(
                                         "field:name error:not of type Array"));
        goto wrap_error;
      }
      for (grpc_json* name = child->child; name != nullptr; name = name->next) {
        grpc_error* parse_error = GRPC_ERROR_NONE;
        UniquePtr<char> path = ParseJsonMethodName(name, &parse_error);
        if (path == nullptr) {
          error = grpc_error_add_child(error, parse_error);
        } else {
          GPR_DEBUG_ASSERT(parse_error == GRPC_ERROR_NONE);
        }
        paths.push_back(std::move(path));
      }
    }
  }
  if (paths.size() == 0) {
    error = grpc_error_add_child(
        error, GRPC_ERROR_CREATE_FROM_STATIC_STRING("No names specified"));
  }
  // Add entry for each path.
  for (size_t i = 0; i < paths.size(); ++i) {
    entries[*idx].key = grpc_slice_from_copied_string(paths[i].get());
    entries[*idx].value = vector_ptr;  // Takes a new ref.
    ++*idx;
  }
wrap_error:
  if (error != GRPC_ERROR_NONE) {
    error = grpc_error_add_child(
        GRPC_ERROR_CREATE_FROM_STATIC_STRING("field:methodConfig"), error);
  }
  return error;
}

grpc_error* ServiceConfig::ParsePerMethodParams(const grpc_json* json_tree) {
  GPR_DEBUG_ASSERT(json_tree_->type == GRPC_JSON_OBJECT);
  GPR_DEBUG_ASSERT(json_tree_->key == nullptr);
  SliceHashTable<const ServiceConfigObjectsVector*>::Entry* entries = nullptr;
  size_t num_entries = 0;
  grpc_error* error = GRPC_ERROR_NONE;
  for (grpc_json* field = json_tree->child; field != nullptr;
       field = field->next) {
    if (field->key == nullptr) {
      error =
          grpc_error_add_child(error, GRPC_ERROR_CREATE_FROM_STATIC_STRING(
                                          "error:Illegal key value - NULL"));
      continue;
    }
    if (strcmp(field->key, "methodConfig") == 0) {
      if (entries != nullptr) {
        GPR_ASSERT(false);
      }
      if (field->type != GRPC_JSON_ARRAY) {
        return grpc_error_add_child(
            error, GRPC_ERROR_CREATE_FROM_STATIC_STRING(
                       "field:methodConfig error:not of type Array"));
      }
      for (grpc_json* method = field->child; method != nullptr;
           method = method->next) {
        int count = CountNamesInMethodConfig(method);
        if (count <= 0) {
          error = grpc_error_add_child(
              error, GRPC_ERROR_CREATE_FROM_STATIC_STRING(
                         "field:methodConfig error:No names found"));
        }
        num_entries += static_cast<size_t>(count);
      }
      entries = static_cast<
          SliceHashTable<const ServiceConfigObjectsVector*>::Entry*>(gpr_zalloc(
          num_entries *
          sizeof(SliceHashTable<const ServiceConfigObjectsVector*>::Entry)));
      size_t idx = 0;
      for (grpc_json* method = field->child; method != nullptr;
           method = method->next) {
        error = grpc_error_add_child(
            error, ParseJsonMethodConfigToServiceConfigObjectsTable(
                       method, entries, &idx));
      }
      GPR_DEBUG_ASSERT(num_entries == idx);
      break;
    }
  }
  if (entries != nullptr) {
    parsed_method_service_config_objects_table_ =
        SliceHashTable<const ServiceConfigObjectsVector*>::Create(
            num_entries, entries, nullptr);
    gpr_free(entries);
  }
  return error;
}

ServiceConfig::~ServiceConfig() { grpc_json_destroy(json_tree_); }

const char* ServiceConfig::GetLoadBalancingPolicyName() const {
  if (json_tree_->type != GRPC_JSON_OBJECT || json_tree_->key != nullptr) {
    return nullptr;
  }
  const char* lb_policy_name = nullptr;
  for (grpc_json* field = json_tree_->child; field != nullptr;
       field = field->next) {
    if (field->key == nullptr) return nullptr;
    if (strcmp(field->key, "loadBalancingPolicy") == 0) {
      if (lb_policy_name != nullptr) return nullptr;  // Duplicate.
      if (field->type != GRPC_JSON_STRING) return nullptr;
      lb_policy_name = field->value;
    }
  }
  return lb_policy_name;
}

int ServiceConfig::CountNamesInMethodConfig(grpc_json* json) {
  int num_names = 0;
  for (grpc_json* field = json->child; field != nullptr; field = field->next) {
    if (field->key != nullptr && strcmp(field->key, "name") == 0) {
      if (field->type != GRPC_JSON_ARRAY) return -1;
      for (grpc_json* name = field->child; name != nullptr; name = name->next) {
        if (name->type != GRPC_JSON_OBJECT) return -1;
        ++num_names;
      }
    }
  }
  return num_names;
}

UniquePtr<char> ServiceConfig::ParseJsonMethodName(grpc_json* json,
                                                   grpc_error** error) {
  if (json->type != GRPC_JSON_OBJECT) {
    *error = GRPC_ERROR_CREATE_FROM_STATIC_STRING(
        "Field name should be of type object");
    return nullptr;
  }
  const char* service_name = nullptr;
  const char* method_name = nullptr;
  for (grpc_json* child = json->child; child != nullptr; child = child->next) {
    if (child->key == nullptr) {
      *error = GRPC_ERROR_CREATE_FROM_STATIC_STRING("Child entry with no key");
      return nullptr;
    }
    if (child->type != GRPC_JSON_STRING) {
      *error = GRPC_ERROR_CREATE_FROM_STATIC_STRING(
          "Child entry should of type string");
      return nullptr;
    }
    if (strcmp(child->key, "service") == 0) {
      if (service_name != nullptr) {
        *error = GRPC_ERROR_CREATE_FROM_STATIC_STRING(
            "field:service error:Multiple entries");
        return nullptr;  // Duplicate.
      }
      if (child->value == nullptr) {
        *error = GRPC_ERROR_CREATE_FROM_STATIC_STRING(
            "field:service error:empty value");
        return nullptr;
      }
      service_name = child->value;
    } else if (strcmp(child->key, "method") == 0) {
      if (method_name != nullptr) {
        *error = GRPC_ERROR_CREATE_FROM_STATIC_STRING(
            "field:method error:multiple entries");
        return nullptr;  // Duplicate.
      }
      if (child->value == nullptr) {
        *error = GRPC_ERROR_CREATE_FROM_STATIC_STRING(
            "field:method error:empty value");
        return nullptr;
      }
      method_name = child->value;
    }
  }
  if (service_name == nullptr) {
    *error =
        GRPC_ERROR_CREATE_FROM_STATIC_STRING("field:service error:not found");
    return nullptr;  // Required field.
  }
  char* path;
  gpr_asprintf(&path, "/%s/%s", service_name,
               method_name == nullptr ? "*" : method_name);
  return UniquePtr<char>(path);
}

const ServiceConfig::ServiceConfigObjectsVector* const*
ServiceConfig::GetMethodServiceConfigObjectsVector(const grpc_slice& path) {
  const auto* value = parsed_method_service_config_objects_table_->Get(path);
  // If we didn't find a match for the path, try looking for a wildcard
  // entry (i.e., change "/service/method" to "/service/*").
  if (value == nullptr) {
    char* path_str = grpc_slice_to_c_string(path);
    const char* sep = strrchr(path_str, '/') + 1;
    const size_t len = (size_t)(sep - path_str);
    char* buf = (char*)gpr_malloc(len + 2);  // '*' and NUL
    memcpy(buf, path_str, len);
    buf[len] = '*';
    buf[len + 1] = '\0';
    grpc_slice wildcard_path = grpc_slice_from_copied_string(buf);
    gpr_free(buf);
    value = parsed_method_service_config_objects_table_->Get(wildcard_path);
    grpc_slice_unref_internal(wildcard_path);
    gpr_free(path_str);
    if (value == nullptr) return nullptr;
  }
  return value;
}

}  // namespace grpc_core
