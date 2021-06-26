#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <wayland-server.h>

#include "y11.h"

static void
y11_surface_handle_destroy(struct wl_resource *resource)
{
  struct y11_surface *surface;

  surface = wl_resource_get_user_data(resource);

  y11_surface_destroy(surface);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_destroy(struct wl_client *_, struct wl_resource *resource)
{
  wl_resource_destroy(resource);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_attach(struct wl_client *client, struct wl_resource *resource,
                            struct wl_resource *buffer_resource, int32_t x, int32_t y)
{
  struct y11_surface *surface = wl_resource_get_user_data(resource);
  surface->pending->buffer_resource = buffer_resource;
  surface->pending->sx = x;
  surface->pending->sy = y;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_damage(struct wl_client *client, struct wl_resource *resource, int32_t x, int32_t y,
                            int32_t width, int32_t height)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_frame(struct wl_client *client, struct wl_resource *resource, uint32_t id)
{
  // TODO: implement
  struct y11_surface *surface;
  struct y11_callback *callback;

  callback = y11_callback_create(client, id);
  if (callback == NULL) {
    // TODO: Error Log
    return;
  }

  surface = wl_resource_get_user_data(resource);

  surface->pending->callback = callback;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_set_opaque_region(struct wl_client *client, struct wl_resource *resource,
                                       struct wl_resource *region_resource)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_set_input_region(struct wl_client *client, struct wl_resource *resource,
                                      struct wl_resource *region_resource)
{
  // TODO: implement
}

static void
y11_surface_protocol_commit(struct wl_client *client, struct wl_resource *resource)
{
  struct y11_surface *surface;

  surface = wl_resource_get_user_data(resource);

  if (surface->pending->buffer_resource != NULL) {
    struct wl_shm_buffer *shm_buffer = wl_shm_buffer_get(surface->pending->buffer_resource);
    uint32_t stride = wl_shm_buffer_get_stride(shm_buffer);
    uint32_t width = wl_shm_buffer_get_width(shm_buffer);
    uint32_t height = wl_shm_buffer_get_height(shm_buffer);
    uint32_t format = wl_shm_buffer_get_format(shm_buffer);
    uint32_t size = stride * height;
    volatile unsigned char unused = 0;
    uint8_t *data = wl_shm_buffer_get_data(shm_buffer);

    fprintf(stdout, "Reading data [%d x %d] %d bytes (format: %d)\n", width, height, size, format);
    fflush(stdout);

    char *NO_HEAD = getenv("NO_HEAD");
    if (NO_HEAD == NULL || strcmp(NO_HEAD, "1")) {
      // print the shape to stdout
      // Supported format
      // 0. 32-bit ARGB format, [31:0] A:R:G:B 8:8:8:8 little endian
      // 1. 32-bit RGB format, [31:0] x:R:G:B 8:8:8:8 little endian
      if (format == 0 || format == 1) {
        FILE *file = stdout;
        fprintf(file, "\x1b[0;0H");  // move cursor to (0, 0)
        wl_shm_buffer_begin_access(shm_buffer);
        for (uint32_t y = 0; y < height; y++) {
          for (uint32_t x = 0; x < width; x++) {
            uint8_t *b = data++;
            uint8_t *g = data++;
            uint8_t *r = data++;
            uint8_t *a = data++;
            if (y % 6 == 0 && x % 3 == 0) {
              if (format == 0 && *a < (UINT8_MAX / 2)) {
                fprintf(file, " ");
              } else {
                if (*r > *g && *r > *b) {
                  fprintf(file, "\x1b[31m|\x1b[39m");
                } else if (*g > *r && *g > *b) {
                  fprintf(file, "\x1b[32m|\x1b[39m");
                } else if (*b > *g && *b > *r) {
                  fprintf(file, "\x1b[34m|\x1b[39m");
                } else {
                  fprintf(file, "|");
                }
              }
            }
          }
          if (y % 6 == 0) fprintf(file, "\n");
        }
        wl_shm_buffer_end_access(shm_buffer);
        fflush(file);
      }
    }

    wl_buffer_send_release(surface->pending->buffer_resource);
    struct timeval now;
    gettimeofday(&now, NULL);
    wl_callback_send_done(surface->pending->callback->resource, now.tv_sec * 1000 + now.tv_usec / 1000);
    wl_resource_destroy(surface->pending->callback->resource);

    y11_surface_state_reset(surface->pending);
  }

  wl_signal_emit(&surface->commit_signal, surface);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_set_buffer_transform(struct wl_client *client, struct wl_resource *resource,
                                          int32_t transform)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_set_buffer_scale(struct wl_client *client, struct wl_resource *resource, int32_t scale)
{
  // TODO: implement
}

#pragma GCC diagnostic ignored "-Wunused-parameter"  // params are determined by the protocol.
static void
y11_surface_protocol_damage_buffer(struct wl_client *client, struct wl_resource *resource, int32_t x,
                                   int32_t y, int32_t width, int32_t height)
{
  // TODO: implement
}

static const struct wl_surface_interface surface_interface = {
    .destroy = y11_surface_protocol_destroy,
    .attach = y11_surface_protocol_attach,
    .damage = y11_surface_protocol_damage,
    .frame = y11_surface_protocol_frame,
    .set_opaque_region = y11_surface_protocol_set_opaque_region,
    .set_input_region = y11_surface_protocol_set_input_region,
    .commit = y11_surface_protocol_commit,
    .set_buffer_transform = y11_surface_protocol_set_buffer_transform,
    .set_buffer_scale = y11_surface_protocol_set_buffer_scale,
    .damage_buffer = y11_surface_protocol_damage_buffer,
};

struct y11_surface *
y11_surface_create(struct wl_client *client, struct y11_compositor *compositor, uint32_t version, uint32_t id)
{
  struct y11_surface *surface;
  struct wl_resource *resource;
  struct y11_surface_state *surface_state;

  surface_state = y11_surface_state_create(client);
  if (surface_state == NULL) goto fail;

  surface = zalloc(sizeof *surface);
  if (surface == NULL) goto no_mem_surface;

  wl_signal_init(&surface->commit_signal);

  surface->compositor = compositor;
  surface->pending = surface_state;

  resource = wl_resource_create(client, &wl_surface_interface, version, id);
  if (resource == NULL) goto no_mem_resource;

  wl_resource_set_implementation(resource, &surface_interface, surface, y11_surface_handle_destroy);

  return surface;

no_mem_resource:
  y11_surface_state_destroy(surface->pending);
  free(surface);

no_mem_surface:
  wl_client_post_no_memory(client);

fail:
  return NULL;
}

void
y11_surface_destroy(struct y11_surface *surface)
{
  // Should I free the resource for this surface?
  if (surface->pending) y11_surface_state_destroy(surface->pending);
  free(surface);
}
