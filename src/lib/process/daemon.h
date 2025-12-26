/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file daemon.h
 * \brief Header for daemon.c
 **/

#ifndef QED_HS_DAEMON_H
#define QED_HS_DAEMON_H

#include <stdbool.h>

int start_daemon(void);
int finish_daemon(const char *desired_cwd);

bool start_daemon_has_been_called(void);

#endif /* !defined(QED_HS_DAEMON_H) */
