/* Copyright (C) Alexander Lamaison <alexander.lamaison@gmail.com>
 * Copyright (C) Douglas Gilbert
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

#ifndef LIBSSH2_CONFIG_H
#define LIBSSH2_CONFIG_H

/* Enable debug logging if configured */
#ifdef CONFIG_LIBSSH2_DEBUG_ENABLE
#define LIBSSH2DEBUG
#endif

/* Use mbedTLS as crypto engine */
#define LIBSSH2_MBEDTLS

/* Platform specific includes */
#include <inttypes.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/* Function availability */
#define HAVE_GETTIMEOFDAY
#define HAVE_STRTOLL
#define HAVE_SNPRINTF
#define HAVE_POLL
#define HAVE_SELECT

/* Socket support */
#define HAVE_SOCKET
#define HAVE_INET_ADDR

/* Memory functions */
#define HAVE_EXPLICIT_BZERO
#define HAVE_EXPLICIT_MEMSET
#define HAVE_MEMSET_S

/* Required for ESP-IDF */
#define HAVE_CONFIG_H

#endif /* LIBSSH2_CONFIG_H */