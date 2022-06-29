#ifndef Y11_CLIENT_COMPOSITOR_H
#define Y11_CLIENT_COMPOSITOR_H

#include <wayland-client.h>

struct y11_client_compositor {
  struct wl_compositor* proxy;  // owning
};

struct y11_client_compositor*
y11_client_compositor_create(struct wl_compositor* proxy);

void
y11_client_compositor_destroy(struct y11_client_compositor* self);

#endif  //  Y11_CLIENT_COMPOSITOR_H
