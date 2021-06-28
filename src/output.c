#include <wayland-server.h>

#include "y11.h"

static void
y11_output_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct y11_output *output = data;
  struct y11_output_client *output_client;

  output_client = y11_output_client_create(client, output, version, id);
  if (output_client == NULL) {
    // TODO: Error log
    return;
  }

  wl_output_send_geometry(output_client->resource, 0, 0, 480, 270, 0, "GrayArmor", "GrayArmor Display", 0);
  wl_output_send_scale(output_client->resource, 1);
  wl_output_send_mode(output_client->resource, 3, 1920, 1080, 60000);
  wl_output_send_done(output_client->resource);
}

struct y11_output *
y11_output_create(struct y11_compositor *compositor)
{
  struct y11_output *output;

  output = zalloc(sizeof *output);
  if (output == NULL) goto no_mem_output;

  if (wl_global_create(compositor->display, &wl_output_interface, 3, output, y11_output_bind) == NULL)
    goto fail_create_global;

  wl_list_init(&output->clients);
  compositor->output = output;

  return output;

fail_create_global:
  free(output);

no_mem_output:
  return NULL;
}
