#include <wayland-server.h>

#include "xdg-output-server-protocol.h"
#include "y11.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_zxdg_output_manager_protocol_destroy(struct wl_client *client, struct wl_resource *resource)
{
  // no thing to do
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_zxdg_output_manager_protocol_get_xdg_output(struct wl_client *client, struct wl_resource *resource,
                                                uint32_t id, struct wl_resource *output)
{
  struct y11_xdg_output *xdg_output;
  xdg_output = y11_xdg_output_create(client, id);
  if (xdg_output == NULL) {
    // TODO: Error log
    return;
  }

  zxdg_output_v1_send_logical_position(xdg_output->resource, 0, 0);
  zxdg_output_v1_send_logical_size(xdg_output->resource, 1920, 1080);
  zxdg_output_v1_send_done(xdg_output->resource);
}

static const struct zxdg_output_manager_v1_interface y11_zxdg_output_manager_interface = {
    .destroy = y11_zxdg_output_manager_protocol_destroy,
    .get_xdg_output = y11_zxdg_output_manager_protocol_get_xdg_output,
};

static void
y11_zxdg_output_manager_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct y11_zxdg_output_manager *output_manager = data;
  struct wl_resource *resource;

  resource = wl_resource_create(client, &zxdg_output_manager_v1_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_zxdg_output_manager_interface, output_manager, NULL);

  return;

no_mem_resource:
  wl_client_post_no_memory(client);
}

struct y11_zxdg_output_manager *
y11_zxdg_output_manager_create(struct y11_compositor *compositor)
{
  struct y11_zxdg_output_manager *output_manager;

  output_manager = zalloc(sizeof *output_manager);
  if (output_manager == NULL) goto fail;

  if (wl_global_create(compositor->display, &zxdg_output_manager_v1_interface, 3, output_manager,
                       y11_zxdg_output_manager_bind) == NULL)
    goto fail;

  return output_manager;

fail:
  return NULL;
}
