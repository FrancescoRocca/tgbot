# tgbot
A minimal C Telegram API Framework

## Requirements
- meson
- libcurl
- json-c

> Note: This project is purely educational. It does not aim to cover the entire Telegram Bot API, but only a selected subset of methods.

## How to build

<details>

<summary>Linux</summary>

```bash
$ meson setup build
$ cd build
$ meson compile
$ meson install
```

</details>

<details>

<summary>Windows</summary>

Install all the required library with `vcpkg` and then copy the DLL file.

```powershell
$ meson setup build --native-file meson-vcpkg.txt
$ cd build
$ meson compile
$ meson install
```

</details>

## Examples
You can find some examples [here](./examples/).

### Supported Types
- **InlineKeyboardMarkup**
  - Note: Standard `KeyboardMarkup` is intentionally not supported.

#### Supported Methods
- `getMe`
- `sendMessage`
- `editMessageText`

## Roadmap
- `sendPhoto`
- `sendAudio`
- `sendDocument`
- `sendVideo`
