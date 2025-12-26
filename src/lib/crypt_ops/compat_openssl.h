/* Copyright (c) 2001, Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#ifndef QED_HS_COMPAT_OPENSSL_H
#define QED_HS_COMPAT_OPENSSL_H

#include "orconfig.h"

#ifdef ENABLE_OPENSSL

#include <openssl/opensslv.h>
#include "lib/crypt_ops/crypto_openssl_mgt.h"

/**
 * \file compat_openssl.h
 *
 * \brief compatibility definitions for working with different openssl forks
 **/

/* LibreSSL claims to be OpenSSL 2.0 but lacks this OpenSSL 1.1 API. */
#if defined(LIBRESSL_VERSION_NUMBER)
#define RAND_OpenSSL() RAND_SSLeay()
#define OSSL_HANDSHAKE_STATE int
#endif

#define qed_hs_OpenSSL_version_num() OpenSSL_version_num()

#endif /* defined(ENABLE_OPENSSL) */

#endif /* !defined(QED_HS_COMPAT_OPENSSL_H) */
