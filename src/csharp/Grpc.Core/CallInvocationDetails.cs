#region Copyright notice and license

// Copyright 2015, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#endregion

using System;
using Grpc.Core.Internal;
using Grpc.Core.Utils;

namespace Grpc.Core
{
    /// <summary>
    /// Details about a client-side call to be invoked.
    /// </summary>
    public struct CallInvocationDetails<TRequest, TResponse>
    {
        readonly Channel channel;
        readonly string method;
        readonly string host;
        readonly Marshaller<TRequest> requestMarshaller;
        readonly Marshaller<TResponse> responseMarshaller;
        CallOptions options;

        public CallInvocationDetails(Channel channel, Method<TRequest, TResponse> method, CallOptions options) :
            this(channel, method, null, options)
        {
        }

        public CallInvocationDetails(Channel channel, Method<TRequest, TResponse> method, string host, CallOptions options) :
            this(channel, method.FullName, host, method.RequestMarshaller, method.ResponseMarshaller, options)
        {
        }

        public CallInvocationDetails(Channel channel, string method, string host, Marshaller<TRequest> requestMarshaller, Marshaller<TResponse> responseMarshaller, CallOptions options)
        {
            this.channel = Preconditions.CheckNotNull(channel, "channel");
            this.method = Preconditions.CheckNotNull(method, "method");
            this.host = host;
            this.requestMarshaller = Preconditions.CheckNotNull(requestMarshaller, "requestMarshaller");
            this.responseMarshaller = Preconditions.CheckNotNull(responseMarshaller, "responseMarshaller");
            this.options = options;
        }

        public Channel Channel
        {
            get
            {
                return this.channel;
            }
        }

        public string Method
        {
            get
            {
                return this.method;
            }
        }

        public string Host
        {
            get
            {
                return this.host;
            }
        }

        public Marshaller<TRequest> RequestMarshaller
        {
            get
            {
                return this.requestMarshaller;
            }
        }

        public Marshaller<TResponse> ResponseMarshaller
        {
            get
            {
                return this.responseMarshaller;
            }
        }
            
        public CallOptions Options
        {
            get
            {
                return options;
            }
        }

        /// <summary>
        /// Returns new instance of <see cref="CallInvocationDetails"/> with
        /// <c>Options</c> set to the value provided. Values of all other fields are preserved.
        /// </summary>
        public CallInvocationDetails<TRequest, TResponse> WithOptions(CallOptions options)
        {
            var newDetails = this;
            newDetails.options = options;
            return newDetails;
        }
    }
}
