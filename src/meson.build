source_files = [
  'main.c', 'y11.h',
  'callback.c',
  'compositor.c',
  'data-device-manager.c',
  'data-device.c',
  'helpers.h',
  'output.c',
  'output-client.c',
  'pointer.c',
  'pointer-client.c',
  'region.c',
  'seat.c',
  'surface-state.c',
  'surface.c',
  'xdg-output.c',
  'xdg-surface.c',
  'xdg-toplevel.c',
  'xdg-shell-desktop.c',
  'xdg-shell-desktop-client.c',
  'zxdg-output-manager.c',
]


dependencies = [
  dependency('wayland-server'),
  dependency('libinput'),
  dependency('libudev'),
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
