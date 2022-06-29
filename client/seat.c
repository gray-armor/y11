#include "seat.h"

#include <stdio.h>
#include <stdlib.h>
#include <wayland-client.h>

static void
y11_client_seat_protocol_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities)
{
  struct y11_client_seat *self = data;
  (void)wl_seat;
  (void)self;

  fprintf(stderr, "[DEBUG] New capabilities\n");
  if (capabilities & WL_SEAT_CAPABILITY_POINTER)
    fprintf(stderr, "pointer  -- yes\n");
  else
    fprintf(stderr, "pointer  -- no\n");

  if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD)
    fprintf(stderr, "keyboard -- yes\n");
  else
    fprintf(stderr, "keyboard -- yes\n");
}

static void
y11_client_seat_protocol_name(void *data, struct wl_seat *wl_seat, const char *name)
{
  struct y11_client_seat *self = data;
  (void)wl_seat;
  (void)self;

  fprintf(stderr, "[DEBUG] SEAT: %s\n", name);
}

static const struct wl_seat_listener seat_listener = {
    .capabilities = y11_client_seat_protocol_capabilities,
    .name = y11_client_seat_protocol_name,
};

struct y11_client_seat *
y11_client_seat_create(struct wl_seat *proxy)
{
  struct y11_client_seat *self;

  self = calloc(1, sizeof *self);
  if (self == NULL) goto err;

  self->proxy = proxy;
  wl_seat_add_listener(proxy, &seat_listener, self);

  return self;

err:
  return NULL;
}

void
y11_client_seat_destroy(struct y11_client_seat *self)
{
  wl_seat_destroy(self->proxy);
  free(self);
}
