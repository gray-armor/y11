#include "helpers.h"
#include "y11.h"

static void
y11_pointer_client_destroy(struct y11_pointer_client *pointer_client);

static void
y11_pointer_client_handle_destroy(struct wl_resource *resource)
{
  struct y11_pointer_client *pointer;

  pointer = wl_resource_get_user_data(resource);

  y11_pointer_client_destroy(pointer);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_pointer_client_protocol_set_cursor(struct wl_client *client, struct wl_resource *resource,
                                       uint32_t serial, struct wl_resource *surface, int32_t hotspot_x,
                                       int32_t hotspot_y)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_pointer_client_protocol_release(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implement
}

static const struct wl_pointer_interface y11_pointer_client_interface = {
    .set_cursor = y11_pointer_client_protocol_set_cursor,
    .release = y11_pointer_client_protocol_release,
};

struct y11_pointer_client *
y11_pointer_client_create(struct wl_client *client, struct y11_pointer *pointer, uint32_t id)
{
  struct y11_pointer_client *pointer_client;
  struct wl_resource *resource;

  pointer_client = zalloc(sizeof *pointer_client);
  if (pointer_client == NULL) goto no_mem_pointer_client;

  resource = wl_resource_create(client, &wl_pointer_interface, MIN(wl_pointer_interface.version, 7), id);
  if (resource == NULL) goto no_mem_resource;

  pointer_client->resource = resource;
  pointer_client->pointer = pointer;
  pointer_client->client = client;

  wl_list_insert(&pointer->clients, &pointer_client->link);

  wl_resource_set_implementation(resource, &y11_pointer_client_interface, pointer_client,
                                 y11_pointer_client_handle_destroy);

  return pointer_client;

no_mem_resource:
  free(pointer_client);

no_mem_pointer_client:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_pointer_client_destroy(struct y11_pointer_client *pointer_client)
{
  wl_list_remove(&pointer_client->link);
  free(pointer_client);
}
