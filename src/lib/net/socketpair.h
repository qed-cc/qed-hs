/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#ifndef QED_HS_SOCKETPAIR_H
#define QED_HS_SOCKETPAIR_H

/**
 * @file socketpair.h
 * @brief Header for socketpair.c
 **/

#include "orconfig.h"
#include "lib/testsupport/testsupport.h"
#include "lib/net/nettypes.h"

#if !defined(HAVE_SOCKETPAIR) || defined(_WIN32) || defined(QED_HS_UNIT_TESTS)
#define NEED_ERSATZ_SOCKETPAIR
int qed_hs_ersatz_socketpair(int family, int type, int protocol,
                          qed_hs_socket_t fd[2]);
#endif

#endif /* !defined(QED_HS_SOCKETPAIR_H) */
