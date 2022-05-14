project( 'ExampleCKernel'
       , 'c'
       , version : '001'
       , meson_version : '>=0.60.0'
       )

bootloader_compiler = find_program(
  'x86_64-w64-mingw32-gcc'
, required : true
)
bootloader_generator = generator(
  bootloader_compiler
, arguments : [ '-ffreestanding'
              , '-fno-stack-protector'
              , '-fshort-wchar'
              , '-mno-red-zone'
              , '-nostdlib'
              , '-static'
              , '-Wl,--subsystem,10'
              , '-e'
              , 'efi_main'
              , '@EXTRA_ARGS@'
              , '-MD'
              , '-MQ'
              , '@OUTPUT@'
              , '-MF'
              , '@OUTPUT@.d'
              , '-o'
              , '@OUTPUT@'
              , '@INPUT@'
              ]
, capture : true
, output : ['@PLAINNAME@.obj']
)

bootloader_src = bootloader_generator.process(
  [ '../src/common.c'
  , '../src/file_operations.c'
  , '../src/loader.c'
  , '../src/main.c'
  , '../src/simple_print.c'
  , '../src/resource_table.c'
  , '../src/resource_table_header.c'
  ]
, extra_args :
    [ '-I../../src'
    , '-I../../src/EFI'
    ])
bootloader_exe = executable(
  'bootloader'
, sources : bootloader_src
)

executable_output_directory = meson.current_source_dir()
executable_output_path = executable_output_directory / 'kernel.elf'

kernel_c_args = [ '-ffreestanding'
                , '-mno-red-zone'
                , '-Wall'
                , '-Wextra'
                , '-Werror'
                ]

kernel_link_script = meson.current_source_dir() / 'kernel.ld'
kernel_link_args = [ '-T'
                   , kernel_link_script
                   , '-static'
                   , '-Bsymbolic'
                   , '-nostdlib'
                   ]

kernel_elf = executable(
  meson.project_name()
, sources : ['kernel.c']
, c_args : kernel_c_args
, link_args : kernel_link_args
)
