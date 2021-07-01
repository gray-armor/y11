#include <wayland-server.h>

#include "y11.h"

static void
y11_shell_client_destroy(struct y11_shell_client *shell_client);

static void
y11_shell_client_handle_destroy(struct wl_resource *resource)
{
  struct y11_shell_client *shell_client;

  shell_client = wl_resource_get_user_data(resource);

  y11_shell_client_destroy(shell_client);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_shell_client_protocol_get_shell_surface(struct wl_client *client, struct wl_resource *resource,
                                            uint32_t id, struct wl_resource *surface)
{
  struct y11_shell_surface *shell_surface;

  shell_surface = y11_shell_surface_create(client, id);
  if (shell_surface == NULL) {
    return;  // TODO: Error log
  }
}

static const struct wl_shell_interface y11_shell_interface = {
    .get_shell_surface = y11_shell_client_protocol_get_shell_surface,
};

struct y11_shell_client *
y11_shell_client_create(struct wl_client *client, uint32_t version, uint32_t id)
{
  struct y11_shell_client *shell_client;
  struct wl_resource *resource;

  shell_client = zalloc(sizeof *shell_client);
  if (shell_client == NULL) goto no_mem_shell_client;

  resource = wl_resource_create(client, &wl_shell_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_shell_interface, shell_client,
                                 y11_shell_client_handle_destroy);

  return shell_client;

no_mem_resource:
  free(shell_client);

no_mem_shell_client:
  wl_client_post_no_memory(client);
  return NULL;
}

static void
y11_shell_client_destroy(struct y11_shell_client *shell_client)
{
  free(shell_client);
}
