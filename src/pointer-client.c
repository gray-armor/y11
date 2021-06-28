#include "helpers.h"
#include "y11.h"

static void
y11_pointer_client_destroy(struct y11_pointer_client *pointer);

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
y11_pointer_client_create(struct wl_client *client, struct y11_seat *seat, uint32_t id)
{
  struct y11_pointer_client *pointer;
  struct wl_resource *resource;

  pointer = zalloc(sizeof *pointer);
  if (pointer == NULL) goto no_mem_pointer;

  resource = wl_resource_create(client, &wl_pointer_interface, MIN(wl_pointer_interface.version, 7), id);
  if (resource == NULL) goto no_mem_resource;

  pointer->resource = resource;
  pointer->seat = seat;
  pointer->client = client;

  wl_list_insert(&seat->pointer_clients, &pointer->link);

  wl_resource_set_implementation(resource, &y11_pointer_client_interface, pointer,
                                 y11_pointer_client_handle_destroy);

  return pointer;

no_mem_resource:
  free(pointer);

no_mem_pointer:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_pointer_client_destroy(struct y11_pointer_client *pointer)
{
  wl_list_remove(&pointer->link);
  free(pointer);
}
