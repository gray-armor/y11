#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "app.h"

int
main()
{
  struct y11_client_app *app;
  struct wl_display *display;
  int status = EXIT_FAILURE;
  const char *socket = "wayland-0";

  display = wl_display_connect(socket);
  if (display == NULL) {
    fprintf(stderr, "Failed to create display\n");
    goto err;
  }

  app = y11_client_app_create(display);
  if (app == NULL) {
    fprintf(stderr, "Failed to create app\n");
    goto err_display;
  }

  wl_display_flush(display);

  while (app->running) {
    if (y11_client_app_poll(app) != 0) goto err_app;
  }

  status = EXIT_SUCCESS;

err_app:
  y11_client_app_destroy(app);

err_display:
  wl_display_disconnect(display);

err:
  fprintf(stderr, "Exited gracefully\n");
  return status;
}
