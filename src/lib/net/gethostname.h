/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file gethostname.h
 * \brief Header for gethostname.c
 **/

#ifndef QED_HS_GETHOSTNAME_H
#define QED_HS_GETHOSTNAME_H

#include "lib/testsupport/testsupport.h"
#include <stddef.h>

MOCK_DECL(int,qed_hs_gethostname,(char *name, size_t namelen));

#endif /* !defined(QED_HS_GETHOSTNAME_H) */
