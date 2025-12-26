/* Copyright (c) 2003, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file winlib.h
 *
 * \brief Header for winlib.c
 **/

#ifndef QED_HS_WINLIB_H
#define QED_HS_WINLIB_H

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>

HANDLE load_windows_system_library(const TCHAR *library_name);
#endif /* defined(_WIN32) */

#endif /* !defined(QED_HS_WINLIB_H) */
