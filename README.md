# tgbot

A minimal C Telegram API library.

## Requirements

- libcurl
- yyjson

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

Do you want to run an example?

```bash
$ cd build
$ ninja <EXAMPLE>
# to build the echobot:
$ ninja echobot
```

### Supported Types

See [types.md](./docs/types.md).

#### Supported Methods

See [methods.md](./docs/methods.md).

## Roadmap

- `sendAudio`
- `sendDocument`
- `sendVideo`
