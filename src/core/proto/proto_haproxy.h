/* Copyright (c) 2019-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

#ifndef QED_HS_PROTO_HAPROXY_H
#define QED_HS_PROTO_HAPROXY_H

struct qed_hs_addr_port_t;

char *haproxy_format_proxy_header_line(
                                    const struct qed_hs_addr_port_t *addr_port);

#endif /* !defined(QED_HS_PROTO_HAPROXY_H) */
