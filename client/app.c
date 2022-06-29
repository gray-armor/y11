#include "app.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>

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

int
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

  self->epoll_event.data.ptr = self;
  self->epoll_event.events = EPOLLIN;

  if (epoll_ctl(self->epoll_fd, EPOLL_CTL_ADD, wl_display_get_fd(display), &self->epoll_event) == -1) {
    fprintf(stderr, "Failed to add wayland event fd to epoll fd\n");
    goto err_close;
  }

  self->display = display;
  self->running = true;

  return self;

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
  free(self);
}
