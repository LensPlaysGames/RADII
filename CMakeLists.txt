cmake_minimum_required(VERSION 3.14)
project(RadiiBootloader VERSION 0.0.1 LANGUAGES C)

# Architecture to build for (only relevant for running virtual machine)
set(
  ARCH "x86_64"
  CACHE STRING
  "The CPU architecture that RADII will run on when booting into a virtual machine"
)

set(
  RADII_CONFIGURATION_FILE "${CMAKE_CURRENT_LIST_DIR}/gen/table.json"
  CACHE PATH
  "The confiugration file that determines the layout of the resource table, among other things."
)

set(
  OVMF_CODE "${CMAKE_CURRENT_LIST_DIR}/OVMFbin/OVMF_CODE-pure-efi.fd"
  CACHE PATH
  "Path to UEFI firmware ROM (code section)."
)

set(
  OVMF_PURE_VARS
  "${CMAKE_CURRENT_LIST_DIR}/OVMFbin/OVMF_VARS-pure-efi.fd"
  CACHE PATH
  "Path to UEFI firmware ROM (immutable variables section)."
)

set(
  OVMF_VARS
  "${CMAKE_CURRENT_LIST_DIR}/OVMFbin/OVMF_VARS_RADII.fd"
  CACHE PATH
  "Path to UEFI firmware ROM (mutable variables section)."
)

option(
  QEMU_DEBUG
  "Start QEMU with `-S -s` flags to wait for a debugger to attach before beginning execution."
  OFF
)

# Use cross compiler.
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)

# Export compilation database in JSON format.
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

set(
  BOOTLOADER_EXECUTABLE_NAME
  "main.efi"
  CACHE STRING
  "The name of the bootloader executable"
)
set(
  BOOTLOADER_EXECUTABLE_DIR
  "${CMAKE_CURRENT_LIST_DIR}/bin"
)
set(
  BOOTLOADER_EXECUTABLE_PATH
  "${BOOTLOADER_EXECUTABLE_DIR}/${BOOTLOADER_EXECUTABLE_NAME}"
)
# Output executable file to `bin/` directory (created by `EnsureBinDirectory` target).
set(EXECUTABLE_OUTPUT_PATH ${BOOTLOADER_EXECUTABLE_DIR})

add_executable(Bootloader)

# Generate resource_table.[c|h] if they don't already exist
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_LIST_DIR}/src/resource_table.c"
  COMMAND
  ${CMAKE_COMMAND}
  -DCONFIG_FILE=${RADII_CONFIGURATION_FILE}
  -P "${CMAKE_CURRENT_LIST_DIR}/generate_resource_table.cmake"
  COMMENT "-- Ensuring resource table exists"
  USES_TERMINAL VERBATIM
)

target_sources(
  Bootloader
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/common.c
  ${CMAKE_CURRENT_LIST_DIR}/src/file_operations.c
  ${CMAKE_CURRENT_LIST_DIR}/src/loader.c
  ${CMAKE_CURRENT_LIST_DIR}/src/main.c
  ${CMAKE_CURRENT_LIST_DIR}/src/simple_print.c
  ${CMAKE_CURRENT_LIST_DIR}/src/resource_table.c
)
target_include_directories(
  Bootloader
  PRIVATE
  src/
)
set_target_properties(
  Bootloader
  PROPERTIES OUTPUT_NAME
  ${BOOTLOADER_EXECUTABLE_NAME}
)
target_compile_options(
  Bootloader PUBLIC
  -ffreestanding
  -fno-stack-protector
  -fshort-wchar
  -mno-red-zone
  -Wall
  -Wextra
  -Werror
)
target_link_options(
  Bootloader PUBLIC
  -nostdlib
  -shared
  -Wl,--subsystem,10
  -e efi_main
)

# This ensures that the bin directory is created before the `Bootloader` target tries to use it.
add_custom_target(
  EnsureBinDirectory
  COMMAND ${CMAKE_COMMAND} -E make_directory
  ${BOOTLOADER_EXECUTABLE_DIR}
)
add_dependencies(Bootloader EnsureBinDirectory)

# Native compilers on Windows always add the `.exe` extension; this automatically removes it.
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  add_custom_command(
	TARGET Bootloader
	POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
    ${BOOTLOADER_EXECUTABLE_PATH}.exe
    ${BOOTLOADER_EXECUTABLE_PATH}
  )
endif()


# Boot media generation: directory.
set(
  QEMU_BOOT_DIR
  "${CMAKE_CURRENT_LIST_DIR}/boot"
  CACHE
  PATH
  "The path to the UEFI-compatible boot directory.
  `image_dir` generates the directory at this path.
  `run_qemu` boots from the directory at this path."
)
add_custom_target(
  image_dir_t
  COMMAND ${CMAKE_COMMAND} -E make_directory "${QEMU_BOOT_DIR}/EFI/BOOT/"
  COMMAND ${CMAKE_COMMAND} -E copy
  ${BOOTLOADER_EXECUTABLE_PATH} "${QEMU_BOOT_DIR}/EFI/BOOT/"
  COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_CURRENT_LIST_DIR}/startup.nsh" "${QEMU_BOOT_DIR}/"
  DEPENDS
  ${CMAKE_CURRENT_LIST_DIR}/startup.nsh
  BYPRODUCTS
  "${QEMU_BOOT_DIR}/EFI/BOOT/${BOOTLOADER_EXECUTABLE_NAME}"
  "${QEMU_BOOT_DIR}/startup.nsh"
  COMMENT "Populating directory with UEFI-compatible boot layout"
  VERBATIM USES_TERMINAL
)
add_dependencies(image_dir_t Bootloader)
add_custom_target(image_dir DEPENDS image_dir_t)

# Boot media generation: raw FAT32 image.
set(
  QEMU_BOOT_IMG "${BOOTLOADER_EXECUTABLE_DIR}/test-build.img"
  CACHE PATH
  "The path to the UEFI-compatible FAT32 boot media image.
  `image_raw` generates the image at this path.
  `runimg_qemu` boots from the image at this path."
)
find_program(DD_PROGRAM dd)
find_program(MTOOLS_PROGRAM mtools)
if(DD_PROGRAM AND MTOOLS_PROGRAM)
  message(
    VERBOSE
    "Found dd and mtools, creating target: image_raw"
  )
  add_custom_target(
    image_raw_t
    COMMAND ${CMAKE_COMMAND} -E make_directory ${BOOTLOADER_EXECUTABLE_DIR}
    COMMAND ${DD_PROGRAM} if=/dev/zero of=${QEMU_BOOT_IMG} count=93750
    COMMAND mformat -i ${QEMU_BOOT_IMG} -F -v "EFI System" ::
    COMMAND mmd -i ${QEMU_BOOT_IMG} ::/EFI
    COMMAND mmd -i ${QEMU_BOOT_IMG} ::/EFI/BOOT
    COMMAND mcopy -i ${QEMU_BOOT_IMG} ${BOOTLOADER_EXECUTABLE_PATH} ::/EFI/BOOT
    COMMAND mcopy -i ${QEMU_BOOT_IMG} ${CMAKE_CURRENT_LIST_DIR}/startup.nsh ::
    COMMENT "Generating UEFI-compatible FAT32 boot media"
    DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/startup.nsh
    VERBATIM USES_TERMINAL
  )
  # Image generation relies on bootloader executable.
  add_dependencies(image_raw_t Bootloader)
  add_custom_target(image_raw DEPENDS image_raw_t)
else()
  message(
    "-- dd and mtools not found on system, skipping image_raw target generation"
  )
endif()

# Add a custom target to run QEMU with the proper flags
# if QEMU for ARCH is found in the PATH environment variable.
find_program(QEMU_PROGRAM qemu-system-${ARCH})
if(QEMU_PROGRAM)
  message(VERBOSE "Found QEMU for ${ARCH}, creating target: runimg_qemu")
  set(
    QEMU_FLAGS
    -machine q35
    # cpu: Broadwell, Cascadelake-Server, Cooperlake, Conroe, core2duo,
    #      Denverton, Dhyana, EPYC, Haswell, IvyBridge, kvm64, max,
    #      Nehalem, Penryn, qemu64, SandyBridge, Skylake-[Client|Server],
    #      Snowridge, Westmere
    -cpu qemu64
    # Multiple processors (commented until utilized).
    #-smp 8,sockets=2,cores=2,threads=2,maxcpus=8
    # One hundred megabytes of RAM.
    -m 100M
    # Use local time as real time clock base.
    -rtc base=localtime,clock=host,driftfix=none
    # This is deprecated, but we'll use it,
    # until we write an actual sound driver.
    -soundhw pcspk
    # No networking drivers yet.
    -net none
    # Show extra information regarding triple faults.
    -d cpu_reset
    # Use stdio as serial input and output.
    # This allows debug messages to reach the terminal.
    -serial stdio
  )
  if(QEMU_DEBUG)
    list(APPEND QEMU_FLAGS -S -s)
  endif()
  list(
    APPEND QEMU_FLAGS
    -drive if=pflash,format=raw,unit=0,file=${OVMF_CODE},readonly=on
    -drive if=pflash,format=raw,unit=1,file=${OVMF_VARS}
  )
  file(
    COPY_FILE
    "${OVMF_PURE_VARS}"
    "${OVMF_VARS}"
  )
  add_custom_command(
	OUTPUT run_qemu_nodeps_cmd
    COMMAND ${QEMU_PROGRAM} ${QEMU_FLAGS}
    -hdb fat:floppy:rw:${QEMU_BOOT_DIR}
    COMMENT "Running QEMU for ${ARCH} from directory treated as FAT32 floppy"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    VERBATIM USES_TERMINAL
  )
  add_custom_target(
	run_qemu
	DEPENDS run_qemu_nodeps_cmd
  )
  add_dependencies(run_qemu image_dir)
  add_custom_target(
	run_qemu_nodeps
	DEPENDS run_qemu_nodeps_cmd
  )
  add_custom_command(
	OUTPUT runimg_qemu_nodeps_cmd
	COMMAND ${QEMU_PROGRAM} ${QEMU_FLAGS}
    -drive format=raw,file=${QEMU_BOOT_IMG}
    COMMENT "Running QEMU for ${ARCH} from FAT32 `${QEMU_BOOT_IMG}`"
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    VERBATIM USES_TERMINAL
  )
  add_custom_target(
	runimg_qemu
	DEPENDS runimg_qemu_nodeps_cmd
  )
  if(TARGET image_raw)
	add_dependencies(runimg_qemu image_raw)
	# Create target for launching QEMU without rebuilding boot media.
	add_custom_target(
	  runimg_qemu_nodeps
	  DEPENDS runimg_qemu_nodeps_cmd
	)
  endif()
endif()
