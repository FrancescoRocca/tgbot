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

install_headers(
    [
        'include/methods.h',
        'include/tg_types.h',
        'include/tgbot.h',
        'include/types.h',
    ],
    subdir: 'tgbot',
)

tgbot_lib = static_library(
    'tgbot',
    sources,
    dependencies: deps,
    include_directories: inc_dir,
    install: true,
)

tgbot_dep = declare_dependency(
    link_with: tgbot_lib,
    include_directories: inc_dir,
    dependencies: deps,
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

# Example
example = executable(
    'example',
    'examples/inlinekeyboard/inlinekeyboard.c',
    dependencies: tgbot_dep,
    build_by_default: false,
)

test('example_inlinekeyboard', example)