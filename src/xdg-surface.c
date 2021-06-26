#include <wayland-server.h>

#include "xdg-shell-server-protocol.h"
#include "y11.h"

static void
y11_xdg_surface_destroy(struct y11_xdg_surface *surface);

static void
y11_xdg_surface_handle_destroy(struct wl_resource *resource)
{
  struct y11_xdg_surface *surface;

  surface = wl_resource_get_user_data(resource);

  y11_xdg_surface_destroy(surface);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_surface_protocol_destroy(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implement
}

static void
y11_xdg_surface_protocol_get_toplevel(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
  struct y11_xdg_surface *xdg_surface = wl_resource_get_user_data(resource);
  y11_xdg_toplevel_create(client, xdg_surface, wl_resource_get_version(resource), id);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_surface_protocol_get_popup(struct wl_client *client, struct wl_resource *resource, uint32_t id,
                                   struct wl_resource *parent, struct wl_resource *positioner)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_surface_protocol_set_window_geometry(struct wl_client *client, struct wl_resource *resource,
                                             int32_t x, int32_t y, int32_t width, int32_t height)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_surface_protocol_ack_configure(struct wl_client *client, struct wl_resource *resource,
                                       uint32_t serial)
{
  // TODO: implement
}

static const struct xdg_surface_interface y11_xdg_surface_interface = {
    .destroy = y11_xdg_surface_protocol_destroy,
    .get_toplevel = y11_xdg_surface_protocol_get_toplevel,
    .get_popup = y11_xdg_surface_protocol_get_popup,
    .set_window_geometry = y11_xdg_surface_protocol_set_window_geometry,
    .ack_configure = y11_xdg_surface_protocol_ack_configure,
};

static void
y11_xdg_surface_surface_commit_signal_handler(struct wl_listener *listener, void *data)
{
  struct y11_xdg_surface *xdg_surface;
  uint32_t serial;

  xdg_surface = wl_container_of(listener, xdg_surface, surface_commit_listener);
  serial = wl_display_next_serial(xdg_surface->desktop_client->desktop->compositor->display);

  xdg_surface_send_configure(xdg_surface->resource, serial);
}

struct y11_xdg_surface *
y11_xdg_surface_create(struct wl_client *client, struct y11_surface *surface,
                       struct y11_xdg_shell_desktop_client *desktop_client, uint32_t version, uint32_t id)
{
  struct wl_resource *resource;
  struct y11_xdg_surface *xdg_surface;

  xdg_surface = zalloc(sizeof *xdg_surface);
  if (xdg_surface == NULL) goto no_mem_xdg_surface;

  resource = wl_resource_create(client, &xdg_surface_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  xdg_surface->resource = resource;
  xdg_surface->surface = surface;
  xdg_surface->desktop_client = desktop_client;

  wl_resource_set_implementation(resource, &y11_xdg_surface_interface, xdg_surface,
                                 y11_xdg_surface_handle_destroy);

  xdg_surface->surface_commit_listener.notify = y11_xdg_surface_surface_commit_signal_handler;
  wl_signal_add(&xdg_surface->surface->commit_signal, &xdg_surface->surface_commit_listener);

  return xdg_surface;

no_mem_resource:
  free(xdg_surface);

no_mem_xdg_surface:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_xdg_surface_destroy(struct y11_xdg_surface *surface)
{
  free(surface);
}
