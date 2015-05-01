/*
 *
 * Copyright 2015, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <map>

#include "src/compiler/cpp_generator.h"
#include "src/compiler/cpp_generator_helpers.h"

#include "src/compiler/config.h"

#include <sstream>

namespace grpc_cpp_generator {
namespace {

template <class T>
grpc::string as_string(T x) {
  std::ostringstream out;
  out << x;
  return out.str();
}

bool NoStreaming(const grpc::protobuf::MethodDescriptor *method) {
  return !method->client_streaming() && !method->server_streaming();
}

bool ClientOnlyStreaming(const grpc::protobuf::MethodDescriptor *method) {
  return method->client_streaming() && !method->server_streaming();
}

bool ServerOnlyStreaming(const grpc::protobuf::MethodDescriptor *method) {
  return !method->client_streaming() && method->server_streaming();
}

bool BidiStreaming(const grpc::protobuf::MethodDescriptor *method) {
  return method->client_streaming() && method->server_streaming();
}

bool HasUnaryCalls(const grpc::protobuf::FileDescriptor *file) {
  for (int i = 0; i < file->service_count(); i++) {
    for (int j = 0; j < file->service(i)->method_count(); j++) {
      if (NoStreaming(file->service(i)->method(j))) {
        return true;
      }
    }
  }
  return false;
}

bool HasClientOnlyStreaming(const grpc::protobuf::FileDescriptor *file) {
  for (int i = 0; i < file->service_count(); i++) {
    for (int j = 0; j < file->service(i)->method_count(); j++) {
      if (ClientOnlyStreaming(file->service(i)->method(j))) {
        return true;
      }
    }
  }
  return false;
}

bool HasServerOnlyStreaming(const grpc::protobuf::FileDescriptor *file) {
  for (int i = 0; i < file->service_count(); i++) {
    for (int j = 0; j < file->service(i)->method_count(); j++) {
      if (ServerOnlyStreaming(file->service(i)->method(j))) {
        return true;
      }
    }
  }
  return false;
}

bool HasBidiStreaming(const grpc::protobuf::FileDescriptor *file) {
  for (int i = 0; i < file->service_count(); i++) {
    for (int j = 0; j < file->service(i)->method_count(); j++) {
      if (BidiStreaming(file->service(i)->method(j))) {
        return true;
      }
    }
  }
  return false;
}

grpc::string FilenameIdentifier(const grpc::string &filename) {
  grpc::string result;
  for (unsigned i = 0; i < filename.size(); i++) {
    char c = filename[i];
    if (isalnum(c)) {
      result.push_back(c);
    } else {
      static char hex[] = "0123456789abcdef";
      result.push_back('_');
      result.push_back(hex[(c >> 4) & 0xf]);
      result.push_back(hex[c & 0xf]);
    }
  }
  return result;
}
}  // namespace

grpc::string GetHeaderPrologue(const grpc::protobuf::FileDescriptor *file,
                               const Parameters &params) {
  grpc::string output;
  grpc::protobuf::io::StringOutputStream output_stream(&output);
  grpc::protobuf::io::Printer printer(&output_stream, '$');
  std::map<grpc::string, grpc::string> vars;

  vars["filename"] = file->name();
  vars["filename_identifier"] = FilenameIdentifier(file->name());
  vars["filename_base"] = grpc_generator::StripProto(file->name());

  printer.Print(vars, "// Generated by the gRPC protobuf plugin.\n");
  printer.Print(vars, "// If you make any local change, they will be lost.\n");
  printer.Print(vars, "// source: $filename$\n");
  printer.Print(vars, "#ifndef GRPC_$filename_identifier$__INCLUDED\n");
  printer.Print(vars, "#define GRPC_$filename_identifier$__INCLUDED\n");
  printer.Print(vars, "\n");
  printer.Print(vars, "#include \"$filename_base$.pb.h\"\n");
  printer.Print(vars, "\n");

  return output;
}

grpc::string GetHeaderIncludes(const grpc::protobuf::FileDescriptor *file,
                               const Parameters &params) {
  grpc::string temp =
      "#include <grpc++/impl/internal_stub.h>\n"
      "#include <grpc++/impl/rpc_method.h>\n"
      "#include <grpc++/impl/service_type.h>\n"
      "#include <grpc++/status.h>\n"
      "\n"
      "namespace grpc {\n"
      "class CompletionQueue;\n"
      "class ChannelInterface;\n"
      "class RpcService;\n"
      "class ServerContext;\n";
  if (HasUnaryCalls(file)) {
    temp.append(
        "template <class OutMessage> class ClientAsyncResponseReader;\n");
    temp.append(
        "template <class OutMessage> class ServerAsyncResponseWriter;\n");
  }
  if (HasClientOnlyStreaming(file)) {
    temp.append("template <class OutMessage> class ClientWriter;\n");
    temp.append("template <class InMessage> class ServerReader;\n");
    temp.append("template <class OutMessage> class ClientAsyncWriter;\n");
    temp.append(
        "template <class OutMessage, class InMessage> class "
        "ServerAsyncReader;\n");
  }
  if (HasServerOnlyStreaming(file)) {
    temp.append("template <class InMessage> class ClientReader;\n");
    temp.append("template <class OutMessage> class ServerWriter;\n");
    temp.append("template <class OutMessage> class ClientAsyncReader;\n");
    temp.append("template <class InMessage> class ServerAsyncWriter;\n");
  }
  if (HasBidiStreaming(file)) {
    temp.append(
        "template <class OutMessage, class InMessage>\n"
        "class ClientReaderWriter;\n");
    temp.append(
        "template <class OutMessage, class InMessage>\n"
        "class ServerReaderWriter;\n");
    temp.append(
        "template <class OutMessage, class InMessage>\n"
        "class ClientAsyncReaderWriter;\n");
    temp.append(
        "template <class OutMessage, class InMessage>\n"
        "class ServerAsyncReaderWriter;\n");
  }
  temp.append("}  // namespace grpc\n");

  temp.append("\n");

  if (!file->package().empty()) {
    std::vector<grpc::string> parts =
        grpc_generator::tokenize(file->package(), ".");

    for (auto part = parts.begin(); part != parts.end(); part++) {
      temp.append("namespace ");
      temp.append(*part);
      temp.append(" {\n");
    }
    temp.append("\n");
  }

  return temp;
}

void PrintHeaderClientMethod(grpc::protobuf::io::Printer *printer,
                             const grpc::protobuf::MethodDescriptor *method,
                             std::map<grpc::string, grpc::string> *vars,
                             bool is_abstract) {
  (*vars)["Method"] = method->name();
  (*vars)["Request"] =
      grpc_cpp_generator::ClassName(method->input_type(), true);
  (*vars)["Response"] =
      grpc_cpp_generator::ClassName(method->output_type(), true);
  (*vars)["IsAbstract"] = is_abstract ? " = 0" : "";
  (*vars)["IsVirtual"] = is_abstract ? "virtual " : "";
  if (NoStreaming(method)) {
    printer->Print(
        *vars,
        "$IsVirtual$::grpc::Status $Method$(::grpc::ClientContext* context, "
        "const $Request$& request, $Response$* response)$IsAbstract$;\n");
    printer->Print(
        *vars,
        "$IsVirtual$"
        "std::unique_ptr< ::grpc::ClientAsyncResponseReader< $Response$>> "
        "Async$Method$(::grpc::ClientContext* context, "
        "const $Request$& request, "
        "::grpc::CompletionQueue* cq, void* tag)$IsAbstract$;\n");
  } else if (ClientOnlyStreaming(method)) {
    printer->Print(
        *vars,
        "$IsVirtual$std::unique_ptr< ::grpc::ClientWriter< $Request$>>"
        " $Method$("
        "::grpc::ClientContext* context, $Response$* response)$IsAbstract$;\n");
    printer->Print(
        *vars,
        "$IsVirtual$std::unique_ptr< ::grpc::ClientAsyncWriter< $Request$>>"
        " Async$Method$(::grpc::ClientContext* context, $Response$* response, "
        "::grpc::CompletionQueue* cq, void* tag)$IsAbstract$;\n");
  } else if (ServerOnlyStreaming(method)) {
    printer->Print(
        *vars,
        "$IsVirtual$std::unique_ptr< ::grpc::ClientReader< $Response$>>"
        " $Method$(::grpc::ClientContext* context, const $Request$& request)"
        "$IsAbstract$;\n");
    printer->Print(
        *vars,
        "$IsVirtual$std::unique_ptr< ::grpc::ClientAsyncReader< $Response$>> "
        "Async$Method$("
        "::grpc::ClientContext* context, const $Request$& request, "
        "::grpc::CompletionQueue* cq, void* tag)$IsAbstract$;\n");
  } else if (BidiStreaming(method)) {
    printer->Print(
        *vars,
        "$IsVirtual$"
        "std::unique_ptr< ::grpc::ClientReaderWriter< $Request$, $Response$>> "
        "$Method$(::grpc::ClientContext* context)$IsAbstract$;\n");
    printer->Print(
        *vars,
        "$IsVirtual$std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< "
        "$Request$, $Response$>> "
        "Async$Method$(::grpc::ClientContext* context, "
        "::grpc::CompletionQueue* cq, void* tag)$IsAbstract$;\n");
  }
}

void PrintHeaderClientMethodData(grpc::protobuf::io::Printer *printer,
                                 const grpc::protobuf::MethodDescriptor *method,
                                 std::map<grpc::string, grpc::string> *vars) {
  (*vars)["Method"] = method->name();
  printer->Print(*vars, "const ::grpc::RpcMethod rpcmethod_$Method$_;\n");
}

void PrintHeaderServerMethodSync(grpc::protobuf::io::Printer *printer,
                                 const grpc::protobuf::MethodDescriptor *method,
                                 std::map<grpc::string, grpc::string> *vars) {
  (*vars)["Method"] = method->name();
  (*vars)["Request"] =
      grpc_cpp_generator::ClassName(method->input_type(), true);
  (*vars)["Response"] =
      grpc_cpp_generator::ClassName(method->output_type(), true);
  if (NoStreaming(method)) {
    printer->Print(*vars,
                   "virtual ::grpc::Status $Method$("
                   "::grpc::ServerContext* context, const $Request$* request, "
                   "$Response$* response);\n");
  } else if (ClientOnlyStreaming(method)) {
    printer->Print(*vars,
                   "virtual ::grpc::Status $Method$("
                   "::grpc::ServerContext* context, "
                   "::grpc::ServerReader< $Request$>* reader, "
                   "$Response$* response);\n");
  } else if (ServerOnlyStreaming(method)) {
    printer->Print(*vars,
                   "virtual ::grpc::Status $Method$("
                   "::grpc::ServerContext* context, const $Request$* request, "
                   "::grpc::ServerWriter< $Response$>* writer);\n");
  } else if (BidiStreaming(method)) {
    printer->Print(
        *vars,
        "virtual ::grpc::Status $Method$("
        "::grpc::ServerContext* context, "
        "::grpc::ServerReaderWriter< $Response$, $Request$>* stream);"
        "\n");
  }
}

void PrintHeaderServerMethodAsync(
    grpc::protobuf::io::Printer *printer,
    const grpc::protobuf::MethodDescriptor *method,
    std::map<grpc::string, grpc::string> *vars) {
  (*vars)["Method"] = method->name();
  (*vars)["Request"] =
      grpc_cpp_generator::ClassName(method->input_type(), true);
  (*vars)["Response"] =
      grpc_cpp_generator::ClassName(method->output_type(), true);
  if (NoStreaming(method)) {
    printer->Print(*vars,
                   "void Request$Method$("
                   "::grpc::ServerContext* context, $Request$* request, "
                   "::grpc::ServerAsyncResponseWriter< $Response$>* response, "
                   "::grpc::CompletionQueue* cq, void *tag);\n");
  } else if (ClientOnlyStreaming(method)) {
    printer->Print(*vars,
                   "void Request$Method$("
                   "::grpc::ServerContext* context, "
                   "::grpc::ServerAsyncReader< $Response$, $Request$>* reader, "
                   "::grpc::CompletionQueue* cq, void *tag);\n");
  } else if (ServerOnlyStreaming(method)) {
    printer->Print(*vars,
                   "void Request$Method$("
                   "::grpc::ServerContext* context, $Request$* request, "
                   "::grpc::ServerAsyncWriter< $Response$>* writer, "
                   "::grpc::CompletionQueue* cq, void *tag);\n");
  } else if (BidiStreaming(method)) {
    printer->Print(
        *vars,
        "void Request$Method$("
        "::grpc::ServerContext* context, "
        "::grpc::ServerAsyncReaderWriter< $Response$, $Request$>* stream, "
        "::grpc::CompletionQueue* cq, void *tag);\n");
  }
}

void PrintHeaderService(grpc::protobuf::io::Printer *printer,
                        const grpc::protobuf::ServiceDescriptor *service,
                        std::map<grpc::string, grpc::string> *vars) {
  (*vars)["Service"] = service->name();

  printer->Print(*vars,
                 "class $Service$ GRPC_FINAL {\n"
                 " public:\n");
  printer->Indent();

  // Client side
  printer->Print(
      "class StubInterface {\n"
      " public:\n");
  printer->Indent();
  printer->Print("StubInterface() {}\n");
  printer->Print("virtual ~StubInterface() {}\n");
  for (int i = 0; i < service->method_count(); ++i) {
    PrintHeaderClientMethod(printer, service->method(i), vars, true);
  }
  printer->Outdent();
  printer->Print("};\n");
  printer->Print(
      "class Stub GRPC_FINAL : public StubInterface,"
      " public ::grpc::InternalStub {\n public:\n");
  printer->Indent();
  printer->Print(
      "Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);\n");
  for (int i = 0; i < service->method_count(); ++i) {
    PrintHeaderClientMethod(printer, service->method(i), vars, false);
  }
  printer->Outdent();
  printer->Print(" private:\n");
  printer->Indent();
  for (int i = 0; i < service->method_count(); ++i) {
    PrintHeaderClientMethodData(printer, service->method(i), vars);
  }
  printer->Outdent();
  printer->Print("};\n");
  printer->Print(
      "static std::unique_ptr<Stub> NewStub(const std::shared_ptr< "
      "::grpc::ChannelInterface>& "
      "channel);\n");

  printer->Print("\n");

  // Server side - Synchronous
  printer->Print(
      "class Service : public ::grpc::SynchronousService {\n"
      " public:\n");
  printer->Indent();
  printer->Print("Service() : service_(nullptr) {}\n");
  printer->Print("virtual ~Service();\n");
  for (int i = 0; i < service->method_count(); ++i) {
    PrintHeaderServerMethodSync(printer, service->method(i), vars);
  }
  printer->Print("::grpc::RpcService* service() GRPC_OVERRIDE GRPC_FINAL;\n");
  printer->Outdent();
  printer->Print(
      " private:\n"
      "  ::grpc::RpcService* service_;\n");
  printer->Print("};\n");

  // Server side - Asynchronous
  printer->Print(
      "class AsyncService GRPC_FINAL : public ::grpc::AsynchronousService {\n"
      " public:\n");
  printer->Indent();
  (*vars)["MethodCount"] = as_string(service->method_count());
  printer->Print("explicit AsyncService(::grpc::CompletionQueue* cq);\n");
  printer->Print("~AsyncService() {};\n");
  for (int i = 0; i < service->method_count(); ++i) {
    PrintHeaderServerMethodAsync(printer, service->method(i), vars);
  }
  printer->Outdent();
  printer->Print("};\n");

  printer->Outdent();
  printer->Print("};\n");
}

grpc::string GetHeaderServices(const grpc::protobuf::FileDescriptor *file,
                               const Parameters &params) {
  grpc::string output;
  grpc::protobuf::io::StringOutputStream output_stream(&output);
  grpc::protobuf::io::Printer printer(&output_stream, '$');
  std::map<grpc::string, grpc::string> vars;

  if (!params.services_namespace.empty()) {
    vars["services_namespace"] = params.services_namespace;
    printer.Print(vars, "\nnamespace $services_namespace$ {\n\n");
  }

  for (int i = 0; i < file->service_count(); ++i) {
    PrintHeaderService(&printer, file->service(i), &vars);
    printer.Print("\n");
  }

  if (!params.services_namespace.empty()) {
    printer.Print(vars, "}  // namespace $services_namespace$\n\n");
  }

  return output;
}

grpc::string GetHeaderEpilogue(const grpc::protobuf::FileDescriptor *file,
                               const Parameters &params) {
  grpc::string output;
  grpc::protobuf::io::StringOutputStream output_stream(&output);
  grpc::protobuf::io::Printer printer(&output_stream, '$');
  std::map<grpc::string, grpc::string> vars;

  vars["filename"] = file->name();
  vars["filename_identifier"] = FilenameIdentifier(file->name());

  if (!file->package().empty()) {
    std::vector<grpc::string> parts =
        grpc_generator::tokenize(file->package(), ".");

    for (auto part = parts.rbegin(); part != parts.rend(); part++) {
      vars["part"] = *part;
      printer.Print(vars, "}  // namespace $part$\n");
    }
    printer.Print(vars, "\n");
  }

  printer.Print(vars, "\n");
  printer.Print(vars, "#endif  // GRPC_$filename_identifier$__INCLUDED\n");

  return output;
}

grpc::string GetSourcePrologue(const grpc::protobuf::FileDescriptor *file,
                               const Parameters &params) {
  grpc::string output;
  grpc::protobuf::io::StringOutputStream output_stream(&output);
  grpc::protobuf::io::Printer printer(&output_stream, '$');
  std::map<grpc::string, grpc::string> vars;

  vars["filename"] = file->name();
  vars["filename_base"] = grpc_generator::StripProto(file->name());

  printer.Print(vars, "// Generated by the gRPC protobuf plugin.\n");
  printer.Print(vars, "// If you make any local change, they will be lost.\n");
  printer.Print(vars, "// source: $filename$\n\n");
  printer.Print(vars, "#include \"$filename_base$.pb.h\"\n");
  printer.Print(vars, "#include \"$filename_base$.grpc.pb.h\"\n");
  printer.Print(vars, "\n");

  return output;
}

grpc::string GetSourceIncludes(const grpc::protobuf::FileDescriptor *file,
                               const Parameters &param) {
  grpc::string output;
  grpc::protobuf::io::StringOutputStream output_stream(&output);
  grpc::protobuf::io::Printer printer(&output_stream, '$');
  std::map<grpc::string, grpc::string> vars;

  printer.Print(vars, "#include <grpc++/async_unary_call.h>\n");
  printer.Print(vars, "#include <grpc++/channel_interface.h>\n");
  printer.Print(vars, "#include <grpc++/impl/client_unary_call.h>\n");
  printer.Print(vars, "#include <grpc++/impl/rpc_service_method.h>\n");
  printer.Print(vars, "#include <grpc++/impl/service_type.h>\n");
  printer.Print(vars, "#include <grpc++/stream.h>\n");

  if (!file->package().empty()) {
    std::vector<grpc::string> parts =
        grpc_generator::tokenize(file->package(), ".");

    for (auto part = parts.begin(); part != parts.end(); part++) {
      vars["part"] = *part;
      printer.Print(vars, "namespace $part$ {\n");
    }
  }

  printer.Print(vars, "\n");

  return output;
}

void PrintSourceClientMethod(grpc::protobuf::io::Printer *printer,
                             const grpc::protobuf::MethodDescriptor *method,
                             std::map<grpc::string, grpc::string> *vars) {
  (*vars)["Method"] = method->name();
  (*vars)["Request"] =
      grpc_cpp_generator::ClassName(method->input_type(), true);
  (*vars)["Response"] =
      grpc_cpp_generator::ClassName(method->output_type(), true);
  if (NoStreaming(method)) {
    printer->Print(*vars,
                   "::grpc::Status $ns$$Service$::Stub::$Method$("
                   "::grpc::ClientContext* context, "
                   "const $Request$& request, $Response$* response) {\n");
    printer->Print(*vars,
                   "  return ::grpc::BlockingUnaryCall(channel(), "
                   "rpcmethod_$Method$_, "
                   "context, request, response);\n"
                   "}\n\n");
    printer->Print(
        *vars,
        "std::unique_ptr< ::grpc::ClientAsyncResponseReader< $Response$>> "
        "$ns$$Service$::Stub::Async$Method$(::grpc::ClientContext* context, "
        "const $Request$& request, "
        "::grpc::CompletionQueue* cq, void* tag) {\n");
    printer->Print(*vars,
                   "  return std::unique_ptr< "
                   "::grpc::ClientAsyncResponseReader< $Response$>>(new "
                   "::grpc::ClientAsyncResponseReader< $Response$>("
                   "channel(), cq, "
                   "rpcmethod_$Method$_, "
                   "context, request, tag));\n"
                   "}\n\n");
  } else if (ClientOnlyStreaming(method)) {
    printer->Print(*vars,
                   "std::unique_ptr< ::grpc::ClientWriter< $Request$>> "
                   "$ns$$Service$::Stub::$Method$("
                   "::grpc::ClientContext* context, $Response$* response) {\n");
    printer->Print(*vars,
                   "  return std::unique_ptr< ::grpc::ClientWriter< "
                   "$Request$>>(new ::grpc::ClientWriter< $Request$>("
                   "channel(),"
                   "rpcmethod_$Method$_, "
                   "context, response));\n"
                   "}\n\n");
    printer->Print(*vars,
                   "std::unique_ptr< ::grpc::ClientAsyncWriter< $Request$>> "
                   "$ns$$Service$::Stub::Async$Method$("
                   "::grpc::ClientContext* context, $Response$* response, "
                   "::grpc::CompletionQueue* cq, void* tag) {\n");
    printer->Print(*vars,
                   "  return std::unique_ptr< ::grpc::ClientAsyncWriter< "
                   "$Request$>>(new ::grpc::ClientAsyncWriter< $Request$>("
                   "channel(), cq, "
                   "rpcmethod_$Method$_, "
                   "context, response, tag));\n"
                   "}\n\n");
  } else if (ServerOnlyStreaming(method)) {
    printer->Print(
        *vars,
        "std::unique_ptr< ::grpc::ClientReader< $Response$>> "
        "$ns$$Service$::Stub::$Method$("
        "::grpc::ClientContext* context, const $Request$& request) {\n");
    printer->Print(*vars,
                   "  return std::unique_ptr< ::grpc::ClientReader< "
                   "$Response$>>(new ::grpc::ClientReader< $Response$>("
                   "channel(),"
                   "rpcmethod_$Method$_, "
                   "context, request));\n"
                   "}\n\n");
    printer->Print(*vars,
                   "std::unique_ptr< ::grpc::ClientAsyncReader< $Response$>> "
                   "$ns$$Service$::Stub::Async$Method$("
                   "::grpc::ClientContext* context, const $Request$& request, "
                   "::grpc::CompletionQueue* cq, void* tag) {\n");
    printer->Print(*vars,
                   "  return std::unique_ptr< ::grpc::ClientAsyncReader< "
                   "$Response$>>(new ::grpc::ClientAsyncReader< $Response$>("
                   "channel(), cq, "
                   "rpcmethod_$Method$_, "
                   "context, request, tag));\n"
                   "}\n\n");
  } else if (BidiStreaming(method)) {
    printer->Print(
        *vars,
        "std::unique_ptr< ::grpc::ClientReaderWriter< $Request$, $Response$>> "
        "$ns$$Service$::Stub::$Method$(::grpc::ClientContext* context) {\n");
    printer->Print(*vars,
                   "  return std::unique_ptr< ::grpc::ClientReaderWriter< "
                   "$Request$, $Response$>>(new ::grpc::ClientReaderWriter< "
                   "$Request$, $Response$>("
                   "channel(),"
                   "rpcmethod_$Method$_, "
                   "context));\n"
                   "}\n\n");
    printer->Print(
        *vars,
        "std::unique_ptr< ::grpc::ClientAsyncReaderWriter< "
        "$Request$, $Response$>> "
        "$ns$$Service$::Stub::Async$Method$(::grpc::ClientContext* context, "
        "::grpc::CompletionQueue* cq, void* tag) {\n");
    printer->Print(*vars,
                   "  return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< "
                   "$Request$, $Response$>>(new "
                   "::grpc::ClientAsyncReaderWriter< $Request$, $Response$>("
                   "channel(), cq, "
                   "rpcmethod_$Method$_, "
                   "context, tag));\n"
                   "}\n\n");
  }
}

void PrintSourceServerMethod(grpc::protobuf::io::Printer *printer,
                             const grpc::protobuf::MethodDescriptor *method,
                             std::map<grpc::string, grpc::string> *vars) {
  (*vars)["Method"] = method->name();
  (*vars)["Request"] =
      grpc_cpp_generator::ClassName(method->input_type(), true);
  (*vars)["Response"] =
      grpc_cpp_generator::ClassName(method->output_type(), true);
  if (NoStreaming(method)) {
    printer->Print(*vars,
                   "::grpc::Status $ns$$Service$::Service::$Method$("
                   "::grpc::ServerContext* context, "
                   "const $Request$* request, $Response$* response) {\n");
    printer->Print(
        "  return ::grpc::Status("
        "::grpc::StatusCode::UNIMPLEMENTED);\n");
    printer->Print("}\n\n");
  } else if (ClientOnlyStreaming(method)) {
    printer->Print(*vars,
                   "::grpc::Status $ns$$Service$::Service::$Method$("
                   "::grpc::ServerContext* context, "
                   "::grpc::ServerReader< $Request$>* reader, "
                   "$Response$* response) {\n");
    printer->Print(
        "  return ::grpc::Status("
        "::grpc::StatusCode::UNIMPLEMENTED);\n");
    printer->Print("}\n\n");
  } else if (ServerOnlyStreaming(method)) {
    printer->Print(*vars,
                   "::grpc::Status $ns$$Service$::Service::$Method$("
                   "::grpc::ServerContext* context, "
                   "const $Request$* request, "
                   "::grpc::ServerWriter< $Response$>* writer) {\n");
    printer->Print(
        "  return ::grpc::Status("
        "::grpc::StatusCode::UNIMPLEMENTED);\n");
    printer->Print("}\n\n");
  } else if (BidiStreaming(method)) {
    printer->Print(*vars,
                   "::grpc::Status $ns$$Service$::Service::$Method$("
                   "::grpc::ServerContext* context, "
                   "::grpc::ServerReaderWriter< $Response$, $Request$>* "
                   "stream) {\n");
    printer->Print(
        "  return ::grpc::Status("
        "::grpc::StatusCode::UNIMPLEMENTED);\n");
    printer->Print("}\n\n");
  }
}

void PrintSourceServerAsyncMethod(
    grpc::protobuf::io::Printer *printer,
    const grpc::protobuf::MethodDescriptor *method,
    std::map<grpc::string, grpc::string> *vars) {
  (*vars)["Method"] = method->name();
  (*vars)["Request"] =
      grpc_cpp_generator::ClassName(method->input_type(), true);
  (*vars)["Response"] =
      grpc_cpp_generator::ClassName(method->output_type(), true);
  if (NoStreaming(method)) {
    printer->Print(*vars,
                   "void $ns$$Service$::AsyncService::Request$Method$("
                   "::grpc::ServerContext* context, "
                   "$Request$* request, "
                   "::grpc::ServerAsyncResponseWriter< $Response$>* response, "
                   "::grpc::CompletionQueue* cq, void* tag) {\n");
    printer->Print(*vars,
                   "  AsynchronousService::RequestAsyncUnary($Idx$, context, "
                   "request, response, cq, tag);\n");
    printer->Print("}\n\n");
  } else if (ClientOnlyStreaming(method)) {
    printer->Print(*vars,
                   "void $ns$$Service$::AsyncService::Request$Method$("
                   "::grpc::ServerContext* context, "
                   "::grpc::ServerAsyncReader< $Response$, $Request$>* reader, "
                   "::grpc::CompletionQueue* cq, void* tag) {\n");
    printer->Print(*vars,
                   "  AsynchronousService::RequestClientStreaming($Idx$, "
                   "context, reader, cq, tag);\n");
    printer->Print("}\n\n");
  } else if (ServerOnlyStreaming(method)) {
    printer->Print(*vars,
                   "void $ns$$Service$::AsyncService::Request$Method$("
                   "::grpc::ServerContext* context, "
                   "$Request$* request, "
                   "::grpc::ServerAsyncWriter< $Response$>* writer, "
                   "::grpc::CompletionQueue* cq, void* tag) {\n");
    printer->Print(*vars,
                   "  AsynchronousService::RequestServerStreaming($Idx$, "
                   "context, request, writer, cq, tag);\n");
    printer->Print("}\n\n");
  } else if (BidiStreaming(method)) {
    printer->Print(
        *vars,
        "void $ns$$Service$::AsyncService::Request$Method$("
        "::grpc::ServerContext* context, "
        "::grpc::ServerAsyncReaderWriter< $Response$, $Request$>* stream, "
        "::grpc::CompletionQueue* cq, void *tag) {\n");
    printer->Print(*vars,
                   "  AsynchronousService::RequestBidiStreaming($Idx$, "
                   "context, stream, cq, tag);\n");
    printer->Print("}\n\n");
  }
}

void PrintSourceService(grpc::protobuf::io::Printer *printer,
                        const grpc::protobuf::ServiceDescriptor *service,
                        std::map<grpc::string, grpc::string> *vars) {
  (*vars)["Service"] = service->name();

  printer->Print(*vars,
                 "static const char* $prefix$$Service$_method_names[] = {\n");
  for (int i = 0; i < service->method_count(); ++i) {
    (*vars)["Method"] = service->method(i)->name();
    printer->Print(*vars, "  \"/$Package$$Service$/$Method$\",\n");
  }
  printer->Print(*vars, "};\n\n");

  printer->Print(
      *vars,
      "std::unique_ptr< $ns$$Service$::Stub> $ns$$Service$::NewStub("
      "const std::shared_ptr< ::grpc::ChannelInterface>& channel) {\n"
      "  std::unique_ptr< $ns$$Service$::Stub> stub(new "
      "$ns$$Service$::Stub(channel));\n"
      "  return stub;\n"
      "}\n\n");
  printer->Print(*vars,
                 "$ns$$Service$::Stub::Stub(const std::shared_ptr< "
                 "::grpc::ChannelInterface>& channel)\n");
  printer->Indent();
  printer->Print(": ::grpc::InternalStub(channel)");
  for (int i = 0; i < service->method_count(); ++i) {
    const grpc::protobuf::MethodDescriptor *method = service->method(i);
    (*vars)["Method"] = method->name();
    (*vars)["Idx"] = as_string(i);
    if (NoStreaming(method)) {
      (*vars)["StreamingType"] = "NORMAL_RPC";
    } else if (ClientOnlyStreaming(method)) {
      (*vars)["StreamingType"] = "CLIENT_STREAMING";
    } else if (ServerOnlyStreaming(method)) {
      (*vars)["StreamingType"] = "SERVER_STREAMING";
    } else {
      (*vars)["StreamingType"] = "BIDI_STREAMING";
    }
    printer->Print(
        *vars,
        ", rpcmethod_$Method$_("
        "$prefix$$Service$_method_names[$Idx$], "
        "::grpc::RpcMethod::$StreamingType$, "
        "channel->RegisterMethod($prefix$$Service$_method_names[$Idx$])"
        ")\n");
  }
  printer->Print("{}\n\n");
  printer->Outdent();

  for (int i = 0; i < service->method_count(); ++i) {
    (*vars)["Idx"] = as_string(i);
    PrintSourceClientMethod(printer, service->method(i), vars);
  }

  (*vars)["MethodCount"] = as_string(service->method_count());
  printer->Print(*vars,
                 "$ns$$Service$::AsyncService::AsyncService(::grpc::"
                 "CompletionQueue* cq) : "
                 "::grpc::AsynchronousService(cq, "
                 "$prefix$$Service$_method_names, $MethodCount$) "
                 "{}\n\n");

  printer->Print(*vars,
                 "$ns$$Service$::Service::~Service() {\n"
                 "  delete service_;\n"
                 "}\n\n");
  for (int i = 0; i < service->method_count(); ++i) {
    (*vars)["Idx"] = as_string(i);
    PrintSourceServerMethod(printer, service->method(i), vars);
    PrintSourceServerAsyncMethod(printer, service->method(i), vars);
  }
  printer->Print(*vars,
                 "::grpc::RpcService* $ns$$Service$::Service::service() {\n");
  printer->Indent();
  printer->Print(
      "if (service_ != nullptr) {\n"
      "  return service_;\n"
      "}\n");
  printer->Print("service_ = new ::grpc::RpcService();\n");
  for (int i = 0; i < service->method_count(); ++i) {
    const grpc::protobuf::MethodDescriptor *method = service->method(i);
    (*vars)["Idx"] = as_string(i);
    (*vars)["Method"] = method->name();
    (*vars)["Request"] =
        grpc_cpp_generator::ClassName(method->input_type(), true);
    (*vars)["Response"] =
        grpc_cpp_generator::ClassName(method->output_type(), true);
    if (NoStreaming(method)) {
      printer->Print(
          *vars,
          "service_->AddMethod(new ::grpc::RpcServiceMethod(\n"
          "    $prefix$$Service$_method_names[$Idx$],\n"
          "    ::grpc::RpcMethod::NORMAL_RPC,\n"
          "    new ::grpc::RpcMethodHandler< $ns$$Service$::Service, "
          "$Request$, "
          "$Response$>(\n"
          "        std::function< ::grpc::Status($ns$$Service$::Service*, "
          "::grpc::ServerContext*, const $Request$*, $Response$*)>("
          "&$ns$$Service$::Service::$Method$), this),\n"
          "    new $Request$, new $Response$));\n");
    } else if (ClientOnlyStreaming(method)) {
      printer->Print(
          *vars,
          "service_->AddMethod(new ::grpc::RpcServiceMethod(\n"
          "    $prefix$$Service$_method_names[$Idx$],\n"
          "    ::grpc::RpcMethod::CLIENT_STREAMING,\n"
          "    new ::grpc::ClientStreamingHandler< "
          "$ns$$Service$::Service, $Request$, $Response$>(\n"
          "        std::function< ::grpc::Status($ns$$Service$::Service*, "
          "::grpc::ServerContext*, "
          "::grpc::ServerReader< $Request$>*, $Response$*)>("
          "&$ns$$Service$::Service::$Method$), this),\n"
          "    new $Request$, new $Response$));\n");
    } else if (ServerOnlyStreaming(method)) {
      printer->Print(
          *vars,
          "service_->AddMethod(new ::grpc::RpcServiceMethod(\n"
          "    $prefix$$Service$_method_names[$Idx$],\n"
          "    ::grpc::RpcMethod::SERVER_STREAMING,\n"
          "    new ::grpc::ServerStreamingHandler< "
          "$ns$$Service$::Service, $Request$, $Response$>(\n"
          "        std::function< ::grpc::Status($ns$$Service$::Service*, "
          "::grpc::ServerContext*, "
          "const $Request$*, ::grpc::ServerWriter< $Response$>*)>("
          "&$ns$$Service$::Service::$Method$), this),\n"
          "    new $Request$, new $Response$));\n");
    } else if (BidiStreaming(method)) {
      printer->Print(
          *vars,
          "service_->AddMethod(new ::grpc::RpcServiceMethod(\n"
          "    $prefix$$Service$_method_names[$Idx$],\n"
          "    ::grpc::RpcMethod::BIDI_STREAMING,\n"
          "    new ::grpc::BidiStreamingHandler< "
          "$ns$$Service$::Service, $Request$, $Response$>(\n"
          "        std::function< ::grpc::Status($ns$$Service$::Service*, "
          "::grpc::ServerContext*, "
          "::grpc::ServerReaderWriter< $Response$, $Request$>*)>("
          "&$ns$$Service$::Service::$Method$), this),\n"
          "    new $Request$, new $Response$));\n");
    }
  }
  printer->Print("return service_;\n");
  printer->Outdent();
  printer->Print("}\n\n");
}

grpc::string GetSourceServices(const grpc::protobuf::FileDescriptor *file,
                               const Parameters &params) {
  grpc::string output;
  grpc::protobuf::io::StringOutputStream output_stream(&output);
  grpc::protobuf::io::Printer printer(&output_stream, '$');
  std::map<grpc::string, grpc::string> vars;
  // Package string is empty or ends with a dot. It is used to fully qualify
  // method names.
  vars["Package"] = file->package();
  if (!file->package().empty()) {
    vars["Package"].append(".");
  }
  if (!params.services_namespace.empty()) {
    vars["ns"] = params.services_namespace + "::";
    vars["prefix"] = params.services_namespace;
  } else {
    vars["ns"] = "";
    vars["prefix"] = "";
  }

  for (int i = 0; i < file->service_count(); ++i) {
    PrintSourceService(&printer, file->service(i), &vars);
    printer.Print("\n");
  }
  return output;
}

grpc::string GetSourceEpilogue(const grpc::protobuf::FileDescriptor *file,
                               const Parameters &params) {
  grpc::string temp;

  if (!file->package().empty()) {
    std::vector<grpc::string> parts =
        grpc_generator::tokenize(file->package(), ".");

    for (auto part = parts.begin(); part != parts.end(); part++) {
      temp.append("}  // namespace ");
      temp.append(*part);
      temp.append("\n");
    }
    temp.append("\n");
  }

  return temp;
}

}  // namespace grpc_cpp_generator
