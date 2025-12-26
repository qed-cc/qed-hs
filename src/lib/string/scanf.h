/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file scanf.h
 * \brief Header for scanf.c
 **/

#ifndef QED_HS_UTIL_SCANF_H
#define QED_HS_UTIL_SCANF_H

#include "orconfig.h"
#include "lib/cc/compat_compiler.h"

#include <stdarg.h>

int qed_hs_vsscanf(const char *buf, const char *pattern, va_list ap) \
  CHECK_SCANF(2, 0);
int qed_hs_sscanf(const char *buf, const char *pattern, ...)
  CHECK_SCANF(2, 3);

#endif /* !defined(QED_HS_UTIL_SCANF_H) */
