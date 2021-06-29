#include <fcntl.h>
#include <libinput.h>
#include <libudev.h>
#include <stdio.h>
#include <unistd.h>
#include <wayland-server.h>

#include "y11.h"

static void
y11_pointer_enter_leave(struct y11_pointer *pointer)
{
  struct y11_compositor *compositor = pointer->seat->compositor;
  struct y11_xdg_shell_desktop_client *desktop_client;
  struct wl_client *client = NULL;

  if (wl_list_empty(&compositor->desktop->clients)) return;
  wl_list_for_each(desktop_client, &compositor->desktop->clients, link)
  {
    client = desktop_client->client;
    break;
  }

  struct y11_pointer_client *pointer_client;
  bool ok = false;
  if (wl_list_empty(&pointer->clients)) return;
  wl_list_for_each(pointer_client, &pointer->clients, link)
  {
    if (pointer_client->client == client) {
      ok = true;
      break;
    }
  }
  if (!ok) return;

  struct y11_surface *surface = NULL;
  struct y11_xdg_surface *xdg_surface;
  if (wl_list_empty(&desktop_client->xdg_surfaces)) return;
  wl_list_for_each(xdg_surface, &desktop_client->xdg_surfaces, link)
  {
    surface = xdg_surface->surface;
    break;
  }

  if (pointer_client->enter) {
    wl_pointer_send_leave(pointer_client->resource, pointer_client->serial, surface->resource);
    pointer_client->enter = false;
    pointer_client->serial += 1;
  } else {
    wl_pointer_send_enter(pointer_client->resource, pointer_client->serial, surface->resource, 3, 3);
    pointer_client->enter = true;
  }
}

static void
y11_pointer_process_events(struct y11_pointer *pointer)
{
  struct libinput_event *event;
  while ((event = libinput_get_event(pointer->libinput))) {
    pointer->counter += 1;
    if (pointer->counter == 100) {
      pointer->counter = 0;
      y11_pointer_enter_leave(pointer);
    }
  }
}

static int
y11_pointer_libinput_source_dispatch(int fd, uint32_t mask, void *data)
{
  struct y11_pointer *pointer = data;
  if (libinput_dispatch(pointer->libinput) != 0) {
    fprintf(stdout, "error libinput dispatch");
    // TODO: Error log
  }

  fflush(stdout);
  y11_pointer_process_events(pointer);

  return 0;
}

static int
y11_pointer_libinput_protocol_open_restricted(const char *path, int flags, void *user_data)
{
  // TODO: implement
  int fd = open(path, flags | O_CLOEXEC);
  if (fd < 0) {
    fprintf(stdout, "fail to open %s: %m\n", path);
  }
  return fd;
}

static void
y11_pointer_libinput_protocol_close_restricted(int fd, void *user_data)
{
  // TODO: implement
}

static const struct libinput_interface y11_libinput_interface = {
    .open_restricted = y11_pointer_libinput_protocol_open_restricted,
    .close_restricted = y11_pointer_libinput_protocol_close_restricted,
};

struct y11_pointer *
y11_pointer_create(struct y11_seat *seat)
{
  struct y11_pointer *pointer;

  pointer = zalloc(sizeof *pointer);
  if (pointer == NULL) goto no_mem_pointer;

  pointer->seat = seat;
  wl_list_init(&pointer->clients);
  pointer->udev = udev_new();
  if (pointer->udev == NULL) goto no_mem_udev;

  pointer->libinput = libinput_udev_create_context(&y11_libinput_interface, NULL, pointer->udev);
  if (pointer->libinput == NULL) goto no_mem_libinput;

  if (libinput_udev_assign_seat(pointer->libinput, "seat0") != 0) goto fail_add_fd;

  struct wl_event_loop *loop;
  int fd;
  loop = wl_display_get_event_loop(seat->compositor->display);
  fd = libinput_get_fd(pointer->libinput);
  if (fd < 0) goto fail_add_fd;
  pointer->libinput_source =
      wl_event_loop_add_fd(loop, fd, WL_EVENT_READABLE, y11_pointer_libinput_source_dispatch, pointer);

  y11_pointer_process_events(pointer);

  if (!pointer->libinput_source) goto fail_add_fd;

  pointer->counter = 0;

  seat->pointer = pointer;

  return pointer;

fail_add_fd:
  wl_event_source_remove(pointer->libinput_source);

no_mem_libinput:
  udev_unref(pointer->udev);

no_mem_udev:
  free(pointer);

no_mem_pointer:
  fprintf(stdout, "Fail to create pointer");
  return NULL;
}
