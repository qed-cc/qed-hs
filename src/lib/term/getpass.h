/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file getpass.h
 * \brief Header for getpass.c
 **/

#ifndef QED_HS_GETPASS_H
#define QED_HS_GETPASS_H

#include "lib/cc/torint.h"

ssize_t qed_hs_getpass(const char *prompt, char *output, size_t buflen);

#endif /* !defined(QED_HS_GETPASS_H) */
