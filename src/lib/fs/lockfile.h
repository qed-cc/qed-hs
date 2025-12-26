/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file lockfile.h
 *
 * \brief Header for lockfile.c
 **/

#ifndef QED_HS_LOCKFILE_H
#define QED_HS_LOCKFILE_H

typedef struct qed_hs_lockfile_t qed_hs_lockfile_t;
qed_hs_lockfile_t *qed_hs_lockfile_lock(const char *filename, int blocking,
                                  int *locked_out);
void qed_hs_lockfile_unlock(qed_hs_lockfile_t *lockfile);

#endif /* !defined(QED_HS_LOCKFILE_H) */
