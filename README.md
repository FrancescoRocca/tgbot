# tgbot

A minimal C Telegram API library.

## Requirements

- libcurl
- json-c

> Note: This project is purely educational. It does not aim to cover the entire Telegram Bot API, but only a selected subset of methods.

## How to build

```bash
$ meson setup build
$ cd build
$ meson compile
$ ninja install
```

## Examples

You can find some examples [here](./examples/).

### Supported Types

See [types.md](./docs/types.md).

#### Supported Methods

See [methods.md](./docs/methods.md).

## Roadmap

- `sendAudio`
- `sendDocument`
- `sendVideo`
