#ifndef Y11_CLIENT_SEAT_H
#define Y11_CLIENT_SEAT_H

#include <wayland-client.h>

struct y11_client_seat {
  struct wl_seat *proxy;  // owning
};

struct y11_client_seat *
y11_client_seat_create(struct wl_seat *proxy);

void
y11_client_seat_destroy(struct y11_client_seat *self);

#endif  //  Y11_CLIENT_SEAT_H
