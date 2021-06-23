#include <stdlib.h>
#include <wayland-server.h>

#include "xdg-shell-server-protocol.h"
#include "y11.h"

static void
y11_xdg_shell_desktop_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct y11_xdg_shell_desktop *desktop = data;
  struct y11_xdg_shell_desktop_client *desktop_client;

  desktop_client = y11_xdg_shell_desktop_client_create(client, desktop, version, id);
  if (!desktop_client) {
    // TODO: Error log
  }
}

struct y11_xdg_shell_desktop *
y11_xdg_shell_desktop_create(struct y11_compositor *compositor)
{
  struct y11_xdg_shell_desktop *desktop;

  desktop = zalloc(sizeof *desktop);
  if (!desktop) goto no_mem_desktop;

  desktop->compositor = compositor;

  if (!wl_global_create(compositor->display, &xdg_wm_base_interface, 1, desktop, y11_xdg_shell_desktop_bind))
    goto fail_create_global;

  return desktop;

fail_create_global:
  free(desktop);

no_mem_desktop:
  return NULL;
}
