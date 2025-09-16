project(
    'tgbot',
    'c',
    version: '0.1',
    default_options: ['warning_level=3', 'c_std=c18'],
)

sources = []
subdir('src')

inc_dir = include_directories('include')

curl_dep = dependency('libcurl')
json_c_dep = dependency('json-c')

deps = [curl_dep, json_c_dep]

install_headers('include/tgbot.h')

tgbot_lib = library(
    'tgbot',
    sources,
    dependencies: deps,
    include_directories: inc_dir,
    install: true,
)

# Example (windows)
executable(
    'example',
    'examples/echobot/echobot.c',
    dependencies: deps,
    link_with: tgbot_lib,
    include_directories: inc_dir,
)