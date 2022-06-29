client_srcs = [
  'app.c',
  'compositor.c',
  'main.c',
  'seat.c',
]

client_deps = [
  wayland_client_dep,
]

executable(
  '@0@-client'.format(meson.project_name()),
  client_srcs,
  install: false,
  dependencies: client_deps,
)
