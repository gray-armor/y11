#include "y11.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_data_device_manager_protocol_create_data_source(struct wl_client *client, struct wl_resource *resource,
                                                    uint32_t id)
{
  // TODO: implementation
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_data_device_manager_protocol_get_data_device(struct wl_client *client, struct wl_resource *resource,
                                                 uint32_t id, struct wl_resource *seat_resource)
{
  // TODO: implementation
}

static const struct wl_data_device_manager_interface y11_data_device_manager_interface = {
    .create_data_source = y11_data_device_manager_protocol_create_data_source,
    .get_data_device = y11_data_device_manager_protocol_get_data_device,
};

static void
y11_data_device_manager_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct wl_resource *resource;

  resource = wl_resource_create(client, &wl_data_device_manager_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_data_device_manager_interface, NULL, NULL);

  return;

no_mem_resource:
  wl_client_post_no_memory(client);
}

struct y11_data_device_manager *
y11_data_device_manager_create(struct wl_display *display)
{
  struct y11_data_device_manager *device_manager;

  device_manager = zalloc(sizeof *device_manager);
  if (device_manager == NULL) goto fail;

  if (wl_global_create(display, &wl_data_device_manager_interface, 3, device_manager,
                       y11_data_device_manager_bind) == NULL)
    goto fail;

  return device_manager;

fail:
  return NULL;
}
