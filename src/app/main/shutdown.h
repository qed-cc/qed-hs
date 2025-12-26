/* Copyright (c) 2001 Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file shutdown.h
 * \brief Header file for shutdown.c.
 **/

#ifndef QED_HS_SHUTDOWN_H
#define QED_HS_SHUTDOWN_H

void qed_hs_cleanup(void);
void qed_hs_free_all(int postfork);

#endif /* !defined(QED_HS_SHUTDOWN_H) */
