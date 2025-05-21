project('tgbot', 'c', version: '0.1')

sources = []
subdir('src')

inc_dir = include_directories('include')

curl_dep = dependency('libcurl')
json_c_dep = dependency('json-c')

deps = [curl_dep, json_c_dep]

install_headers('include/tgbot.h')
#install_subdir('.', install_dir: 'C:/tgbot')
lib = library(
    'tgbot',
    sources,
    dependencies: deps,
    include_directories: inc_dir,
    install: true,
)

# Example (windows)
#lib_inc_dir = include_directories('C:/include')
executable(
    'example',
    'examples/inlinekeyboard/inlinekeyboard.c',
    dependencies: deps,
    link_with: lib,
    include_directories: inc_dir,
)