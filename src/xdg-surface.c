#include <stdlib.h>
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

struct y11_xdg_surface *
y11_xdg_surface_create(struct wl_client *client, struct y11_surface *surface, uint32_t version, uint32_t id)
{
  struct wl_resource *resource;
  struct y11_xdg_surface *xdg_surface;

  xdg_surface = malloc(sizeof xdg_surface);
  if (xdg_surface == NULL) goto no_mem_xdg_surface;

  resource = wl_resource_create(client, &xdg_surface_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  xdg_surface->surface = surface;

  wl_resource_set_implementation(resource, &y11_xdg_surface_interface, xdg_surface,
                                 y11_xdg_surface_handle_destroy);

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
