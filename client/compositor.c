#include "compositor.h"

#include <stdlib.h>

struct y11_client_compositor*
y11_client_compositor_create(struct wl_compositor* proxy)
{
  struct y11_client_compositor* self;

  self = calloc(1, sizeof *self);
  if (self == NULL) goto err;

  self->proxy = proxy;
  wl_compositor_set_user_data(proxy, self);

  return self;

err:
  return NULL;
}

void
y11_client_compositor_destroy(struct y11_client_compositor* self)
{
  wl_compositor_destroy(self->proxy);
  free(self);
}
