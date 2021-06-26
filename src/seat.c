#include "helpers.h"
#include "y11.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_seat_protocol_get_pointer(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_seat_protocol_get_keyboard(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_seat_protocol_get_touch(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_seat_protocol_release(struct wl_client *client, struct wl_resource *resource)
{
  // TODO: implement
}

static const struct wl_seat_interface y11_seat_interface = {
    .get_pointer = y11_seat_protocol_get_pointer,
    .get_keyboard = y11_seat_protocol_get_keyboard,
    .get_touch = y11_seat_protocol_get_touch,
    .release = y11_seat_protocol_release,
};

static void
y11_seat_bind(struct wl_client *client, void *data, uint32_t version, uint32_t id)
{
  struct y11_seat *seat = data;
  struct wl_resource *resource;

  resource = wl_resource_create(client, &wl_seat_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &y11_seat_interface, seat, NULL);

  return;

no_mem_resource:
  wl_client_post_no_memory(client);
}

struct y11_seat *
y11_seat_create(struct wl_display *display)
{
  struct y11_seat *seat;

  seat = zalloc(sizeof *seat);
  if (seat == NULL) goto fail;

  if (wl_global_create(display, &wl_seat_interface, MIN(wl_seat_interface.version, 7), seat, y11_seat_bind) ==
      NULL)
    goto fail;

  return seat;

fail:
  return NULL;
}
