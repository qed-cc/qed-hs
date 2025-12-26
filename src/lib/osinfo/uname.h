/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file uname.h
 * \brief Header for uname.c
 **/

#ifndef HAVE_QED_HS_UNAME_H
#define HAVE_QED_HS_UNAME_H

#include "lib/testsupport/testsupport.h"

MOCK_DECL(const char *, get_uname,(void));

#endif /* !defined(HAVE_QED_HS_UNAME_H) */
