#include <wayland-server.h>

#include "y11.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct y11_shell_client *shell_client;

  shell_client = y11_shell_client_create(client, version, id);
  if (shell_client == NULL) {
    return;  // TODO: Error log
  }
}

struct y11_shell *
y11_shell_create(struct y11_compositor *compositor)
{
  struct y11_shell *shell;

  shell = zalloc(sizeof *shell);
  if (shell == NULL) goto fail;

  if (wl_global_create(compositor->display, &wl_shell_interface, 1, shell, y11_shell_bind) == NULL) goto fail;

  return shell;

fail:
  return NULL;
}
