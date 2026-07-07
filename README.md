# tgbot

A C Telegram API library.

## Requirements

- libcurl
- yyjson

> Note: This project is purely educational.

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
