#include <stdlib.h>
#include <wayland-server.h>

#include "y11.h"

void
y11_surface_state_reset(struct y11_surface_state *surface_state)
{
  surface_state->buffer_resource = NULL;
  surface_state->sx = 0;
  surface_state->sy = 0;
}

struct y11_surface_state *
y11_surface_state_create(struct wl_client *client)
{
  struct y11_surface_state *surface_state;

  surface_state = zalloc(sizeof *surface_state);
  if (!surface_state) goto no_mem_surface_state;

  surface_state->buffer_resource = NULL;
  surface_state->sx = 0;
  surface_state->sy = 0;
  surface_state->callback = NULL;

  return surface_state;

no_mem_surface_state:
  wl_client_post_no_memory(client);
  return NULL;
}

void
y11_surface_state_destroy(struct y11_surface_state *surface_state)
{
  free(surface_state);
}
