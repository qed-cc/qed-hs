/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file qed_hs_gettimeofday.h
 * \brief Header for qed_hs_gettimeofday.c
 **/

#ifndef QED_HS_GETTIMEOFDAY_H
#define QED_HS_GETTIMEOFDAY_H

#include "lib/testsupport/testsupport.h"

struct timeval;

MOCK_DECL(void, qed_hs_gettimeofday, (struct timeval *timeval));

#endif /* !defined(QED_HS_GETTIMEOFDAY_H) */
