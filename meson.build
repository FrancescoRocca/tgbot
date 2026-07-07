project(
    'tgbot',
    'c',
    version: '0.2',
    default_options: ['warning_level=3', 'c_std=c18'],
)

sources = []
headers = []
subdir('src')
subdir('include')

inc_dir = include_directories('include')

curl_dep = dependency('libcurl')
yyjson_dep = dependency('yyjson')

deps = [curl_dep, yyjson_dep]

install_headers(
    headers,
    subdir: 'tgbot',
)

tgbot_lib = static_library(
    'tgbot',
    sources,
    dependencies: deps,
    include_directories: inc_dir,
    install: true,
)

clangformat = find_program('clang-format', required: false)
if clangformat.found()
    run_target('format', command: [clangformat, '-i'] + sources + headers)
endif

# Examples
executable(
    'echobot',
    'examples/echobot/echobot.c',
    build_by_default: false,
    link_with: tgbot_lib,
    include_directories: inc_dir,
)

executable(
    'inlinekeyboard',
    'examples/inlinekeyboard/inlinekeyboard.c',
    build_by_default: false,
    link_with: tgbot_lib,
    include_directories: inc_dir,
)

executable(
    'sendpic',
    'examples/sender/sendpic.c',
    build_by_default: false,
    link_with: tgbot_lib,
    include_directories: inc_dir,
)
