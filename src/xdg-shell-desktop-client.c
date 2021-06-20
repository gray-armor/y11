#include <stdlib.h>
#include <wayland-server.h>

#include "xdg-shell-server-protocol.h"
#include "y11.h"

void
y11_xdg_shell_desktop_client_destroy(struct y11_xdg_shell_desktop_client *desktop_client);

static void
y11_surface_handle_destroy(struct wl_resource *resource)
{
  struct y11_xdg_shell_desktop_client *desktop_client;

  desktop_client = wl_resource_get_user_data(resource);

  y11_xdg_shell_desktop_client_destroy(desktop_client);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_shell_desktop_client_protocol_destroy(struct wl_client *_, struct wl_resource *resource)
{
  wl_resource_destroy(resource);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_shell_desktop_client_protocol_create_positioner(struct wl_client *client,
                                                        struct wl_resource *resource, uint32_t id)
{
  // TODO: implement
}

static void
y11_xdg_shell_desktop_client_protocol_get_xdg_surface(struct wl_client *client, struct wl_resource *resource,
                                                      uint32_t id, struct wl_resource *surface_resource)
{
  struct y11_surface *surface;
  struct y11_xdg_surface *xdg_surface;

  surface = wl_resource_get_user_data(surface_resource);

  xdg_surface = y11_xdg_surface_create(client, surface, wl_resource_get_version(resource), id);
  if (!xdg_surface) {
    // TODO: Error log
  }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_shell_desktop_client_protocol_pong(struct wl_client *client, struct wl_resource *resource,
                                           uint32_t serial)
{
  // TODO: implement
}

static const struct xdg_wm_base_interface y11_xdg_shell_desktop_interface = {
    .destroy = y11_xdg_shell_desktop_client_protocol_destroy,
    .create_positioner = y11_xdg_shell_desktop_client_protocol_create_positioner,
    .get_xdg_surface = y11_xdg_shell_desktop_client_protocol_get_xdg_surface,
    .pong = y11_xdg_shell_desktop_client_protocol_pong,
};

struct y11_xdg_shell_desktop_client *
y11_xdg_shell_desktop_client_create(struct wl_client *client, uint32_t version, uint32_t id)
{
  struct y11_xdg_shell_desktop_client *desktop_client;
  struct wl_resource *resource;

  desktop_client = malloc(sizeof *desktop_client);
  if (!desktop_client) goto no_mem_desktop_client;

  resource = wl_resource_create(client, &xdg_wm_base_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_xdg_shell_desktop_interface, desktop_client,
                                 y11_surface_handle_destroy);

  return desktop_client;

no_mem_resource:
  free(desktop_client);

no_mem_desktop_client:
  wl_client_post_no_memory(client);
  return NULL;
}

void
y11_xdg_shell_desktop_client_destroy(struct y11_xdg_shell_desktop_client *desktop_client)
{
  free(desktop_client);
}
