#include <assert.h>
#include <stdlib.h>
#include <wayland-server.h>

#include "y11.h"

int
main()
{
  struct y11_compositor *compositor;
  struct y11_xdg_shell_desktop *desktop;
  struct y11_output *output;
  const char *socket;

  compositor = y11_compositor_create();
  if (!compositor) {
    return 1;  // TODO: Error log
  }

  desktop = y11_xdg_shell_desktop_create(compositor->display);
  if (!desktop) {
    return 1;  // TODO: Error log
  }

  wl_display_init_shm(compositor->display);

  output = y11_output_create(compositor->display);
  if (!output) {
    return 1;  // TODO: Error log
  }

  socket = wl_display_add_socket_auto(compositor->display);
  if (!socket) {
    return 1;  // TODO: Error log
  }

  wl_display_run(compositor->display);
}
