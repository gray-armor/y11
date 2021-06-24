#include <stdlib.h>
#include <wayland-server.h>

#include "y11.h"

static void
y11_callback_destroy(struct y11_callback *callback);

static void
y11_callback_handle_destroy(struct wl_resource *resource)
{
  struct y11_callback *callback;

  callback = wl_resource_get_user_data(resource);

  y11_callback_destroy(callback);
}

struct y11_callback *
y11_callback_create(struct wl_client *client, uint32_t id)
{
  struct y11_callback *callback;
  struct wl_resource *resource;

  callback = zalloc(sizeof *callback);
  if (callback == NULL) goto no_mem_callback;

  resource = wl_resource_create(client, &wl_callback_interface, 1, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, NULL, callback, y11_callback_handle_destroy);

  callback->resource = resource;

  return callback;

no_mem_resource:
  free(callback);

no_mem_callback:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_callback_destroy(struct y11_callback *callback)
{
  free(callback);
}
