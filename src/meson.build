source_files = [
  'main.c', 'y11.h',
  'callback.c',
  'compositor.c',
  'helpers.h',
  'region.c',
  'surface-state.c',
  'surface.c',
  'xdg-surface.c',
  'xdg-toplevel.c',
  'xdg-shell-desktop.c',
  'xdg-shell-desktop-client.c',
  xdg_shell_protocol_c,
  xdg_shell_server_protocol_h,
]


dependencies = [
  wayland_server_dep,
  libinput_dep,
  libudev_dep,
]

executable(
  meson.project_name(),
  source_files,
  install : true,
  dependencies : dependencies,
)
