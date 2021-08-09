project(
  'pd-sndfilter',
  'c',
  default_options: [
    # removes the -Wl,--no-undefined flag
    'b_lundef=false',
    'default_library=static', # linking sndfilter_dep statically
  ]
)

sndfilter_proj = subproject('sndfilter')
sndfilter_dep = subproject('sndfilter').get_variable('sndfilter_dep')

# for including m_pd.h
pd = dependency('pd')

pd_sndfilter_lib = shared_library(
  'sndfilter',
  # include_directories: include_directories('include'),
  name_prefix: '',
  name_suffix: 'pd_linux',
  sources: files(
    'src/sndfilter.c',
  ),
  dependencies: [
    sndfilter_dep,
    pd,
  ],
  # some args below not sure if actually needed!
  # just copied from the Makefile template (see http://puredata.info/docs/developer/MakefileTemplate)
  c_args: [
    '-DPD',
    '-fPIC',
    ],
  link_args: [
    '-rdynamic',
    '-Wl,-rpath,$ORIGIN',
    '-Wl,-R,"\$ORIGIN"',
    '--enable-new-dtags',
    '-lc',
    ]
)
