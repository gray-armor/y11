#include "y11.h"

static void
y11_data_device_destroy(struct y11_data_device *data_device);

static void
y11_data_device_handle_destroy(struct wl_resource *resource)
{
  struct y11_data_device *data_device;

  data_device = wl_resource_get_user_data(resource);

  y11_data_device_destroy(data_device);
}

static const struct wl_data_device_interface y11_data_device_interface = {};

struct y11_data_device *
y11_data_device_create(struct wl_client *client, uint32_t id)
{
  struct y11_data_device *data_device;
  struct wl_resource *resource;

  data_device = zalloc(sizeof *data_device);
  if (data_device == NULL) goto no_mem_data_device;

  resource = wl_resource_create(client, &wl_data_device_interface, 3, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_data_device_interface, data_device,
                                 y11_data_device_handle_destroy);

  return data_device;

no_mem_resource:
  free(data_device);

no_mem_data_device:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_data_device_destroy(struct y11_data_device *data_device)
{
  free(data_device);
}
