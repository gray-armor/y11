#include <wayland-server.h>

#include "xdg-output-server-protocol.h"
#include "y11.h"

static void
y11_xdg_output_destroy(struct y11_xdg_output *xdg_output);

static void
y11_xdg_output_handle_destroy(struct wl_resource *resource)
{
  struct y11_xdg_output *xdg_output;

  xdg_output = wl_resource_get_user_data(resource);

  y11_xdg_output_destroy(xdg_output);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_xdg_output_protocol_destroy(struct wl_client *_, struct wl_resource *resource)
{
  wl_resource_destroy(resource);
}

static const struct zxdg_output_v1_interface y11_xdg_output_interface = {
    .destroy = y11_xdg_output_protocol_destroy,
};

struct y11_xdg_output *
y11_xdg_output_create(struct wl_client *client, uint32_t id)
{
  struct y11_xdg_output *xdg_output;
  struct wl_resource *resource;

  xdg_output = zalloc(sizeof *xdg_output);
  if (xdg_output == NULL) goto no_mem_xdg_output;

  resource = wl_resource_create(client, &zxdg_output_v1_interface, 3, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_xdg_output_interface, xdg_output,
                                 y11_xdg_output_handle_destroy);

  xdg_output->resource = resource;

  return xdg_output;

no_mem_resource:
  free(xdg_output);

no_mem_xdg_output:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_xdg_output_destroy(struct y11_xdg_output *xdg_output)
{
  free(xdg_output);
}
