#include <stdlib.h>
#include <wayland-server.h>

#include "xdg-shell-server-protocol.h"
#include "y11.h"

static void
y11_xdg_shell_desktop_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
#pragma GCC diagnostic ignored "-Wunused-variable"  // will use later
  struct y11_xdg_shell_desktop *desktop = data;
  struct y11_xdg_shell_desktop_client *desktop_client;

  desktop_client = y11_xdg_shell_desktop_client_create(client, version, id);
  if (!desktop_client) {
    // TODO: Error log
  }
}

struct y11_xdg_shell_desktop *
y11_xdg_shell_desktop_create(struct wl_display *display)
{
  struct y11_xdg_shell_desktop *desktop;

  desktop = malloc(sizeof desktop);
  if (!desktop) goto no_mem_desktop;

  if (!wl_global_create(display, &xdg_wm_base_interface, 1, desktop, y11_xdg_shell_desktop_bind))
    goto fail_create_global;

  return desktop;

fail_create_global:
  free(desktop);

no_mem_desktop:
  return NULL;
}
