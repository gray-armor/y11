#include <wayland-server.h>

#include "y11.h"

int
main()
{
  struct y11_compositor *compositor;
  struct y11_xdg_shell_desktop *desktop;
  const char *socket;

  compositor = y11_compositor_create();
  if (compositor == NULL) {
    return 1;  // TODO: Error log
  }

  desktop = y11_xdg_shell_desktop_create(compositor);
  if (desktop == NULL) {
    return 1;  // TODO: Error log
  }

  wl_display_init_shm(compositor->display);

  socket = wl_display_add_socket_auto(compositor->display);
  if (socket == NULL) {
    return 1;  // TODO: Error log
  }

  wl_display_run(compositor->display);
}
