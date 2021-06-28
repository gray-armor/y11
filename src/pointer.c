#include <wayland-server.h>

#include "y11.h"

struct y11_pointer *
y11_pointer_create(struct y11_seat *seat)
{
  struct y11_pointer *pointer;

  pointer = zalloc(sizeof *pointer);
  if (pointer == NULL) goto no_mem_pointer;

  pointer->seat = seat;
  wl_list_init(&pointer->clients);

  seat->pointer = pointer;

  return pointer;

no_mem_pointer:
  return NULL;
}
