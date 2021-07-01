#include <wayland-server.h>

#include "y11.h"

static void
y11_shell_surface_destory(struct y11_shell_surface *shell_surface);

static void
y11_shell_surface_handle_destroy(struct wl_resource *resource)
{
  struct y11_shell_surface *shell_surface;

  shell_surface = wl_resource_get_user_data(resource);

  y11_shell_surface_destory(shell_surface);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_pong(struct wl_client *client, struct wl_resource *resource, uint32_t serial)
{
  // TODO: implament
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_move(struct wl_client *client, struct wl_resource *resource,
                                struct wl_resource *seat, uint32_t serial)
{
  // TODO: implament
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_resize(struct wl_client *client, struct wl_resource *resource,
                                  struct wl_resource *seat, uint32_t serial, uint32_t edges)
{
  // TODO: implament
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_set_toplevel(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implament
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_set_transient(struct wl_client *client, struct wl_resource *resource,
                                         struct wl_resource *parent, int32_t x, int32_t y, uint32_t flags)
{
  // TODO: implament
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_set_fullscreen(struct wl_client *client, struct wl_resource *resource,
                                          uint32_t method, uint32_t framerate, struct wl_resource *output)
{
  // TODO: implament
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_set_popup(struct wl_client *client, struct wl_resource *resource,
                                     struct wl_resource *seat, uint32_t serial, struct wl_resource *parent,
                                     int32_t x, int32_t y, uint32_t flags)
{
  // TODO: implament
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_set_maximized(struct wl_client *client, struct wl_resource *resource,
                                         struct wl_resource *output)
{
  // TODO: implament
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_set_title(struct wl_client *client, struct wl_resource *resource,
                                     const char *title)
{
  // TODO: implament
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_surface_protocol_set_class(struct wl_client *client, struct wl_resource *resource,
                                     const char *class_)
{
  // TODO: implament
}

static const struct wl_shell_surface_interface y11_shell_surface_interface = {
    .pong = y11_shell_surface_protocol_pong,
    .move = y11_shell_surface_protocol_move,
    .resize = y11_shell_surface_protocol_resize,
    .set_toplevel = y11_shell_surface_protocol_set_toplevel,
    .set_transient = y11_shell_surface_protocol_set_transient,
    .set_fullscreen = y11_shell_surface_protocol_set_fullscreen,
    .set_popup = y11_shell_surface_protocol_set_popup,
    .set_maximized = y11_shell_surface_protocol_set_maximized,
    .set_title = y11_shell_surface_protocol_set_title,
    .set_class = y11_shell_surface_protocol_set_class,
};

struct y11_shell_surface *
y11_shell_surface_create(struct wl_client *client, uint32_t id)
{
  struct y11_shell_surface *shell_surface;
  struct wl_resource *resource;

  shell_surface = zalloc(sizeof *shell_surface);
  if (shell_surface == NULL) goto no_mem_shell_surface;

  resource = wl_resource_create(client, &wl_shell_surface_interface, 1, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_shell_surface_interface, shell_surface,
                                 y11_shell_surface_handle_destroy);

  return shell_surface;

no_mem_resource:
  free(shell_surface);

no_mem_shell_surface:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_shell_surface_destory(struct y11_shell_surface *shell_surface)
{
  free(shell_surface);
}
