footswitch_srcs = [
  'footswitch.c',
  'module.c',
  'common.c',
  'debug.c',
]
inc = get_variable('inc', join_paths(meson.current_source_dir(),'..'))
footswitch_dirs = [
  include_directories('.'),
  inc,
]
footswitch_deps = [
  c_vector_dep,
  hidapi_dep,
  c_fsio_dep,
  c_stringfn_dep,
  ansi_codes_dep,
  str_flatten_c_dep,
]
footswitch_libs = []
footswitch_c_args = [
]
footswitch_link_args = [
]

footswitch_dir = join_paths(meson.current_source_dir(), '..')
footswitch_lib = static_library('footswitch',
  footswitch_srcs,
  install: false,
  dependencies: footswitch_deps,
  include_directories: footswitch_dirs,
)
footswitch_dep = declare_dependency(
  include_directories: footswitch_dirs,
  link_with: [footswitch_lib],
  link_args: footswitch_link_args,
)
