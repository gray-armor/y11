#include "y11.h"

static void
y11_region_destroy(struct y11_region *region);

static void
y11_region_handle_destroy(struct wl_resource *resource)
{
  struct y11_region *region;

  region = wl_resource_get_user_data(resource);

  y11_region_destroy(region);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_region_protocol_destroy(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_region_protocol_add(struct wl_client *client, struct wl_resource *resource, int32_t x, int32_t y,
                        int32_t width, int32_t height)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_region_protocol_subtract(struct wl_client *client, struct wl_resource *resource, int32_t x, int32_t y,
                             int32_t width, int32_t height)
{
  // TODO: implement
}

static const struct wl_region_interface y11_region_interface = {
    .destroy = y11_region_protocol_destroy,
    .add = y11_region_protocol_add,
    .subtract = y11_region_protocol_subtract,
};

struct y11_region *
y11_region_create(struct wl_client *client, uint32_t id)
{
  struct y11_region *region;
  struct wl_resource *resource;

  region = zalloc(sizeof *region);
  if (!region) goto no_mem_region;

  resource = wl_resource_create(client, &wl_region_interface, 1, id);
  if (!resource) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_region_interface, region, y11_region_handle_destroy);

  return region;

no_mem_resource:
  free(region);

no_mem_region:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_region_destroy(struct y11_region *region)
{
  free(region);
}
