footswitch_test_srcs = [
  'footswitch-test.c',
]
inc = get_variable('inc', join_paths(meson.current_source_dir(),'..'))
footswitch_test_include_dirs = [
  '.',
  inc,
]
footswitch_test_deps = [
  c_greatest_dep,
  c_vector_dep,
  c_fsio_dep,
  c_stringfn_dep,
  footswitch_dep,
  ansi_codes_dep,
  #debug_memory_dep,
]
footswitch_test_c_args = [
]
footswitch_test_link_args = [
]

footswitch_test_test_dir = join_paths(meson.current_source_dir(), '..')

if get_option('enable-binaries')
  footswitch_test_exec = executable('footswitch-test',
     footswitch_test_srcs,
     dependencies: footswitch_test_deps,
     include_directories: footswitch_test_include_dirs,
     link_args: footswitch_test_link_args,
     c_args: footswitch_test_c_args,
     install: false,
  )
  test('footswitch-test', 
    footswitch_test_exec, args: ['-v','-a'],
    workdir: footswitch_test_test_dir,
  )
endif
