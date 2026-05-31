project(
    'tgbot',
    'c',
    version: '0.1',
    default_options: ['warning_level=3', 'c_std=c18'],
)

sources = []
headers = []
subdir('src')
subdir('include')

inc_dir = include_directories('include')

curl_dep = dependency('libcurl')
json_c_dep = dependency('json-c')

deps = [curl_dep, json_c_dep]

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

cppcheck = find_program('cppcheck', required: false)
if cppcheck.found()
    run_target(
        'cppcheck',
        command: [
            cppcheck,
            '--enable=all',
            '--inconclusive',
            '--std=c18',
            '--check-level=exhaustive',
            '--suppress=missingIncludeSystem',
            '--project='
            + join_paths(meson.current_build_dir(), 'compile_commands.json'),
        ],
    )
endif

clangformat = find_program('clang-format', required: false)
if clangformat.found()
    run_target('format', command: [clangformat, '-i'] + sources + headers)
endif

# Example
executable(
    'example',
    'examples/sender/sendpic.c',
    build_by_default: false,
    link_with: tgbot_lib,
    include_directories: inc_dir,
)
