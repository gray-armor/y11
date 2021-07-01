#ifndef Y11_H
#define Y11_H

#include <libinput.h>
#include <libudev.h>
#include <stdlib.h>
#include <wayland-server.h>

/* helper function */

static inline void *
zalloc(size_t size)
{
  return calloc(1, size);
}

/* data device manager */
#pragma GCC diagnostic ignored "-Wpedantic"  // will add later
struct y11_data_device_manager {
};

struct y11_data_device_manager *
y11_data_device_manager_create(struct wl_display *display);

/* data device */
#pragma GCC diagnostic ignored "-Wpedantic"  // will add later
struct y11_data_device {
};

struct y11_data_device *
y11_data_device_create(struct wl_client *client, uint32_t id);

struct y11_pointer;

/* seat */
struct y11_seat {
  struct y11_compositor *compositor;
  struct y11_pointer *pointer;
};

struct y11_seat *
y11_seat_create(struct y11_compositor *compositor);

/* pointer */
struct y11_pointer {
  struct wl_list clients;
  struct y11_seat *seat;
  struct udev *udev;
  struct libinput *libinput;
  struct wl_event_source *libinput_source;
  int counter;
};

struct y11_pointer *
y11_pointer_create(struct y11_seat *seat);

/* pointer client */
struct y11_pointer_client {
  struct wl_list link;
  struct wl_resource *resource;
  struct y11_pointer *pointer;
  struct wl_client *client;
  uint32_t serial;
  bool enter;
};

struct y11_pointer_client *
y11_pointer_client_create(struct wl_client *client, struct y11_pointer *pointer, uint32_t id);

/* compositor */
struct y11_compositor {
  struct wl_display *display;
  struct wl_resource *resource;

  // need to annotate that this can be NULL
  struct y11_output *output;
  struct y11_xdg_shell_desktop *desktop;
};

struct y11_compositor *
y11_compositor_create();

/* callback */
struct y11_callback {
  struct wl_resource *resource;
};

struct y11_callback *
y11_callback_create(struct wl_client *client, uint32_t id);

/* surface state */
struct y11_surface_state {
  struct wl_resource *buffer_resource;
  int32_t sx;
  int32_t sy;
  struct y11_callback *callback;
};

void
y11_surface_state_reset(struct y11_surface_state *surface_state);

struct y11_surface_state *
y11_surface_state_create(struct wl_client *client);

void
y11_surface_state_destroy(struct y11_surface_state *surface_state);

/* surface */
struct y11_surface {
  struct wl_resource *resource;
  struct y11_compositor *compositor;
  struct wl_signal commit_signal;
  struct y11_surface_state *pending;
};

struct y11_surface *
y11_surface_create(struct wl_client *client, struct y11_compositor *compositor, uint32_t version,
                   uint32_t id);

void
y11_surface_destroy(struct y11_surface *surface);

/* xdg shell desktop client */
struct y11_xdg_shell_desktop_client {
  struct wl_list link;
  struct y11_xdg_shell_desktop *desktop;
  struct wl_client *client;
  struct wl_list xdg_surfaces;
};

struct y11_xdg_shell_desktop_client *
y11_xdg_shell_desktop_client_create(struct wl_client *client, struct y11_xdg_shell_desktop *desktop,
                                    uint32_t version, uint32_t id);

/* xdg shell desktop */
struct y11_xdg_shell_desktop {
  struct y11_compositor *compositor;
  struct wl_list clients;
};

struct y11_xdg_shell_desktop *
y11_xdg_shell_desktop_create(struct y11_compositor *compositor);

/* xdg surface */
struct y11_xdg_surface {
  struct wl_list link;
  struct wl_resource *resource;
  struct y11_surface *surface;
  struct y11_xdg_shell_desktop_client *desktop_client;
  struct wl_listener surface_commit_listener;
};

struct y11_xdg_surface *
y11_xdg_surface_create(struct wl_client *client, struct y11_surface *surface,
                       struct y11_xdg_shell_desktop_client *desktop_client, uint32_t version, uint32_t id);

/* xdg toplevel */
struct y11_xdg_toplevel {
  struct y11_xdg_surface *surface;
};

struct y11_xdg_toplevel *
y11_xdg_toplevel_create(struct wl_client *client, struct y11_xdg_surface *xdg_surface, uint32_t version,
                        uint32_t id);

/* output */
struct y11_output {
  struct wl_list clients;
};

struct y11_output *
y11_output_create(struct y11_compositor *compositor);

/* output client*/
struct y11_output_client {
  struct wl_list link;
  struct wl_client *client;
  struct wl_resource *resource;
};

struct y11_output_client *
y11_output_client_create(struct wl_client *client, struct y11_output *output, uint32_t version, uint32_t id);

/* region */
#pragma GCC diagnostic ignored "-Wpedantic"  // will add later
struct y11_region {
};

struct y11_region *
y11_region_create(struct wl_client *client, uint32_t id);

/* zxdg output manager */
#pragma GCC diagnostic ignored "-Wpedantic"  // will add later
struct y11_zxdg_output_manager {
};

struct y11_zxdg_output_manager *
y11_zxdg_output_manager_create(struct y11_compositor *compositor);

/* xdg output */
#pragma GCC diagnostic ignored "-Wpedantic"  // will add later
struct y11_xdg_output {
  struct wl_resource *resource;
};

struct y11_xdg_output *
y11_xdg_output_create(struct wl_client *client, uint32_t id);

#endif  // Y11_H
