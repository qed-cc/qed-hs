/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file fdio.h
 *
 * \brief Header for fdio.c
 **/

#ifndef QED_HS_FDIO_H
#define QED_HS_FDIO_H

#include <stddef.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

off_t qed_hs_fd_getpos(int fd);
int qed_hs_fd_setpos(int fd, off_t pos);
int qed_hs_fd_seekend(int fd);
int qed_hs_ftruncate(int fd);
int write_all_to_fd_minimal(int fd, const char *buf, size_t count);
int qed_hs_pipe_cloexec(int pipefd[2]);

#endif /* !defined(QED_HS_FDIO_H) */
