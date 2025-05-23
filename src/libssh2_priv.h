#ifndef LIBSSH2_PRIV_H
#define LIBSSH2_PRIV_H

/* Copyright (C) Sara Golemon <sarag@libssh2.org>
 * Copyright (C) Daniel Stenberg
 * Copyright (C) Simon Josefsson
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

#define LIBSSH2_LIBRARY

/* platform/compiler-specific setup */
#include "libssh2_setup.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>

/* Platform specific includes */
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include "libssh2.h"
#include "libssh2_publickey.h"
#include "libssh2_sftp.h"


/* Basic definitions */
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#ifndef UINT32_MAX
#define UINT32_MAX 0xffffffffU
#endif

/* Memory management macros */
#define LIBSSH2_ALLOC(session, count) \
    session->alloc((count), &(session)->abstract)
#define LIBSSH2_CALLOC(session, count) _libssh2_calloc(session, count)
#define LIBSSH2_REALLOC(session, ptr, count) \
    ((ptr) ? session->realloc((ptr), (count), &(session)->abstract) : \
             session->alloc((count), &(session)->abstract))
#define LIBSSH2_FREE(session, ptr) \
    session->free((ptr), &(session)->abstract)

/* Session callbacks */
#define LIBSSH2_IGNORE(session, data, datalen) \
    session->ssh_msg_ignore((session), (data), (int)(datalen), \
                            &(session)->abstract)
#define LIBSSH2_DEBUG(session, always_display, message, message_len, \
                      language, language_len) \
    session->ssh_msg_debug((session), (always_display), \
                           (message), (int)(message_len), \
                           (language), (int)(language_len), \
                           &(session)->abstract)
#define LIBSSH2_DISCONNECT(session, reason, message, message_len, \
                           language, language_len) \
    session->ssh_msg_disconnect((session), (reason), \
                               (message), (int)(message_len), \
                               (language), (int)(language_len), \
                               &(session)->abstract)

/* Constants */
#define MAX_BLOCKSIZE 32    /* MUST fit biggest crypto block size we use/get */
#define MAX_MACSIZE 64      /* MUST fit biggest MAC length we support */
#define MAX_SSH_PACKET_LEN 35000
#define MAX_SHA_DIGEST_LEN SHA512_DIGEST_LENGTH

/* Forward declarations */
typedef struct _LIBSSH2_KEX_METHOD LIBSSH2_KEX_METHOD;
typedef struct _LIBSSH2_HOSTKEY_METHOD LIBSSH2_HOSTKEY_METHOD;
typedef struct _LIBSSH2_CRYPT_METHOD LIBSSH2_CRYPT_METHOD;
typedef struct _LIBSSH2_COMP_METHOD LIBSSH2_COMP_METHOD;
typedef struct _LIBSSH2_PACKET LIBSSH2_PACKET;

/* Non-blocking states */
typedef enum
{
    libssh2_NB_state_idle = 0,
    libssh2_NB_state_allocated,
    libssh2_NB_state_created,
    libssh2_NB_state_sent,
    libssh2_NB_state_sent1,
    libssh2_NB_state_sent2,
    libssh2_NB_state_sent3,
    libssh2_NB_state_sent4,
    libssh2_NB_state_sent5,
    libssh2_NB_state_sent6,
    libssh2_NB_state_sent7,
    libssh2_NB_state_jump1,
    libssh2_NB_state_jump2,
    libssh2_NB_state_jump3,
    libssh2_NB_state_jump4,
    libssh2_NB_state_jump5,
    libssh2_NB_state_error_closing,
    libssh2_NB_state_end,
    libssh2_NB_state_jumpauthagent
} libssh2_nonblocking_states;

/* Session structure */
struct _LIBSSH2_SESSION
{
    /* Memory management callbacks */
    void *abstract;
    LIBSSH2_ALLOC_FUNC((*alloc));
    LIBSSH2_REALLOC_FUNC((*realloc));
    LIBSSH2_FREE_FUNC((*free));

    /* Other callbacks */
    LIBSSH2_IGNORE_FUNC((*ssh_msg_ignore));
    LIBSSH2_DEBUG_FUNC((*ssh_msg_debug));
    LIBSSH2_DISCONNECT_FUNC((*ssh_msg_disconnect));
    LIBSSH2_MACERROR_FUNC((*macerror));
    LIBSSH2_X11_OPEN_FUNC((*x11));
    LIBSSH2_AUTHAGENT_FUNC((*authagent));
    LIBSSH2_ADD_IDENTITIES_FUNC((*addLocalIdentities));
    LIBSSH2_AUTHAGENT_SIGN_FUNC((*agentSignCallback));
    LIBSSH2_SEND_FUNC((*send));
    LIBSSH2_RECV_FUNC((*recv));

    /* Method preferences */
    char *kex_prefs;
    char *hostkey_prefs;

    int state;

    /* Agreed Key Exchange Method */
    const LIBSSH2_KEX_METHOD *kex;
    unsigned int burn_optimistic_kexinit;

    unsigned char *session_id;
    uint32_t session_id_len;

    /* Server's public key */
    const LIBSSH2_HOSTKEY_METHOD *hostkey;
    void *server_hostkey_abstract;
    unsigned char *server_hostkey;
    uint32_t server_hostkey_len;

    /* Error tracking */
    const char *err_msg;
    int err_code;
    int err_flags;

    /* Actual I/O socket */
    libssh2_socket_t socket_fd;
    int socket_state;
    int socket_block_directions;
    int socket_prev_blockstate;

#ifdef LIBSSH2DEBUG
    int showmask;
    libssh2_trace_handler_func tracehandler;
    void *tracehandler_context;
#endif

    /* Keepalive variables */
    int keepalive_interval;
    int keepalive_want_reply;
    time_t keepalive_last_sent;

    /* Configurable timeout for packets */
    long packet_read_timeout;
};

/* Function declarations */
void _libssh2_init_if_needed(void);
int _libssh2_bcrypt_pbkdf(const char *pass,
                          size_t passlen,
                          const uint8_t *salt,
                          size_t saltlen,
                          uint8_t *key,
                          size_t keylen,
                          unsigned int rounds);

#endif /* LIBSSH2_PRIV_H */ 