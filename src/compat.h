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

#ifndef _LIBSSH2_ESP32_COMPAT_H_
#define _LIBSSH2_ESP32_COMPAT_H_

#include <Arduino.h>
#include <WiFi.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
#include <lwip/inet.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Définitions spécifiques à ESP32 */
#define LIBSSH2_ESP32 1

/* Redéfinition des types si nécessaire */
typedef int socklen_t;

/* Fonctions de compatibilité */
#define close(s) lwip_close(s)
#define read(s, buf, len) lwip_read(s, buf, len)
#define write(s, buf, len) lwip_write(s, buf, len)
#define select(n, r, w, e, t) lwip_select(n, r, w, e, t)
#define ioctl(s, cmd, arg) lwip_ioctl(s, cmd, arg)

/* Gestion des erreurs */
#define LIBSSH2_ESP32_ERRNO errno

/* Configuration de la mémoire */
#define LIBSSH2_ESP32_MALLOC(s) malloc(s)
#define LIBSSH2_ESP32_FREE(p) free(p)

/* Configuration du réseau */
#define LIBSSH2_ESP32_SOCKET_OPTIONS(s) \
    do { \
        int opt = 1; \
        setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); \
    } while(0)

#endif /* _LIBSSH2_ESP32_COMPAT_H_ */