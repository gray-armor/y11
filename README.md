# Y11

Wayland compositor with XR output

## Build

```sh
$ meson --prefix=$(pwd) build
$ ninja -C build/ install
```

## Run

### start server

```shell
WAYLAND_DEBUG=1 XDG_RUNTIME_DIR=~/.xdg ./bin/y11
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


next

マウス入力を受け取れるようにする。
イベントループにうまく入るようにする
westonのコードで
`wl_event_loop_get_fd`とか、libinputを初期化する部分とかがキーワード
