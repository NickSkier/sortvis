project('sortvis', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3',
                     'cpp_std=c++17'])

sortvis_deps = [
  dependency('ncurses')
]

exe = executable('sortvis', 'sortvis.cpp',
  dependencies: sortvis_deps,
  install : true,
)

test('basic', exe)
