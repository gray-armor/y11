#include <wayland-server.h>

#include "y11.h"

int
main()
{
  struct y11_compositor *compositor;
  struct y11_xdg_shell_desktop *desktop;
  struct y11_data_device_manager *device_manager;
  struct y11_seat *seat;
  struct y11_shell *shell;
  struct y11_zxdg_output_manager *output_manager;
  struct y11_output *output;
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

  device_manager = y11_data_device_manager_create(compositor->display);
  if (device_manager == NULL) {
    return 1;  // TODO: Error log
  }

  seat = y11_seat_create(compositor);
  if (seat == NULL) {
    return 1;  // TODO: Error log
  }

  shell = y11_shell_create(compositor);
  if (shell == NULL) {
    return 1;  // TODO: Error log
  }

  output_manager = y11_zxdg_output_manager_create(compositor);
  if (output_manager == NULL) {
    return 1;  // TODO: Error log
  }

  output = y11_output_create(compositor);
  if (output == NULL) {
    return 1;  // TODO: Error log
  }

  socket = wl_display_add_socket_auto(compositor->display);
  if (socket == NULL) {
    return 1;  // TODO: Error log
  }

  wl_display_run(compositor->display);
}
