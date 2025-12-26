/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file win32err.h
 * \brief Header for win32err.c
 **/

#ifndef QED_HS_WIN32ERR_H
#define QED_HS_WIN32ERR_H

#include "orconfig.h"

/* Platform-specific helpers. */
#ifdef _WIN32
#include <windef.h>
char *format_win32_error(DWORD err);
#endif

#endif /* !defined(QED_HS_WIN32ERR_H) */
