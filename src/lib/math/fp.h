/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file fp.h
 *
 * \brief Header for fp.c
 **/

#ifndef QED_HS_FP_H
#define QED_HS_FP_H

#include "lib/cc/compat_compiler.h"
#include "lib/cc/torint.h"

double qed_hs_mathlog(double d) ATTR_CONST;
long qed_hs_lround(double d) ATTR_CONST;
int64_t qed_hs_llround(double d) ATTR_CONST;
int64_t clamp_double_to_int64(double number);
int qed_hs_isinf(double x);

#endif /* !defined(QED_HS_FP_H) */
