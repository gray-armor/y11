#include <stdlib.h>
#include <wayland-server.h>

#include "y11.h"

static void
y11_surface_handle_destroy(struct wl_resource *resource)
{
  struct y11_surface *surface;

  surface = wl_resource_get_user_data(resource);

  y11_surface_destroy(surface);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_destroy(struct wl_client *_, struct wl_resource *resource)
{
  wl_resource_destroy(resource);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_attach(struct wl_client *client, struct wl_resource *resource,
                            struct wl_resource *buffer_resource, int32_t x, int32_t y)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_damage(struct wl_client *client, struct wl_resource *resource, int32_t x, int32_t y,
                            int32_t width, int32_t height)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_frame(struct wl_client *client, struct wl_resource *resource, uint32_t callback)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_set_opaque_region(struct wl_client *client, struct wl_resource *resource,
                                       struct wl_resource *region_resource)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_set_input_region(struct wl_client *client, struct wl_resource *resource,
                                      struct wl_resource *region_resource)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_commit(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_set_buffer_transform(struct wl_client *client, struct wl_resource *resource,
                                          int32_t transform)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_set_buffer_scale(struct wl_client *client, struct wl_resource *resource, int32_t scale)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_damage_buffer(struct wl_client *client, struct wl_resource *resource, int32_t x,
                                   int32_t y, int32_t width, int32_t height)
{
  // TODO: implement
}

static const struct wl_surface_interface surface_interface = {
    .destroy = y11_surface_protocol_destroy,
    .attach = y11_surface_protocol_attach,
    .damage = y11_surface_protocol_damage,
    .frame = y11_surface_protocol_frame,
    .set_opaque_region = y11_surface_protocol_set_opaque_region,
    .set_input_region = y11_surface_protocol_set_input_region,
    .commit = y11_surface_protocol_commit,
    .set_buffer_transform = y11_surface_protocol_set_buffer_transform,
    .set_buffer_scale = y11_surface_protocol_set_buffer_scale,
    .damage_buffer = y11_surface_protocol_damage_buffer,
};

struct y11_surface *
y11_surface_create(struct wl_client *client, struct y11_compositor *compositor, uint32_t version, uint32_t id)
{
  struct y11_surface *surface;
  struct wl_resource *resource;

  surface = malloc(sizeof *surface);
  if (!surface) goto no_mem_surface;

  surface->compositor = compositor;

  resource = wl_resource_create(client, &wl_surface_interface, version, id);
  if (!resource) goto no_mem_resource;

  wl_resource_set_implementation(resource, &surface_interface, surface, y11_surface_handle_destroy);

  return surface;

no_mem_resource:
  free(surface);

no_mem_surface:
  wl_resource_post_no_memory(resource);
  return NULL;
}

void
y11_surface_destroy(struct y11_surface *surface)
{
  // Should I free the resource for this surface?
  free(surface);
}
