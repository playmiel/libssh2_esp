/* Copyright (C) 2025 skuodi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided
 * that the following conditions are met:
 *
 *   Redistributions of source code must retain the above
 *   copyright notice, this list of conditions and the
 *   following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following
 *   disclaimer in the documentation and/or other materials
 *   provided with the distribution.
 *
 *   Neither the name of the copyright holder nor the names
 *   of any other contributors may be used to endorse or
 *   promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef LIBSSH2_COMPAT_H
#define LIBSSH2_COMPAT_H

/* Platform specific compatibility definitions */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

/* Socket compatibility */
#ifndef HAVE_SOCKET
#define socket(a,b,c) (-1)
#endif

#ifndef HAVE_INET_ADDR
#define inet_addr(a) (0)
#endif

/* Time compatibility */
#ifndef HAVE_GETTIMEOFDAY
#include <time.h>
#define gettimeofday(tv, tz) do { \
    time_t t = time(NULL); \
    (tv)->tv_sec = t; \
    (tv)->tv_usec = 0; \
} while(0)
#endif

/* Memory compatibility */
#ifndef HAVE_EXPLICIT_BZERO
#define explicit_bzero(p, n) memset(p, 0, n)
#endif

#ifndef HAVE_EXPLICIT_MEMSET
#define explicit_memset(p, v, n) memset(p, v, n)
#endif

#ifndef HAVE_MEMSET_S
#define memset_s(p, s, v, n) memset(p, v, n)
#endif

#endif /* LIBSSH2_COMPAT_H */