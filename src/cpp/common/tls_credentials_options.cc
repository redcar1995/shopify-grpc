/*
 *
 * Copyright 2019 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <grpcpp/security/tls_credentials_options.h>

#include "src/core/lib/security/credentials/tls/grpc_tls_credentials_options.h"
#include "src/cpp/common/tls_credentials_options_util.h"

namespace grpc_impl {
namespace experimental {

/** TLS key materials config API implementation **/
void TlsKeyMaterialsConfig::set_key_materials(
    grpc::string pem_root_certs,
    std::vector<PemKeyCertPair> pem_key_cert_pair_list) {
  pem_key_cert_pair_list_ = std::move(pem_key_cert_pair_list);
  pem_root_certs_ = std::move(pem_root_certs);
}

/** TLS credential reload arg API implementation **/
TlsCredentialReloadArg::TlsCredentialReloadArg(
    grpc_tls_credential_reload_arg arg) {
  c_arg_ = arg;
}

TlsCredentialReloadArg::~TlsCredentialReloadArg() {}

void* TlsCredentialReloadArg::cb_user_data() const {
  return c_arg_.cb_user_data;
}

/** This function creates a new TlsKeyMaterialsConfig instance whose fields are
 * not shared with the corresponding key materials config fields of the
 * TlsCredentialReloadArg instance. **/
std::shared_ptr<TlsKeyMaterialsConfig>
TlsCredentialReloadArg::key_materials_config() const {
  return ConvertToCppKeyMaterialsConfig(c_arg_.key_materials_config);
}

grpc_ssl_certificate_config_reload_status TlsCredentialReloadArg::status()
    const {
  return c_arg_.status;
}

grpc::string TlsCredentialReloadArg::error_details() const {
  grpc::string cpp_error_details(c_arg_.error_details);
  return cpp_error_details;
}

void TlsCredentialReloadArg::set_cb_user_data(void* cb_user_data) {
  c_arg_.cb_user_data = cb_user_data;
}

void TlsCredentialReloadArg::set_key_materials_config(
    const std::shared_ptr<TlsKeyMaterialsConfig>& key_materials_config) {
  c_arg_.key_materials_config =
      ConvertToCKeyMaterialsConfig(key_materials_config);
}

void TlsCredentialReloadArg::set_status(
    grpc_ssl_certificate_config_reload_status status) {
  c_arg_.status = status;
}

void TlsCredentialReloadArg::set_error_details(
    const grpc::string& error_details) {
  c_arg_.error_details = gpr_strdup(error_details.c_str());
}

void TlsCredentialReloadArg::OnCredentialReloadDoneCallback() {
  c_arg_.cb(&c_arg_);
}

/** gRPC TLS credential reload config API implementation **/
TlsCredentialReloadConfig::TlsCredentialReloadConfig(
    const void* config_user_data,
    int (*schedule)(void* config_user_data, TlsCredentialReloadArg* arg),
    void (*cancel)(void* config_user_data, TlsCredentialReloadArg* arg),
    void (*destruct)(void* config_user_data)) {
  config_user_data_ = const_cast<void*>(config_user_data);
  schedule_ = schedule;
  cancel_ = cancel;
  destruct_ = destruct;
  c_config_ = grpc_tls_credential_reload_config_create(
      config_user_data_, &tls_credential_reload_config_c_schedule,
      &tls_credential_reload_config_c_cancel, destruct_);
  c_config_->set_context(static_cast<void*>(this));
}

TlsCredentialReloadConfig::~TlsCredentialReloadConfig() {}

/** gRPC TLS server authorization check arg API implementation **/
TlsServerAuthorizationCheckArg::TlsServerAuthorizationCheckArg(
    grpc_tls_server_authorization_check_arg arg) {
  c_arg_ = arg;
}

TlsServerAuthorizationCheckArg::~TlsServerAuthorizationCheckArg() {}

void* TlsServerAuthorizationCheckArg::cb_user_data() const {
  return c_arg_.cb_user_data;
}

int TlsServerAuthorizationCheckArg::success() const { return c_arg_.success; }

grpc::string TlsServerAuthorizationCheckArg::target_name() const {
  grpc::string cpp_target_name(c_arg_.target_name);
  return cpp_target_name;
}

grpc::string TlsServerAuthorizationCheckArg::peer_cert() const {
  grpc::string cpp_peer_cert(c_arg_.peer_cert);
  return cpp_peer_cert;
}

grpc_status_code TlsServerAuthorizationCheckArg::status() const {
  return c_arg_.status;
}

grpc::string TlsServerAuthorizationCheckArg::error_details() const {
  grpc::string cpp_error_details(c_arg_.error_details);
  return cpp_error_details;
}

void TlsServerAuthorizationCheckArg::set_cb_user_data(void* cb_user_data) {
  c_arg_.cb_user_data = cb_user_data;
}

void TlsServerAuthorizationCheckArg::set_success(int success) {
  c_arg_.success = success;
}

void TlsServerAuthorizationCheckArg::set_target_name(
    const grpc::string& target_name) {
  c_arg_.target_name = gpr_strdup(target_name.c_str());
}

void TlsServerAuthorizationCheckArg::set_peer_cert(
    const grpc::string& peer_cert) {
  c_arg_.peer_cert = gpr_strdup(peer_cert.c_str());
}

void TlsServerAuthorizationCheckArg::set_status(grpc_status_code status) {
  c_arg_.status = status;
}

void TlsServerAuthorizationCheckArg::set_error_details(
    const grpc::string& error_details) {
  c_arg_.error_details = gpr_strdup(error_details.c_str());
}

void TlsServerAuthorizationCheckArg::OnServerAuthorizationCheckDoneCallback() {
  c_arg_.cb(&c_arg_);
}

/** gRPC TLS server authorization check config API implementation **/
TlsServerAuthorizationCheckConfig::TlsServerAuthorizationCheckConfig(
    const void* config_user_data,
    int (*schedule)(void* config_user_data,
                    TlsServerAuthorizationCheckArg* arg),
    void (*cancel)(void* config_user_data, TlsServerAuthorizationCheckArg* arg),
    void (*destruct)(void* config_user_data)) {
  config_user_data_ = const_cast<void*>(config_user_data);
  schedule_ = schedule;
  cancel_ = cancel;
  destruct_ = destruct;
  c_config_ = grpc_tls_server_authorization_check_config_create(
      config_user_data_, &tls_server_authorization_check_config_c_schedule,
      &tls_server_authorization_check_config_c_cancel, destruct_);
  c_config_->set_context(static_cast<void*>(this));
}

TlsServerAuthorizationCheckConfig::~TlsServerAuthorizationCheckConfig() {}

/** gRPC TLS credential options API implementation **/
grpc_tls_credentials_options* TlsCredentialsOptions::c_credentials_options()
    const {
  grpc_tls_credentials_options* c_options =
      grpc_tls_credentials_options_create();
  c_options->set_cert_request_type(cert_request_type_);
  c_options->set_key_materials_config(
      ::grpc_core::RefCountedPtr<grpc_tls_key_materials_config>(
          ConvertToCKeyMaterialsConfig(key_materials_config_)));
  c_options->set_credential_reload_config(
      ::grpc_core::RefCountedPtr<grpc_tls_credential_reload_config>(
          credential_reload_config_->c_config()));
  c_options->set_server_authorization_check_config(
      ::grpc_core::RefCountedPtr<grpc_tls_server_authorization_check_config>(
          server_authorization_check_config_->c_config()));
  return c_options;
}

}  // namespace experimental
}  // namespace grpc_impl
