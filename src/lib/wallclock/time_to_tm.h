/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file time_to_tm.h
 * \brief Header for time_to_tm.c
 **/

#ifndef QED_HS_WALLCLOCK_TIME_TO_TM_H
#define QED_HS_WALLCLOCK_TIME_TO_TM_H

#include <sys/types.h>

struct tm;
struct tm *qed_hs_localtime_r_msg(const time_t *timep, struct tm *result,
                               char **err_out);
struct tm *qed_hs_gmtime_r_msg(const time_t *timep, struct tm *result,
                            char **err_out);

#endif /* !defined(QED_HS_WALLCLOCK_TIME_TO_TM_H) */
