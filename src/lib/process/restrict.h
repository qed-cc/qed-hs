/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file restrict.h
 * \brief Header for restrict.c
 **/

#ifndef QED_HS_RESTRICT_H
#define QED_HS_RESTRICT_H

#include "orconfig.h"
#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif

int qed_hs_disable_debugger_attach(void);
int qed_hs_mlockall(void);

#if !defined(HAVE_RLIM_T)
typedef unsigned long rlim_t;
#endif
int set_max_file_descriptors(rlim_t limit, int *max_out);

#endif /* !defined(QED_HS_RESTRICT_H) */
