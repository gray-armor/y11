#ifndef Y11_H
#define Y11_H

#include <wayland-server.h>

/* compositor */
struct y11_compositor {
  struct wl_display *display;
  struct wl_resource *resource;
};

struct y11_compositor *
y11_compositor_create();

/* surface */
struct y11_surface {
  struct y11_compositor *compositor;
};

struct y11_surface *
y11_surface_create(struct wl_client *client, struct y11_compositor *compositor, uint32_t version,
                   uint32_t id);

void
y11_surface_destroy(struct y11_surface *surface);

/* xdg shell desktop */
#pragma GCC diagnostic ignored "-Wpedantic"  // will add later
struct y11_xdg_shell_desktop {
};

struct y11_xdg_shell_desktop *
y11_xdg_shell_desktop_create(struct wl_display *display);

/* xdg shell desktop client */
#pragma GCC diagnostic ignored "-Wpedantic"  // will add later
struct y11_xdg_shell_desktop_client {
};

struct y11_xdg_shell_desktop_client *
y11_xdg_shell_desktop_client_create(struct wl_client *client, uint32_t version, uint32_t id);

/* xdg surface */
struct y11_xdg_surface {
  struct y11_surface *surface;
};

struct y11_xdg_surface *
y11_xdg_surface_create(struct wl_client *client, struct y11_surface *surface, uint32_t version, uint32_t id);

/* xdg toplevel */
struct y11_xdg_toplevel {
  struct y11_xdg_surface *surface;
};

struct y11_xdg_toplevel *
y11_xdg_toplevel_create(struct wl_client *client, struct y11_xdg_surface *xdg_surface, uint32_t version,
                        uint32_t id);

/* output */
#pragma GCC diagnostic ignored "-Wpedantic"  // will add later
struct y11_output {
};

struct y11_output *
y11_output_create(struct wl_display *display);

#endif  // Y11_H
