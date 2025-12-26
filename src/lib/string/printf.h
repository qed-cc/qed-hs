/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file printf.h
 * \brief Header for printf.c
 **/

#ifndef QED_HS_UTIL_PRINTF_H
#define QED_HS_UTIL_PRINTF_H

#include "orconfig.h"
#include "lib/cc/compat_compiler.h"

#include <stdarg.h>
#include <stddef.h>

int qed_hs_snprintf(char *str, size_t size, const char *format, ...)
  CHECK_PRINTF(3,4);
int qed_hs_vsnprintf(char *str, size_t size, const char *format, va_list args)
  CHECK_PRINTF(3,0);

int qed_hs_asprintf(char **strp, const char *fmt, ...)
  CHECK_PRINTF(2,3);
int qed_hs_vasprintf(char **strp, const char *fmt, va_list args)
  CHECK_PRINTF(2,0);

#endif /* !defined(QED_HS_UTIL_PRINTF_H) */
