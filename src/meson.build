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
]


dependencies = [
  wayland_server_dep,
  libinput_dep,
  libudev_dep,
]

scanner = dependency('wayland-scanner')
scanner_path = scanner.get_variable(pkgconfig : 'wayland_scanner')

xdg_shell_server_protocol_h = custom_target(
  'xdg-shell-server-protocol',
  output : 'xdg-shell-server-protocol.h',
  input : '/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml',
  command : [scanner_path, 'server-header', '@INPUT@', '@OUTPUT@'],
)

xdg_shell_protocol_c = custom_target(
  'xdg-shell-protocol',
  output : 'xdg-shell-protocol.c',
  input : '/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml',
  command : [scanner_path, 'code', '@INPUT@', '@OUTPUT@'],
)

xdg_output_server_protocol_h = custom_target(
  'xdg-output-server-protocol',
  output : 'xdg-output-server-protocol.h',
  input : '/usr/share/wayland-protocols/unstable/xdg-output/xdg-output-unstable-v1.xml',
  command : [scanner_path, 'server-header', '@INPUT@', '@OUTPUT@'],
)

xdg_output_protocol_c = custom_target(
  'xdg-output-protocol',
  output : 'xdg-output-protocol.c',
  input : '/usr/share/wayland-protocols/unstable/xdg-output/xdg-output-unstable-v1.xml',
  command : [scanner_path, 'code', '@INPUT@', '@OUTPUT@'],
)

source_files += xdg_shell_server_protocol_h
source_files += xdg_shell_protocol_c
source_files += xdg_output_server_protocol_h
source_files += xdg_output_protocol_c

executable(
  meson.project_name(),
  source_files,
  install : true,
  dependencies : dependencies,
)
