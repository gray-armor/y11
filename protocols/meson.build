scanner = find_program(wayland_scanner_dep.get_variable('wayland_scanner'))
wayland_protocols_base_dir = wayland_protocols_dep.get_variable('pkgdatadir')

xdg_shell_xml = join_paths(wayland_protocols_base_dir, 'stable', 'xdg-shell', 'xdg-shell.xml')

xdg_shell_server_protocol_h = custom_target(
  'xdg-shell-server-protocol',
  output: 'xdg-shell-server-protocol.h',
  input: xdg_shell_xml,
  command: [scanner, 'server-header', '@INPUT@', '@OUTPUT@'],
)

xdg_shell_protocol_c = custom_target(
  'xdg-shell-protocol',
  output : 'xdg-shell-protocol.c',
  input: xdg_shell_xml,
  command : [scanner, 'code', '@INPUT@', '@OUTPUT@'],
)
