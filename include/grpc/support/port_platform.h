/*
 *
 * Copyright 2014, Google Inc.
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

#ifndef __GRPC_SUPPORT_PORT_PLATFORM_H__
#define __GRPC_SUPPORT_PORT_PLATFORM_H__

/* Override this file with one for your platform if you need to redefine
   things.  */

/* For a common case, assume that the platform has a C99-like stdint.h */

#include <stdint.h>

#if !defined(GPR_NO_AUTODETECT_PLATFORM)
#if defined(_WIN64) || defined(WIN64)
#define GPR_WIN32 1
#define GPR_ARCH_64 1
#define GPR_GETPID_IN_PROCESS_H 1
#elif defined(_WIN32) || defined(WIN32)
#define GPR_ARCH_32 1
#define GPR_WIN32 1
#define GPR_GETPID_IN_PROCESS_H 1
#elif defined(ANDROID) || defined(__ANDROID__)
#define GPR_ANDROID 1
#define GPR_ARCH_32 1
#define GPR_CPU_LINUX 1
#define GPR_GCC_SYNC 1
#define GPR_LIBEVENT 1
#define GPR_POSIX_MULTIPOLL_WITH_POLL 1
#define GPR_POSIX_SOCKET 1
#define GPR_POSIX_SOCKETADDR 1
#define GPR_POSIX_SOCKETUTILS 1
#define GPR_POSIX_STRING 1
#define GPR_POSIX_SYNC 1
#define GPR_POSIX_TIME 1
#define GPR_GETPID_IN_UNISTD_H 1
#elif defined(__linux__)
#define GPR_CPU_LINUX 1
#define GPR_GCC_ATOMIC 1
#define GPR_LIBEVENT 1
#define GPR_LINUX 1
#define GPR_POSIX_MULTIPOLL_WITH_POLL 1
#define GPR_POSIX_SOCKET 1
#define GPR_POSIX_SOCKETADDR 1
#define GPR_POSIX_STRING 1
#define GPR_POSIX_SYNC 1
#define GPR_POSIX_TIME 1
#define GPR_GETPID_IN_UNISTD_H 1
#ifdef _LP64
#define GPR_ARCH_64 1
#else /* _LP64 */
#define GPR_ARCH_32 1
#endif /* _LP64 */
#elif defined(__APPLE__)
#define GPR_CPU_POSIX 1
#define GPR_GCC_ATOMIC 1
#define GPR_LIBEVENT 1
#define GPR_POSIX_LOG 1
#define GPR_POSIX_MULTIPOLL_WITH_POLL 1
#define GPR_POSIX_SOCKET 1
#define GPR_POSIX_SOCKETADDR 1
#define GPR_POSIX_SOCKETUTILS 1
#define GPR_POSIX_STRING 1
#define GPR_POSIX_SYNC 1
#define GPR_POSIX_TIME 1
#define GPR_GETPID_IN_UNISTD_H 1
#ifdef _LP64
#define GPR_ARCH_64 1
#else /* _LP64 */
#define GPR_ARCH_32 1
#endif /* _LP64 */
#else
#error Could not auto-detect platform
#endif
#endif /* GPR_NO_AUTODETECT_PLATFORM */

/* Cache line alignment */
#ifndef GPR_CACHELINE_SIZE
#if defined(__i386__) || defined(__x86_64__)
#define GPR_CACHELINE_SIZE 64
#endif
#ifndef GPR_CACHELINE_SIZE
/* A reasonable default guess. Note that overestimates tend to waste more
   space, while underestimates tend to waste more time. */
#define GPR_CACHELINE_SIZE 64
#endif /* GPR_CACHELINE_SIZE */
#endif /* GPR_CACHELINE_SIZE */

/* scrub GCC_ATOMIC if it's not available on this compiler */
#if defined(GPR_GCC_ATOMIC) && !defined(__ATOMIC_RELAXED)
#undef GPR_GCC_ATOMIC
#define GPR_GCC_SYNC 1
#endif

/* Validate platform combinations */
#if defined(GPR_GCC_ATOMIC) + defined(GPR_GCC_SYNC) + defined(GPR_WIN32) != 1
#error Must define exactly one of GPR_GCC_ATOMIC, GPR_GCC_SYNC, GPR_WIN32
#endif

#if defined(GPR_ARCH_32) + defined(GPR_ARCH_64) != 1
#error Must define exactly one of GPR_ARCH_32, GPR_ARCH_64
#endif

#if defined(GPR_CPU_LINUX) + defined(GPR_CPU_POSIX) + defined(GPR_WIN32) != 1
#error Must define exactly one of GPR_CPU_LINUX, GPR_CPU_POSIX, GPR_WIN32
#endif

typedef int16_t gpr_int16;
typedef int32_t gpr_int32;
typedef int64_t gpr_int64;
typedef uint8_t gpr_uint8;
typedef uint16_t gpr_uint16;
typedef uint32_t gpr_uint32;
typedef uint64_t gpr_uint64;
typedef intmax_t gpr_intmax;
typedef intptr_t gpr_intptr;
typedef uintmax_t gpr_uintmax;
typedef uintptr_t gpr_uintptr;

/* INT64_MAX is unavailable on some platforms. */
#define GPR_INT64_MAX (~(gpr_uint64)0 >> 1)

/* maximum alignment needed for any type on this platform, rounded up to a
   power of two */
#define GPR_MAX_ALIGNMENT 16

#endif /* __GRPC_SUPPORT_PORT_PLATFORM_H__ */
