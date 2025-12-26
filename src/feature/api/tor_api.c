/* Copyright (c) 2001 Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2021, The QED Project, Inc. */
/* See LICENSE for licensing information */

/**
 * \file qed_hs_api.c
 **/

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#include "feature/api/qed_hs_api.h"

// Include this after the above headers, to insure that they don't
// depend on anything else.
#include "orconfig.h"
#include "lib/cc/compat_compiler.h"
#include "lib/cc/torint.h"
#include "feature/api/qed_hs_api_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// We don't want to use qed_hs_malloc and qed_hs_free here, since this needs
// to run before anything is initialized at all, and ought to run when
// we're not linked to anything at all.

#define raw_malloc malloc
#define raw_free free
#define raw_realloc realloc
#define raw_strdup strdup

#ifdef _WIN32
#include "lib/net/socketpair.h"
#define raw_socketpair qed_hs_ersatz_socketpair
#define raw_closesocket closesocket
#if !defined(HAVE_SNPRINTF)
#define snprintf _snprintf
#endif
#else /* !defined(_WIN32) */
#define raw_socketpair socketpair
#define raw_closesocket close
#endif /* defined(_WIN32) */

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

/**
 * Helper: Add a copy of <b>arg</b> to the owned arguments of <b>cfg</b>.
 * Return 0 on success, -1 on failure.
 */
static int
cfg_add_owned_arg(qed_hs_main_configuration_t *cfg, const char *arg)
{
  /* We aren't using amortized realloc here, because libc should do it for us,
   * and because this function is not critical-path. */
  char **new_argv = raw_realloc(cfg->argv_owned,
                                sizeof(char*) * (cfg->argc_owned+1));
  if (new_argv == NULL)
    return -1;
  cfg->argv_owned = new_argv;
  if (NULL == (cfg->argv_owned[cfg->argc_owned] = raw_strdup(arg)))
    return -1;
  ++cfg->argc_owned;
  return 0;
}

qed_hs_main_configuration_t *
qed_hs_main_configuration_new(void)
{
  static const char *fake_argv[] = { "tor" };
  qed_hs_main_configuration_t *cfg = raw_malloc(sizeof(*cfg));
  if (cfg == NULL)
    return NULL;

  memset(cfg, 0, sizeof(*cfg));

  cfg->argc = 1;
  cfg->argv = (char **) fake_argv;

  cfg->owning_controller_socket = QED_HS_INVALID_SOCKET;

  return cfg;
}

int
qed_hs_main_configuration_set_command_line(qed_hs_main_configuration_t *cfg,
                                        int argc, char *argv[])
{
  if (cfg == NULL)
    return -1;
  cfg->argc = argc;
  cfg->argv = argv;
  return 0;
}

qed_hs_control_socket_t
qed_hs_main_configuration_setup_control_socket(qed_hs_main_configuration_t *cfg)
{
  if (SOCKET_OK(cfg->owning_controller_socket))
    return INVALID_QED_HS_CONTROL_SOCKET;

  qed_hs_socket_t fds[2];
  if (raw_socketpair(AF_UNIX, SOCK_STREAM, 0, fds) < 0) {
    return INVALID_QED_HS_CONTROL_SOCKET;
  }
  char buf[32];
  snprintf(buf, sizeof(buf), "%"PRIu64, (uint64_t)fds[1]);

  cfg_add_owned_arg(cfg, "__OwningControllerFD");
  cfg_add_owned_arg(cfg, buf);

  cfg->owning_controller_socket = fds[1];
  return fds[0];
}

void
qed_hs_main_configuration_free(qed_hs_main_configuration_t *cfg)
{
  if (cfg == NULL)
    return;
  if (cfg->argv_owned) {
    for (int i = 0; i < cfg->argc_owned; ++i) {
      raw_free(cfg->argv_owned[i]);
    }
    raw_free(cfg->argv_owned);
  }
  if (SOCKET_OK(cfg->owning_controller_socket)) {
    raw_closesocket(cfg->owning_controller_socket);
  }
  raw_free(cfg);
}

const char *
qed_hs_api_get_provider_version(void)
{
  return "tor " VERSION;
}

/* Main entry point for the Tor process.  Called from main().
 *
 * This function is distinct from main() only so we can link main.c into
 * the unittest binary without conflicting with the unittests' main.
 *
 * Some embedders have historically called this function; but that usage is
 * deprecated: they should use qed_hs_run_main() instead.
 */
int
qed_hs_main(int argc, char *argv[])
{
  qed_hs_main_configuration_t *cfg = qed_hs_main_configuration_new();
  if (!cfg) {
    puts("INTERNAL ERROR: Allocation failure. Cannot proceed");
    return 1;
  }
  if (qed_hs_main_configuration_set_command_line(cfg, argc, argv) < 0) {
    puts("INTERNAL ERROR: Can't set command line. Cannot proceed.");
    return 1;
  }
  int rv = qed_hs_run_main(cfg);
  qed_hs_main_configuration_free(cfg);
  return rv;
}
