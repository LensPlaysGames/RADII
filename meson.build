project( 'RADII'
       , 'c'
       , version : '001'
       , meson_version : '>=0.60.0'
       )

executable_output_directory = meson.current_source_dir() / 'bin'
executable_output_path = executable_output_directory / get_option('bootloader_executable_name')

python = find_program(
  [ 'python3'
  , 'usr/bin/python3'
  , 'python'
  , 'usr/bin/python'
  ],
  required : true
)
python = python.full_path()

# Create bin directory to store output executables.
run_command(
  python
, '-c'
, 'import os;os.makedirs("'
  + executable_output_directory
  + '",exist_ok=True)'
, capture : true
, check : true
)

# Generate resource_table.[c|h] if they don't already exist
generation_directory = meson.current_source_dir() / 'gen'
radii_cfg_fullpath = generation_directory / get_option('radii_cfg')
run_command(
  python
, generation_directory / 'generate_resource_table.py'
, '--config'
, radii_cfg_fullpath
, capture : true
, check : true
)
bootloader_src_dir = meson.current_source_dir() / 'src'
run_command(
  'cp'
, '-t'
, bootloader_src_dir
, generation_directory / 'resource_table.h'
, generation_directory / 'resource_table.c'
, capture : true
, check : true
)

bootloader_src = [ 'src/common.c'
                 , 'src/file_operations.c'
                 , 'src/loader.c'
                 , 'src/main.c'
                 , 'src/simple_print.c'
                 , 'src/resource_table.c'
                 , 'src/resource_table_header.c'
                 ]

bootloader_inc = include_directories('src')

bootloader_c_args = [ '-ffreestanding'
                    , '-fno-stack-protector'
                    , '-fshort-wchar'
                    , '-mno-red-zone'
                    ]

bootloader_link_args = [ '-nostdlib'
                       , '-static'
                       , '-e', 'efi_main'
                       ]

bootloader_exe = executable(
  meson.project_name(),
  sources : bootloader_src,
  c_args : bootloader_c_args,
  link_args : bootloader_link_args,
  include_directories : bootloader_inc,
  win_subsystem : 'efi_application'
)

rename_bootloader_exe_t = custom_target(
  get_option('bootloader_executable_name'),
  depends : bootloader_exe,
  input : bootloader_exe,
  output : 'bootloader_executable',
  command : ['mv'
            , meson.current_build_dir() / meson.project_name() + '.exe'
            , executable_output_path
            ],
  build_by_default : true
)

# The fact that you can't run multiple commands
# in a single target is quite the limitation.
boot_image_dir = executable_output_directory / 'boot'
image_dir_step0_t = custom_target(
  'image-dir-step0',
  output : 'image_directory_step0',
  command : [python
            , '-c'
            , 'import os;os.makedirs("'
              + boot_image_dir / 'EFI/BOOT/'
              + '",exist_ok=True)'
            ],
  depends : [rename_bootloader_exe_t]
)
image_dir_step1_t = custom_target(
  'image-dir-step1',
  output : 'image_directory_step1',
  command : ['cp'
            , '-t'
            , boot_image_dir / 'EFI/BOOT'
            , executable_output_path
            ],
  depends : [image_dir_step0_t]
)
image_dir_laststep_t = custom_target(
  'image-dir-step2',
  input : meson.current_source_dir() / 'startup.nsh',
  output : 'image_directory_step2',
  command : ['cp'
            , '-t'
            , boot_image_dir
            , meson.current_source_dir() / 'startup.nsh'
            ],
  depends : [image_dir_step1_t]
)
# This uses python as a dummy command because meson
# requires a command section for all targets.
image_dir_t = custom_target(
  'image-dir',
  output : 'image_directory',
  command : [python, '-c', '0'],
  depends : [image_dir_laststep_t]
)

qemu = find_program('qemu-system-x86_64', required : false)
if (qemu.found())
  # Ensure OVMF firmware for QEMU.
  ovmf_bin = meson.current_source_dir() / 'OVMFbin'
  ovmf_pure_vars = ovmf_bin / 'OVMF_VARS-pure-efi.fd'
  ovmf_vars = ovmf_bin / 'OVMF_VARS_RADII.fd'
  ovmf_code = ovmf_bin / 'OVMF_CODE-pure-efi.fd'
  run_command('cp', ovmf_pure_vars, ovmf_vars, check : true)
  # Flags shared by all QEMU targets.
  qemu_flags = [ '-machine', 'q35'
                # cpu: Broadwell, Cascadelake-Server, Cooperlake, Conroe, core2duo,
                #      Denverton, Dhyana, EPYC, Haswell, IvyBridge, kvm64, max,
                #      Nehalem, Penryn, qemu64, SandyBridge, Skylake-[Client|Server],
                #      Snowridge, Westmere
                , '-cpu', 'qemu64'
                # Multiple processors (commented until utilized).
                #-smp 8,sockets=2,cores=2,threads=2,maxcpus=8
                # One hundred megabytes of RAM.
                , '-m', '100M'
                # Use local time as real time clock base.
                , '-rtc', 'base=localtime,clock=host,driftfix=none'
                # This is deprecated, but we'll use it,
                # until we write an actual sound driver.
                , '-soundhw', 'pcspk'
                # No networking drivers yet.
                , '-net', 'none'
                # Show extra information regarding triple faults.
                , '-d', 'cpu_reset'
                # Use stdio as serial input and output.
                # This allows debug messages to reach the terminal.
                , '-serial', 'stdio'
                , '-drive', 'if=pflash,format=raw,unit=0,file='
                            + ovmf_code
                            + ',readonly=on'
                , '-drive', 'if=pflash,format=raw,unit=1,file='
                            + ovmf_vars
               ]
  run_target(
    'qemu-dir',
    command : [qemu, qemu_flags, '-hdb', 'fat:floppy:rw:' + boot_image_dir],
    depends : [image_dir_t]
  )
endif
