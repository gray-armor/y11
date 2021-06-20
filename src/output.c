#include <stdlib.h>
#include <wayland-server.h>

#include "y11.h"

static void
y11_output_destroy(struct y11_output *output);

static void
y11_output_handle_destroy(struct wl_resource *resource)
{
  struct y11_output *output;

  output = wl_resource_get_user_data(resource);

  y11_output_destroy(output);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_output_protocol_release(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: Implement
}

static const struct wl_output_interface output_interface = {
    .release = y11_output_protocol_release,
};

static void
y11_output_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct y11_output *output = data;
  struct wl_resource *resource;

  resource = wl_resource_create(client, &wl_output_interface, version, id);
  if (!resource) goto no_mem_resource;

  wl_resource_set_implementation(resource, &output_interface, output, y11_output_handle_destroy);

  return;

no_mem_resource:
  wl_client_post_no_memory(client);
}

struct y11_output *
y11_output_create(struct wl_display *display)
{
  struct y11_output *output;

  output = malloc(sizeof output);
  if (!output) goto no_mem_output;

  if (!wl_global_create(display, &wl_output_interface, 3, output, y11_output_bind)) goto fail_create_global;

  return output;

fail_create_global:
  free(output);

no_mem_output:
  return NULL;
}

static void
y11_output_destroy(struct y11_output *output)
{
  free(output);
}
