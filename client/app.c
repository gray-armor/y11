#include "app.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <wayland-client.h>

static void
registry_global(void* data, struct wl_registry* registry, uint32_t id, const char* interface,
                uint32_t version)
{
  struct y11_client_app* self = data;
  if (strcmp(interface, "wl_compositor") == 0) {
    struct wl_compositor* proxy;
    proxy = wl_registry_bind(registry, id, &wl_compositor_interface, version);
    self->compositor = y11_client_compositor_create(proxy);
  } else if (strcmp(interface, "wl_seat") == 0) {
    struct wl_seat* proxy;
    proxy = wl_registry_bind(registry, id, &wl_seat_interface, version);
    self->seat = y11_client_seat_create(proxy);
  }
}

static void
registry_global_remove(void* data, struct wl_registry* wl_registry, uint32_t name)
{
  (void)data;
  (void)wl_registry;
  (void)name;
}

static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

static int
y11_client_app_dispatch(struct y11_client_app* self)
{
  while (wl_display_prepare_read(self->display) != 0) {
    if (errno != EAGAIN) return -1;
    wl_display_dispatch_pending(self->display);
  }

  if (wl_display_flush(self->display) != 0) return -1;
  wl_display_read_events(self->display);
  wl_display_dispatch_pending(self->display);

  return 0;
}

static int
y11_client_app_poll(struct y11_client_app* self)
{
  int epoll_count;
  int ret;
  struct epoll_event events[16];

  epoll_count = epoll_wait(self->epoll_fd, events, 16, -1);
  for (int i = 0; i < epoll_count; i++) {
    assert(events[i].data.ptr == self);
    ret = y11_client_app_dispatch(self);
    if (ret != 0) return ret;
  }

  return 0;
}

int
y11_client_app_run(struct y11_client_app* self)
{
  if (epoll_ctl(self->epoll_fd, EPOLL_CTL_ADD, wl_display_get_fd(self->display), &self->epoll_event) == -1) {
    fprintf(stderr, "Failed to add wayland event fd to epoll fd\n");
    return -1;
  }

  self->running = true;
  while (self->running) {
    if (y11_client_app_poll(self) != 0) return -1;
  }

  return 0;
}

struct y11_client_app*
y11_client_app_create(struct wl_display* display)
{
  struct y11_client_app* self;

  self = calloc(1, sizeof *self);
  if (self == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    goto err;
  }

  self->epoll_fd = epoll_create1(EPOLL_CLOEXEC);
  if (self->epoll_fd == -1) {
    fprintf(stderr, "Failed to create epoll fd\n");
    goto err_free;
  }

  self->registry = wl_display_get_registry(display);
  if (self->registry == NULL) goto err_close;

  wl_registry_add_listener(self->registry, &registry_listener, self);

  wl_display_dispatch(display);
  wl_display_roundtrip(display);

  if (self->compositor == NULL || self->seat == NULL) goto err_globals;

  self->epoll_event.data.ptr = self;
  self->epoll_event.events = EPOLLIN;
  self->display = display;
  self->running = false;

  return self;

err_globals:
  if (self->seat) y11_client_seat_destroy(self->seat);
  if (self->compositor) y11_client_compositor_destroy(self->compositor);
  wl_registry_destroy(self->registry);

err_close:
  close(self->epoll_fd);

err_free:
  free(self);

err:
  return NULL;
}

void
y11_client_app_destroy(struct y11_client_app* self)
{
  y11_client_seat_destroy(self->seat);
  y11_client_compositor_destroy(self->compositor);
  wl_registry_destroy(self->registry);
  close(self->epoll_fd);
  free(self);
}
