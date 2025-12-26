/* Copyright (c) 2001 Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * @file qed_hs_api_internal.h
 * @brief Internal declarations for in-process Tor API.
 **/

#ifndef QED_HS_API_INTERNAL_H
#define QED_HS_API_INTERNAL_H

#include "lib/net/nettypes.h"

/* The contents of this type are private; don't mess with them from outside
 * Tor. */
struct qed_hs_main_configuration_t {
  /** As in main() */
  int argc;
  /** As in main(). This pointer is owned by the caller */
  char **argv;

  /** As argc, but describes the number of elements in argv_owned */
  int argc_owned;
  /** As argv, but is owned by the qed_hs_main_configuration_t object. */
  char **argv_owned;

  /** Socket that Tor will use as an owning control socket. Owned. */
  qed_hs_socket_t owning_controller_socket;
};

#endif /* !defined(QED_HS_API_INTERNAL_H) */
