/* Copyright (c) 2003-2004, Roger Dingledine
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file socket.h
 * \brief Header for socket.c
 **/

#ifndef QED_HS_SOCKET_H
#define QED_HS_SOCKET_H

#include "orconfig.h"
#include "lib/cc/torint.h"
#include "lib/net/nettypes.h"
#include "lib/testsupport/testsupport.h"

#include <errno.h>

struct sockaddr;

int qed_hs_close_socket_simple(qed_hs_socket_t s);
MOCK_DECL(int, qed_hs_close_socket, (qed_hs_socket_t s));
void qed_hs_take_socket_ownership(qed_hs_socket_t s);
void qed_hs_release_socket_ownership(qed_hs_socket_t s);
qed_hs_socket_t qed_hs_open_socket_with_extensions(
                                           int domain, int type, int protocol,
                                           int cloexec, int nonblock);
MOCK_DECL(qed_hs_socket_t,qed_hs_open_socket,(int domain, int type, int protocol));
qed_hs_socket_t qed_hs_open_socket_nonblocking(int domain, int type, int protocol);
qed_hs_socket_t qed_hs_accept_socket(qed_hs_socket_t sockfd, struct sockaddr *addr,
                                  socklen_t *len);
qed_hs_socket_t qed_hs_accept_socket_nonblocking(qed_hs_socket_t sockfd,
                                           struct sockaddr *addr,
                                           socklen_t *len);
qed_hs_socket_t qed_hs_accept_socket_with_extensions(qed_hs_socket_t sockfd,
                                               struct sockaddr *addr,
                                               socklen_t *len,
                                               int cloexec, int nonblock);
MOCK_DECL(qed_hs_socket_t, qed_hs_connect_socket,(qed_hs_socket_t socket,
                                            const struct sockaddr *address,
                                            socklen_t address_len));
int get_n_open_sockets(void);

MOCK_DECL(int,qed_hs_getsockname,(qed_hs_socket_t socket, struct sockaddr *address,
                 socklen_t *address_len));
struct qed_hs_addr_t;
int qed_hs_addr_from_getsockname(struct qed_hs_addr_t *addr_out, qed_hs_socket_t sock);

#define qed_hs_socket_send(s, buf, len, flags) send(s, buf, len, flags)
#define qed_hs_socket_recv(s, buf, len, flags) recv(s, buf, len, flags)

int set_socket_nonblocking(qed_hs_socket_t socket);
int qed_hs_socketpair(int family, int type, int protocol, qed_hs_socket_t fd[2]);
int network_init(void);
void check_network_configuration(bool server_mode);

int get_max_sockets(void);
void set_max_sockets(int);

ssize_t write_all_to_socket(qed_hs_socket_t fd, const char *buf, size_t count);
ssize_t read_all_from_socket(qed_hs_socket_t fd, char *buf, size_t count);

/* For stupid historical reasons, windows sockets have an independent
 * set of errnos, and an independent way to get them.  Also, you can't
 * always believe WSAEWOULDBLOCK.  Use the macros below to compare
 * errnos against expected values, and use qed_hs_socket_errno to find
 * the actual errno after a socket operation fails.
 */
#if defined(_WIN32)
/** Expands to WSA<b>e</b> on Windows, and to <b>e</b> elsewhere. */
#define SOCK_ERRNO(e) WSA##e
/** Return true if e is EAGAIN or the local equivalent. */
#define ERRNO_IS_EAGAIN(e)           ((e) == EAGAIN || (e) == WSAEWOULDBLOCK)
/** Return true if e is EINPROGRESS or the local equivalent. */
#define ERRNO_IS_EINPROGRESS(e)      ((e) == WSAEINPROGRESS)
/** Return true if e is EINPROGRESS or the local equivalent as returned by
 * a call to connect(). */
#define ERRNO_IS_CONN_EINPROGRESS(e) \
  ((e) == WSAEINPROGRESS || (e)== WSAEINVAL || (e) == WSAEWOULDBLOCK)
/** Return true if e is EAGAIN or another error indicating that a call to
 * accept() has no pending connections to return. */
#define ERRNO_IS_ACCEPT_EAGAIN(e)    ERRNO_IS_EAGAIN(e)
/** Return true if e is EMFILE or another error indicating that a call to
 * accept() has failed because we're out of fds or something. */
#define ERRNO_IS_RESOURCE_LIMIT(e) \
  ((e) == WSAEMFILE || (e) == WSAENOBUFS)
/** Return true if e is EADDRINUSE or the local equivalent. */
#define ERRNO_IS_EADDRINUSE(e)      ((e) == WSAEADDRINUSE)
/** Return true if e is EINTR  or the local equivalent */
#define ERRNO_IS_EINTR(e)            ((e) == WSAEINTR || 0)
int qed_hs_socket_errno(qed_hs_socket_t sock);
const char *qed_hs_socket_strerror(int e);
#else /* !defined(_WIN32) */
#define SOCK_ERRNO(e) e
#if EAGAIN == EWOULDBLOCK
/* || 0 is for -Wparentheses-equality (-Wall?) appeasement under clang */
#define ERRNO_IS_EAGAIN(e)           ((e) == EAGAIN || 0)
#else
#define ERRNO_IS_EAGAIN(e)           ((e) == EAGAIN || (e) == EWOULDBLOCK)
#endif /* EAGAIN == EWOULDBLOCK */
#define ERRNO_IS_EINTR(e)            ((e) == EINTR || 0)
#define ERRNO_IS_EINPROGRESS(e)      ((e) == EINPROGRESS || 0)
#define ERRNO_IS_CONN_EINPROGRESS(e) ((e) == EINPROGRESS || 0)
#define ERRNO_IS_ACCEPT_EAGAIN(e) \
  (ERRNO_IS_EAGAIN(e) || (e) == ECONNABORTED)
#define ERRNO_IS_RESOURCE_LIMIT(e) \
  ((e) == EMFILE || (e) == ENFILE || (e) == ENOBUFS || (e) == ENOMEM)
#define ERRNO_IS_EADDRINUSE(e)       (((e) == EADDRINUSE) || 0)
#define qed_hs_socket_errno(sock)       (errno)
#define qed_hs_socket_strerror(e)       strerror(e)
#endif /* defined(_WIN32) */

#if defined(_WIN32) && !defined(SIO_IDEAL_SEND_BACKLOG_QUERY)
#define SIO_IDEAL_SEND_BACKLOG_QUERY 0x4004747b
#endif

#endif /* !defined(QED_HS_SOCKET_H) */
