/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file userdb.h
 *
 * \brief Header for userdb.c
 **/

#ifndef QED_HS_USERDB_H
#define QED_HS_USERDB_H

#include "orconfig.h"

#ifndef _WIN32
#include <sys/types.h>

struct passwd;
const struct passwd *qed_hs_getpwnam(const char *username);
const struct passwd *qed_hs_getpwuid(uid_t uid);
char *get_user_homedir(const char *username);
#endif /* !defined(_WIN32) */

#endif /* !defined(QED_HS_USERDB_H) */
