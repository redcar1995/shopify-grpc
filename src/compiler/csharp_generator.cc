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

#include <cctype>
#include <map>
#include <vector>

#include "src/compiler/config.h"
#include "src/compiler/csharp_generator.h"

using grpc::protobuf::FileDescriptor;
using grpc::protobuf::Descriptor;
using grpc::protobuf::ServiceDescriptor;
using grpc::protobuf::MethodDescriptor;
using grpc::protobuf::io::Printer;
using grpc::protobuf::io::StringOutputStream;
using std::map;
using std::vector;

namespace grpc_csharp_generator {
namespace {

enum MethodType {
  METHODTYPE_NO_STREAMING,
  METHODTYPE_CLIENT_STREAMING,
  METHODTYPE_SERVER_STREAMING,
  METHODTYPE_BIDI_STREAMING
};

MethodType GetMethodType(const MethodDescriptor *method) {
  if (method->client_streaming()) {
    if (method->server_streaming()) {
      return METHODTYPE_BIDI_STREAMING;
    } else {
      return METHODTYPE_CLIENT_STREAMING;
    }
  } else {
    if (method->server_streaming()) {
      return METHODTYPE_SERVER_STREAMING;
    } else {
      return METHODTYPE_NO_STREAMING;
    }
  }
}

std::string GetCSharpNamespace(const FileDescriptor* file) {
  // TODO(jtattermusch): this should be based on csharp_namespace option
  return file->package();
}

std::string GetMessageType(const Descriptor* message) {
  // TODO(jtattermusch): this has to match with C# protobuf generator
  return message->name();
}

std::string GetServiceClassName(const ServiceDescriptor* service) {
  return service->name();
}

std::string GetClientInterfaceName(const ServiceDescriptor* service) {
  return "I" + service->name() + "Client";
}

std::string GetClientClassName(const ServiceDescriptor* service) {
  return service->name() + "Client";
}

std::string GetServerInterfaceName(const ServiceDescriptor* service) {
  return "I" + service->name();
}

std::string GetCSharpMethodType(MethodType method_type) {
  switch (method_type) {
    case METHODTYPE_NO_STREAMING:
      return "MethodType.Unary";
    case METHODTYPE_CLIENT_STREAMING:
      return "MethodType.ClientStreaming";
    case METHODTYPE_SERVER_STREAMING:
      return "MethodType.ServerStreaming";
    case METHODTYPE_BIDI_STREAMING:
      return "MethodType.DuplexStreaming";
  }
  GOOGLE_LOG(FATAL)<< "Can't get here.";
  return "";
}

std::string GetServiceNameFieldName() {
  return "__ServiceName";
}

std::string GetMarshallerFieldName(const Descriptor *message) {
  return "__Marshaller_" + message->name();
}

std::string GetMethodFieldName(const MethodDescriptor *method) {
  return "__Method_" + method->name();
}

std::string GetMethodRequestParamMaybe(const MethodDescriptor *method) {
  if (method->client_streaming()) {
    return "";
  }
  return GetMessageType(method->input_type()) + " request, ";
}

std::string GetMethodReturnTypeClient(const MethodDescriptor *method) {
  switch (GetMethodType(method)) {
    case METHODTYPE_NO_STREAMING:
      return "Task<" + GetMessageType(method->output_type()) + ">";
    case METHODTYPE_CLIENT_STREAMING:
      return "AsyncClientStreamingCall<" + GetMessageType(method->input_type())
          + ", " + GetMessageType(method->output_type()) + ">";
    case METHODTYPE_SERVER_STREAMING:
      return "AsyncServerStreamingCall<" + GetMessageType(method->output_type())
          + ">";
    case METHODTYPE_BIDI_STREAMING:
      return "AsyncDuplexStreamingCall<" + GetMessageType(method->input_type())
          + ", " + GetMessageType(method->output_type()) + ">";
  }
  GOOGLE_LOG(FATAL)<< "Can't get here.";
  return "";
}

std::string GetMethodRequestParamServer(const MethodDescriptor *method) {
  switch (GetMethodType(method)) {
    case METHODTYPE_NO_STREAMING:
    case METHODTYPE_SERVER_STREAMING:
      return GetMessageType(method->input_type()) + " request";
    case METHODTYPE_CLIENT_STREAMING:
    case METHODTYPE_BIDI_STREAMING:
      return "IAsyncStreamReader<" + GetMessageType(method->input_type())
          + "> requestStream";
  }
  GOOGLE_LOG(FATAL)<< "Can't get here.";
  return "";
}

std::string GetMethodReturnTypeServer(const MethodDescriptor *method) {
  switch (GetMethodType(method)) {
    case METHODTYPE_NO_STREAMING:
    case METHODTYPE_CLIENT_STREAMING:
      return "Task<" + GetMessageType(method->output_type()) + ">";
    case METHODTYPE_SERVER_STREAMING:
    case METHODTYPE_BIDI_STREAMING:
      return "Task";
  }
  GOOGLE_LOG(FATAL)<< "Can't get here.";
  return "";
}

std::string GetMethodResponseStreamMaybe(const MethodDescriptor *method) {
  switch (GetMethodType(method)) {
    case METHODTYPE_NO_STREAMING:
    case METHODTYPE_CLIENT_STREAMING:
      return "";
    case METHODTYPE_SERVER_STREAMING:
    case METHODTYPE_BIDI_STREAMING:
      return ", IServerStreamWriter<" + GetMessageType(method->output_type())
          + "> responseStream";
  }
  GOOGLE_LOG(FATAL)<< "Can't get here.";
  return "";
}

// Gets vector of all messages used as input or output types.
std::vector<const Descriptor*> GetUsedMessages(
    const ServiceDescriptor *service) {
  std::set<const Descriptor*> descriptor_set;
  std::vector<const Descriptor*> result;  // vector is to maintain stable ordering
  for (int i = 0; i < service->method_count(); i++) {
    const MethodDescriptor *method = service->method(i);
    if (descriptor_set.find(method->input_type()) == descriptor_set.end()) {
      descriptor_set.insert(method->input_type());
      result.push_back(method->input_type());
    }
    if (descriptor_set.find(method->output_type()) == descriptor_set.end()) {
      descriptor_set.insert(method->output_type());
      result.push_back(method->output_type());
    }
  }
  return result;
}

void GenerateMarshallerFields(Printer* out, const ServiceDescriptor *service) {
  std::vector<const Descriptor*> used_messages = GetUsedMessages(service);
  for (size_t i = 0; i < used_messages.size(); i++) {
    const Descriptor *message = used_messages[i];
    out->Print(
        "static readonly Marshaller<$type$> $fieldname$ = Marshallers.Create((arg) => arg.ToByteArray(), $type$.ParseFrom);\n",
        "fieldname", GetMarshallerFieldName(message), "type",
        GetMessageType(message));
  }
  out->Print("\n");
}

void GenerateStaticMethodField(Printer* out, const MethodDescriptor *method) {
  out->Print(
      "static readonly Method<$request$, $response$> $fieldname$ = new Method<$request$, $response$>(\n",
      "fieldname", GetMethodFieldName(method), "request",
      GetMessageType(method->input_type()), "response",
      GetMessageType(method->output_type()));
  out->Indent();
  out->Indent();
  out->Print("$methodtype$,\n", "methodtype",
             GetCSharpMethodType(GetMethodType(method)));
  out->Print("\"$methodname$\",\n", "methodname", method->name());
  out->Print("$requestmarshaller$,\n", "requestmarshaller",
             GetMarshallerFieldName(method->input_type()));
  out->Print("$responsemarshaller$);\n", "responsemarshaller",
             GetMarshallerFieldName(method->output_type()));
  out->Print("\n");
  out->Outdent();
  out->Outdent();
}

void GenerateClientInterface(Printer* out, const ServiceDescriptor *service) {
  out->Print("// client-side stub interface\n");
  out->Print("public interface $name$\n", "name",
             GetClientInterfaceName(service));
  out->Print("{\n");
  out->Indent();
  for (int i = 0; i < service->method_count(); i++) {
    const MethodDescriptor *method = service->method(i);
    MethodType method_type = GetMethodType(method);

    if (method_type == METHODTYPE_NO_STREAMING) {
      // unary calls have an extra synchronous stub method
      out->Print(
          "$response$ $methodname$($request$ request, CancellationToken token = default(CancellationToken));\n",
          "methodname", method->name(), "request",
          GetMessageType(method->input_type()), "response",
          GetMessageType(method->output_type()));
    }

    std::string method_name = method->name();
    if (method_type == METHODTYPE_NO_STREAMING) {
      method_name += "Async";  // prevent name clash with synchronous method.
    }
    out->Print(
        "$returntype$ $methodname$($request_maybe$CancellationToken token = default(CancellationToken));\n",
        "methodname", method_name, "request_maybe",
        GetMethodRequestParamMaybe(method), "returntype",
        GetMethodReturnTypeClient(method));
  }
  out->Outdent();
  out->Print("}\n");
  out->Print("\n");
}

void GenerateServerInterface(Printer* out, const ServiceDescriptor *service) {
  out->Print("// server-side interface\n");
  out->Print("public interface $name$\n", "name",
             GetServerInterfaceName(service));
  out->Print("{\n");
  out->Indent();
  for (int i = 0; i < service->method_count(); i++) {
    const MethodDescriptor *method = service->method(i);
    out->Print("$returntype$ $methodname$(ServerCallContext context, $request$$response_stream_maybe$);\n",
               "methodname", method->name(), "returntype",
               GetMethodReturnTypeServer(method), "request",
               GetMethodRequestParamServer(method), "response_stream_maybe",
               GetMethodResponseStreamMaybe(method));
  }
  out->Outdent();
  out->Print("}\n");
  out->Print("\n");
}

void GenerateClientStub(Printer* out, const ServiceDescriptor *service) {
  out->Print("// client stub\n");
  out->Print(
      "public class $name$ : AbstractStub<$name$, StubConfiguration>, $interface$\n",
      "name", GetClientClassName(service), "interface",
      GetClientInterfaceName(service));
  out->Print("{\n");
  out->Indent();

  // constructors
  out->Print(
      "public $name$(Channel channel) : this(channel, StubConfiguration.Default)\n",
      "name", GetClientClassName(service));
  out->Print("{\n");
  out->Print("}\n");
  out->Print(
      "public $name$(Channel channel, StubConfiguration config) : base(channel, config)\n",
      "name", GetClientClassName(service));
  out->Print("{\n");
  out->Print("}\n");

  for (int i = 0; i < service->method_count(); i++) {
    const MethodDescriptor *method = service->method(i);
    MethodType method_type = GetMethodType(method);

    if (method_type == METHODTYPE_NO_STREAMING) {
      // unary calls have an extra synchronous stub method
      out->Print(
          "public $response$ $methodname$($request$ request, CancellationToken token = default(CancellationToken))\n",
          "methodname", method->name(), "request",
          GetMessageType(method->input_type()), "response",
          GetMessageType(method->output_type()));
      out->Print("{\n");
      out->Indent();
      out->Print("var call = CreateCall($servicenamefield$, $methodfield$);\n",
                 "servicenamefield", GetServiceNameFieldName(), "methodfield",
                 GetMethodFieldName(method));
      out->Print("return Calls.BlockingUnaryCall(call, request, token);\n");
      out->Outdent();
      out->Print("}\n");
    }

    std::string method_name = method->name();
    if (method_type == METHODTYPE_NO_STREAMING) {
      method_name += "Async";  // prevent name clash with synchronous method.
    }
    out->Print(
        "public $returntype$ $methodname$($request_maybe$CancellationToken token = default(CancellationToken))\n",
        "methodname", method_name, "request_maybe",
        GetMethodRequestParamMaybe(method), "returntype",
        GetMethodReturnTypeClient(method));
    out->Print("{\n");
    out->Indent();
    out->Print("var call = CreateCall($servicenamefield$, $methodfield$);\n",
               "servicenamefield", GetServiceNameFieldName(), "methodfield",
               GetMethodFieldName(method));
    switch (GetMethodType(method)) {
      case METHODTYPE_NO_STREAMING:
        out->Print("return Calls.AsyncUnaryCall(call, request, token);\n");
        break;
      case METHODTYPE_CLIENT_STREAMING:
        out->Print("return Calls.AsyncClientStreamingCall(call, token);\n");
        break;
      case METHODTYPE_SERVER_STREAMING:
        out->Print(
            "return Calls.AsyncServerStreamingCall(call, request, token);\n");
        break;
      case METHODTYPE_BIDI_STREAMING:
        out->Print("return Calls.AsyncDuplexStreamingCall(call, token);\n");
        break;
      default:
        GOOGLE_LOG(FATAL)<< "Can't get here.";
      }
    out->Outdent();
    out->Print("}\n");
  }
  out->Outdent();
  out->Print("}\n");
  out->Print("\n");
}

void GenerateBindServiceMethod(Printer* out, const ServiceDescriptor *service) {
  out->Print(
      "// creates service definition that can be registered with a server\n");
  out->Print(
      "public static ServerServiceDefinition BindService($interface$ serviceImpl)\n",
      "interface", GetServerInterfaceName(service));
  out->Print("{\n");
  out->Indent();

  out->Print(
      "return ServerServiceDefinition.CreateBuilder($servicenamefield$)\n",
      "servicenamefield", GetServiceNameFieldName());
  out->Indent();
  out->Indent();
  for (int i = 0; i < service->method_count(); i++) {
    const MethodDescriptor *method = service->method(i);
    out->Print(".AddMethod($methodfield$, serviceImpl.$methodname$)",
               "methodfield", GetMethodFieldName(method), "methodname",
               method->name());
    if (i == service->method_count() - 1) {
      out->Print(".Build();");
    }
    out->Print("\n");
  }
  out->Outdent();
  out->Outdent();

  out->Outdent();
  out->Print("}\n");
  out->Print("\n");
}

void GenerateNewStubMethods(Printer* out, const ServiceDescriptor *service) {
  out->Print("// creates a new client stub\n");
  out->Print("public static $interface$ NewStub(Channel channel)\n",
             "interface", GetClientInterfaceName(service));
  out->Print("{\n");
  out->Indent();
  out->Print("return new $classname$(channel);\n", "classname",
             GetClientClassName(service));
  out->Outdent();
  out->Print("}\n");
  out->Print("\n");

  out->Print("// creates a new client stub\n");
  out->Print(
      "public static $interface$ NewStub(Channel channel, StubConfiguration config)\n",
      "interface", GetClientInterfaceName(service));
  out->Print("{\n");
  out->Indent();
  out->Print("return new $classname$(channel, config);\n", "classname",
             GetClientClassName(service));
  out->Outdent();
  out->Print("}\n");
}

void GenerateService(Printer* out, const ServiceDescriptor *service) {
  out->Print("public static class $classname$\n", "classname",
             GetServiceClassName(service));
  out->Print("{\n");
  out->Indent();
  out->Print("static readonly string $servicenamefield$ = \"$servicename$\";\n",
             "servicenamefield", GetServiceNameFieldName(), "servicename",
             service->full_name());
  out->Print("\n");

  GenerateMarshallerFields(out, service);
  for (int i = 0; i < service->method_count(); i++) {
    GenerateStaticMethodField(out, service->method(i));
  }
  GenerateClientInterface(out, service);
  GenerateServerInterface(out, service);
  GenerateClientStub(out, service);
  GenerateBindServiceMethod(out, service);
  GenerateNewStubMethods(out, service);

  out->Outdent();
  out->Print("}\n");
}

}  // anonymous namespace

grpc::string GetServices(const FileDescriptor *file) {
  grpc::string output;
  StringOutputStream output_stream(&output);
  Printer out(&output_stream, '$');

  // Don't write out any output if there no services, to avoid empty service
  // files being generated for proto files that don't declare any.
  if (file->service_count() == 0) {
    return output;
  }

  // Write out a file header.
  out.Print("// Generated by the protocol buffer compiler.  DO NOT EDIT!\n");
  out.Print("// source: $filename$\n", "filename", file->name());
  out.Print("#region Designer generated code\n");
  out.Print("\n");
  out.Print("using System;\n");
  out.Print("using System.Threading;\n");
  out.Print("using System.Threading.Tasks;\n");
  out.Print("using Grpc.Core;\n");
  // TODO(jtattermusch): add using for protobuf message classes
  out.Print("\n");

  out.Print("namespace $namespace$ {\n", "namespace", GetCSharpNamespace(file));
  out.Indent();
  for (int i = 0; i < file->service_count(); i++) {
    GenerateService(&out, file->service(i));
  }
  out.Outdent();
  out.Print("}\n");
  out.Print("#endregion");
  return output;
}

}  // namespace grpc_csharp_generator
