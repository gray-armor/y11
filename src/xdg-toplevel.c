#include <stdlib.h>
#include <wayland-server.h>

#include "xdg-shell-server-protocol.h"
#include "y11.h"

static void
y11_xdg_toplevel_destroy(struct y11_xdg_toplevel *xdg_toplevel);

static void
y11_xdg_toplevel_handle_destroy(struct wl_resource *resource)
{
  struct y11_xdg_toplevel *xdg_toplevel;

  xdg_toplevel = wl_resource_get_user_data(resource);

  y11_xdg_toplevel_destroy(xdg_toplevel);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_destroy(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_set_parent(struct wl_client *client, struct wl_resource *resource,
                                     struct wl_resource *parent)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_set_title(struct wl_client *client, struct wl_resource *resource, const char *title)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_set_app_id(struct wl_client *client, struct wl_resource *resource,
                                     const char *app_id)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_show_window_menu(struct wl_client *client, struct wl_resource *resource,
                                           struct wl_resource *seat, uint32_t serial, int32_t x, int32_t y)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_move(struct wl_client *client, struct wl_resource *resource,
                               struct wl_resource *seat, uint32_t serial)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_resize(struct wl_client *client, struct wl_resource *resource,
                                 struct wl_resource *seat, uint32_t serial, uint32_t edges)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_set_max_size(struct wl_client *client, struct wl_resource *resource, int32_t width,
                                       int32_t height)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_set_min_size(struct wl_client *client, struct wl_resource *resource, int32_t width,
                                       int32_t height)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_set_maximized(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_unset_maximized(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_set_fullscreen(struct wl_client *client, struct wl_resource *resource,
                                         struct wl_resource *output)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_toplevel_protocol_set_minimized(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implement
}

static const struct xdg_toplevel_interface y11_xdg_toplevel_interface = {
    .destroy = y11_xdg_toplevel_protocol_destroy,
    .set_parent = y11_xdg_toplevel_protocol_set_parent,
    .set_title = y11_xdg_toplevel_protocol_set_title,
    .set_app_id = y11_xdg_toplevel_protocol_set_app_id,
    .show_window_menu = y11_xdg_toplevel_protocol_show_window_menu,
    .move = y11_xdg_toplevel_protocol_move,
    .resize = y11_xdg_toplevel_protocol_resize,
    .set_max_size = y11_xdg_toplevel_protocol_set_max_size,
    .set_min_size = y11_xdg_toplevel_protocol_set_min_size,
    .set_maximized = y11_xdg_toplevel_protocol_set_maximized,
    .unset_maximized = y11_xdg_toplevel_protocol_unset_maximized,
    .set_fullscreen = y11_xdg_toplevel_protocol_set_fullscreen,
    .set_minimized = y11_xdg_toplevel_protocol_set_minimized,
};

struct y11_xdg_toplevel *
y11_xdg_toplevel_create(struct wl_client *client, struct y11_xdg_surface *xdg_surface, uint32_t version,
                        uint32_t id)
{
  struct wl_resource *resource;
  struct y11_xdg_toplevel *xdg_toplevel;

  xdg_toplevel = zalloc(sizeof *xdg_toplevel);
  if (!xdg_toplevel) goto no_mem_xdg_toplevel;

  xdg_toplevel->surface = xdg_surface;

  resource = wl_resource_create(client, &xdg_toplevel_interface, version, id);
  if (!resource) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_xdg_toplevel_interface, xdg_toplevel,
                                 y11_xdg_toplevel_handle_destroy);

  return xdg_toplevel;

no_mem_resource:
  free(xdg_toplevel);

no_mem_xdg_toplevel:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_xdg_toplevel_destroy(struct y11_xdg_toplevel *xdg_toplevel)
{
  free(xdg_toplevel);
}
