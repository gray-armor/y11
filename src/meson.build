source_files = [
  'main.c', 'y11.h',
  'callback.c',
  'compositor.c',
  'data-device-manager.c',
  'data-device.c',
  'helpers.h',
  'output.c',
  'output-client.c',
  'pointer-client.c',
  'region.c',
  'seat.c',
  'surface-state.c',
  'surface.c',
  'xdg-surface.c',
  'xdg-toplevel.c',
  'xdg-shell-desktop.c',
  'xdg-shell-desktop-client.c',
]

wl_server = dependency('wayland-server')
libinput = dependency('libinput')

scanner = dependency('wayland-scanner')
scanner_path = scanner.get_pkgconfig_variable('wayland_scanner')

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

source_files += xdg_shell_server_protocol_h
source_files += xdg_shell_protocol_c

executable(
  meson.project_name(),
  source_files,
  install : true,
  dependencies : [wl_server, libinput],
)
