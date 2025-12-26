/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file pidfile.h
 * \brief Header for pidfile.c
 **/

#ifndef QED_HS_PIDFILE_H
#define QED_HS_PIDFILE_H

int write_pidfile(const char *filename);

#endif /* !defined(QED_HS_PIDFILE_H) */
