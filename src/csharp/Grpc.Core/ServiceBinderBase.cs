#region Copyright notice and license

// Copyright 2018 The gRPC Authors
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

#endregion

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using Grpc.Core.Interceptors;
using Grpc.Core.Internal;
using Grpc.Core.Utils;

namespace Grpc.Core
{
    /// <summary>
    /// Allows binding server-side method implementations in alternative serving stacks.
    /// Instances of this class are usually populated by the <c>BindService</c> method
    /// that is part of the autogenerated code for a protocol buffers service definition.
    /// <seealso cref="ServerServiceDefinition"/>
    /// </summary>
    public class ServiceBinderBase
    {
        /// <summary>
        /// Adds a definition for a single request - single response method.
        /// </summary>
        /// <typeparam name="TRequest">The request message class.</typeparam>
        /// <typeparam name="TResponse">The response message class.</typeparam>
        /// <param name="method">The method.</param>
        /// <param name="handler">The method handler.</param>
        public virtual void AddMethod<TRequest, TResponse>(
            Method<TRequest, TResponse> method,
            UnaryServerMethod<TRequest, TResponse> handler)
                where TRequest : class
                where TResponse : class
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Adds a definition for a client streaming method.
        /// </summary>
        /// <typeparam name="TRequest">The request message class.</typeparam>
        /// <typeparam name="TResponse">The response message class.</typeparam>
        /// <param name="method">The method.</param>
        /// <param name="handler">The method handler.</param>
        public virtual void AddMethod<TRequest, TResponse>(
            Method<TRequest, TResponse> method,
            ClientStreamingServerMethod<TRequest, TResponse> handler)
                where TRequest : class
                where TResponse : class
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Adds a definition for a server streaming method.
        /// </summary>
        /// <typeparam name="TRequest">The request message class.</typeparam>
        /// <typeparam name="TResponse">The response message class.</typeparam>
        /// <param name="method">The method.</param>
        /// <param name="handler">The method handler.</param>
        public virtual void AddMethod<TRequest, TResponse>(
            Method<TRequest, TResponse> method,
            ServerStreamingServerMethod<TRequest, TResponse> handler)
                where TRequest : class
                where TResponse : class
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Adds a definition for a bidirectional streaming method.
        /// </summary>
        /// <typeparam name="TRequest">The request message class.</typeparam>
        /// <typeparam name="TResponse">The response message class.</typeparam>
        /// <param name="method">The method.</param>
        /// <param name="handler">The method handler.</param>
        public virtual void AddMethod<TRequest, TResponse>(
            Method<TRequest, TResponse> method,
            DuplexStreamingServerMethod<TRequest, TResponse> handler)
                where TRequest : class
                where TResponse : class
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Adds a method without a handler.
        /// </summary>
        /// <typeparam name="TRequest">The request message class.</typeparam>
        /// <typeparam name="TResponse">The response message class.</typeparam>
        /// <param name="method">The method.</param>
        public virtual void AddMethod<TRequest, TResponse>(
            Method<TRequest, TResponse> method)
                where TRequest : class
                where TResponse : class
        {
            throw new NotImplementedException();
        }
    }
}
