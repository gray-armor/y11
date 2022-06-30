# Y11

Minimum sample code to learn wayland.

## Build

```sh
$ meson build
$ ninja -C build
```

## Run server side sample

### start server

```shell
WAYLAND_DEBUG=1 XDG_RUNTIME_DIR=~/.xdg ./build/src/y11
```

#### Environment variable options

```env
NO_HEAD=1 # will not paint pixel maps to the console.
```

### connect client

```shell
WAYLAND_DEBUG=1 XDG_RUNTIME_DIR=~/.xdg weston-flower
```

You can see the communication messages in the debug log.

## Run client side sample

### start weston or another wayland compositor

_in a virtual terminal (Press Alt + Shift + (F2 or F3 or F4 or ...) )_
```shell
weston
```

### run sample client in weston or another wayland compositor

```shell
./build/client/y11-client
```

Plug and unplug your mouse.
You will see the current input device capabilites in the client's log.

\* keyboard capability might be always set in weston.
