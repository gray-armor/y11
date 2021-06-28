#include <wayland-server.h>

#include "helpers.h"
#include "y11.h"

static void
y11_output_client_destroy(struct y11_output_client *output_client);

static void
y11_output_client_handle_destroy(struct wl_resource *resource)
{
  struct y11_output_client *output_client;

  output_client = wl_resource_get_user_data(resource);

  y11_output_client_destroy(output_client);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_output_client_protocol_release(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: Implement
}

static const struct wl_output_interface output_client_interface = {
    .release = y11_output_client_protocol_release,
};

struct y11_output_client *
y11_output_client_create(struct wl_client *client, struct y11_output *output, uint32_t version, uint32_t id)
{
  struct y11_output_client *output_client;
  struct wl_resource *resource;

  output_client = zalloc(sizeof *output_client);
  if (output_client == NULL) goto no_mem_output_client;

  resource = wl_resource_create(client, &wl_output_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &output_client_interface, output_client,
                                 y11_output_client_handle_destroy);

  output_client->client = client;
  output_client->resource = resource;
  wl_list_insert(&output->clients, &output_client->link);

  return output_client;

no_mem_resource:
  free(output_client);

no_mem_output_client:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_output_client_destroy(struct y11_output_client *output_client)
{
  wl_list_remove(&output_client->link);
  free(output_client);
}
