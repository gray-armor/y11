#include <stdlib.h>
#include <wayland-server.h>

#include "y11.h"

static void
y11_compositor_protocol_create_surface(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
  struct y11_compositor *compositor;
  struct y11_surface *surface;

  compositor = wl_resource_get_user_data(resource);

  surface = y11_surface_create(client, compositor, wl_resource_get_version(resource), id);
  if (!surface) {
    // TODO: Error log
  }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void
y11_compositor_protocol_create_region(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
  struct y11_region *region;

  region = y11_region_create(client, id);
  if (!region) {
    // TODO: Error log
  }
}

static const struct wl_compositor_interface compositor_interface = {
    .create_surface = y11_compositor_protocol_create_surface,
    .create_region = y11_compositor_protocol_create_region,
};

static void
y11_compositor_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct y11_compositor *compositor = data;
  struct wl_resource *resource;

  resource = wl_resource_create(client, &wl_compositor_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &compositor_interface, compositor, NULL);

  return;

no_mem_resource:
  wl_client_post_no_memory(client);
}

struct y11_compositor *
y11_compositor_create()
{
  struct y11_compositor *compositor;
  struct wl_display *display;

  compositor = zalloc(sizeof *compositor);
  if (compositor == NULL) goto fail;

  display = wl_display_create();

  compositor->display = display;

  if (!wl_global_create(display, &wl_compositor_interface, 4, compositor, y11_compositor_bind)) goto fail;

  return compositor;

fail:
  return NULL;
}
