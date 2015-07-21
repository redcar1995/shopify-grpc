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
using System.Runtime.InteropServices;
using System.Threading.Tasks;

namespace Grpc.Core.Internal
{
    

    /// <summary>
    /// grpc_metadata_array from <grpc/grpc.h>
    /// </summary>
    internal class MetadataArraySafeHandle : SafeHandleZeroIsInvalid
    {
        [DllImport("grpc_csharp_ext.dll")]
        static extern MetadataArraySafeHandle grpcsharp_metadata_array_create(UIntPtr capacity);

        [DllImport("grpc_csharp_ext.dll", CharSet = CharSet.Ansi)]
        static extern void grpcsharp_metadata_array_add(MetadataArraySafeHandle array, string key, byte[] value, UIntPtr valueLength);

        [DllImport("grpc_csharp_ext.dll")]
        static extern UIntPtr grpcsharp_metadata_array_count(IntPtr metadataArray);

        [DllImport("grpc_csharp_ext.dll")]
        static extern MetadataEntryStruct grpcsharp_metadata_array_get(IntPtr metadataArray, UIntPtr index);

        [DllImport("grpc_csharp_ext.dll")]
        static extern void grpcsharp_metadata_array_destroy_full(IntPtr array);

        private MetadataArraySafeHandle()
        {
        }
            
        public static MetadataArraySafeHandle Create(Metadata metadata)
        {
            // TODO(jtattermusch): we might wanna check that the metadata is readonly 
            var metadataArray = grpcsharp_metadata_array_create(new UIntPtr((ulong)metadata.Count));
            for (int i = 0; i < metadata.Count; i++)
            {
                grpcsharp_metadata_array_add(metadataArray, metadata[i].Key, metadata[i].ValueBytes, new UIntPtr((ulong)metadata[i].ValueBytes.Length));
            }
            return metadataArray;
        }

        /// <summary>
        /// Reads metadata from pointer to grpc_metadata_array
        /// </summary>
        public static Metadata ReadMetadataFromPtrUnsafe(IntPtr metadataArray)
        {
            if (metadataArray == IntPtr.Zero)
            {
                return null;
            }

            ulong count = grpcsharp_metadata_array_count(metadataArray).ToUInt64();

            var metadata = new Metadata();
            for (ulong index = 0; index < count; index ++)
            {
                var rawEntry = grpcsharp_metadata_array_get(metadataArray, new UIntPtr(index));
                string key = Marshal.PtrToStringAnsi(rawEntry.key);
                var bytes = new byte[rawEntry.valueLength.ToUInt64()];
                Marshal.Copy(rawEntry.value, bytes, 0, bytes.Length);
                metadata.Add(new Metadata.Entry(key, bytes));
            }
            return metadata;
        }

        internal IntPtr Handle
        {
            get
            {
                return handle;
            }
        }

        protected override bool ReleaseHandle()
        {
            grpcsharp_metadata_array_destroy_full(handle);
            return true;
        }

        /// <summary>
        /// gprc_metadata from grpc/grpc.h
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        private struct MetadataEntryStruct
        {
            public IntPtr key;  // const char*
            public IntPtr value;  // const char*
            public UIntPtr valueLength;
        }
    }
}
