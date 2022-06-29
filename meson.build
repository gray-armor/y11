project(
  'y11',
  'c',
  version : '0.0.1',
  default_options : ['warning_level=3'],
  meson_version : '>=0.58.0',
)


wayland_server_dep = dependency('wayland-server')
wayland_scanner_dep = dependency('wayland-scanner')
wayland_protocols_dep = dependency('wayland-protocols')
libinput_dep = dependency('libinput')
libudev_dep = dependency('libudev')

subdir('protocols')
subdir('src')
