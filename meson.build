project('tgbot', 'c')
cc = meson.get_compiler('c')

sources = []
subdir('src')

inc_dir = include_directories('include')

deps = []
include_dirs = [inc_dir]

curl_dep = dependency('libcurl')
deps += curl_dep

json_c_dep = dependency('json-c')
deps += json_c_dep

executable('tgbot', sources, include_directories: [include_dirs], dependencies: deps)
