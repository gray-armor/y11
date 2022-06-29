#ifndef Y11_CLINET_APP_H
#define Y11_CLINET_APP_H

#include <stdbool.h>
#include <sys/epoll.h>
#include <wayland-client.h>

#include "compositor.h"
#include "seat.h"

struct y11_client_app {
  struct wl_display *display;
  struct wl_registry *registry;
  struct y11_client_compositor *compositor;
  struct y11_client_seat *seat;

  struct epoll_event epoll_event;
  int epoll_fd;
  bool running;
};

int
y11_client_app_run(struct y11_client_app *self);

struct y11_client_app *
y11_client_app_create(struct wl_display *dsiplay);

void
y11_client_app_destroy(struct y11_client_app *self);

#endif  //  Y11_CLINET_APP_H
