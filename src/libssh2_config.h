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

/* Configuration ESP32 */
#define LIBSSH2_ESP32 1

/* Headers */
#define HAVE_UNISTD_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_UN_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_SYS_POLL_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_UIO_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_NETDB_H 1
#define HAVE_FCNTL_H 1
#define HAVE_ERRNO_H 1
#define HAVE_TIME_H 1
#define HAVE_STDIO_H 1
#define HAVE_CTYPE_H 1
#define HAVE_CRYPT_H 1
#define HAVE_SYS_PARAM_H 1

/* Fonctions */
#define HAVE_GETTIMEOFDAY 1
#define HAVE_INET_ADDR 1
#define HAVE_INET_NTOA 1
#define HAVE_INET_NTOA_R 1
#define HAVE_SOCKET 1
#define HAVE_SELECT 1
#define HAVE_POLL 1
#define HAVE_STRTOLL 1
#define HAVE_STRTOI64 1
#define HAVE_STRERROR_R 1
#define HAVE_SNPRINTF 1
#define HAVE_VSNPRINTF 1
#define HAVE_GETADDRINFO 1
#define HAVE_GETNAMEINFO 1

/* Support des sockets non-bloquants */
#define HAVE_O_NONBLOCK 1

/* Configuration OpenSSL */
#define HAVE_LIBSSL 1
#define HAVE_OPENSSL_CRYPTO_H 1
#define HAVE_OPENSSL_SSL_H 1

/* Désactiver les fonctionnalités non utilisées pour économiser de l'espace */
#define LIBSSH2_HAVE_ZLIB 0
#define LIBSSH2_HAVE_LIBGCRYPT 0
#define LIBSSH2_HAVE_LIBMBEDCRYPTO 0

/* Activer le logging de debug si nécessaire */
#ifdef DEBUG
#define LIBSSH2DEBUG 1
#endif

/* Configuration de la mémoire */
#define LIBSSH2_ESP32_MALLOC(s) malloc(s)
#define LIBSSH2_ESP32_FREE(p) free(p)

/* Configuration du réseau */
#define LIBSSH2_ESP32_SOCKET_OPTIONS(s) \
    do { \
        int opt = 1; \
        setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); \
    } while(0)

#endif /* LIBSSH2_CONFIG_H */
