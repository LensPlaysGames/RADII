cmake_minimum_required(VERSION 3.14)
project(RadiiBootloader VERSION 0.0.1 LANGUAGES C)

# Architecture to build for (only relevant for running virtual machine)
set(
  ARCH "x86_64"
  CACHE STRING
  "The CPU architecture that RADII will run on when booting into a virtual machine"
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

# Output files to `bin/` directory (created by `EnsureBinDirectory` target).
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(
SOURCES
    src/common.c
    src/file_operations.c
    src/loader.c
    src/main.c
    src/simple_print.c
    src/resource_table.c
)

add_executable(Bootloader ${SOURCES})
set_target_properties(Bootloader PROPERTIES OUTPUT_NAME main.efi)
target_include_directories(Bootloader PRIVATE src/)

# Command line options passed to `gcc`
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

# Command line options passed to `ld`
target_link_options(
Bootloader PUBLIC
    -nostdlib
    -shared
    -Wl,--subsystem,10
    -e efi_main
)

# This ensures that the bin directory is created before the `Bootloader` target tries to use it.
add_custom_target(EnsureBinDirectory ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_LIST_DIR}/bin)

# Native compilers on Windows always add the `.exe` extension; this automatically removes it.
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  add_custom_command(TARGET Bootloader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
    ${EXECUTABLE_OUTPUT_PATH}/main.efi.exe
    ${EXECUTABLE_OUTPUT_PATH}/main.efi
  )
endif()


set(BOOT_IMAGE_DIR "${CMAKE_CURRENT_LIST_DIR}/boot")

# Boot media generation: directory.
add_custom_target(
  image_dir
  COMMAND ${CMAKE_COMMAND} -E make_directory "${BOOT_IMAGE_DIR}/EFI/BOOT"
  COMMAND ${CMAKE_COMMAND} -E copy
  "${EXECUTABLE_OUTPUT_PATH}/main.efi" "${BOOT_IMAGE_DIR}/EFI/BOOT/"
  COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_CURRENT_LIST_DIR}/startup.nsh" "${BOOT_IMAGE_DIR}"
  COMMENT "Generating directory with UEFI-compatible boot layout"
  DEPENDS
  ${CMAKE_CURRENT_LIST_DIR}/startup.nsh
  USES_TERMINAL
  VERBATIM
)
# Image generation relies on main.efi
add_dependencies(image_dir Bootloader)

# Boot media generation: raw FAT32 image.
find_program(DD_PROGRAM dd)
find_program(MTOOLS_PROGRAM mtools)
if(DD_PROGRAM AND MTOOLS_PROGRAM)
  message(
    VERBOSE
    "Found dd and mtools, creating target: image_raw"
  )
  add_custom_target(
    image_raw
    COMMAND ${CMAKE_COMMAND} -E make_directory ${EXECUTABLE_OUTPUT_PATH}
    COMMAND ${DD_PROGRAM} if=/dev/zero of=${EXECUTABLE_OUTPUT_PATH}/test-build.img count=93750
    COMMAND mformat -i ${EXECUTABLE_OUTPUT_PATH}/test-build.img -F -v "EFI System" ::
    COMMAND mmd -i ${EXECUTABLE_OUTPUT_PATH}/test-build.img ::/EFI
    COMMAND mmd -i ${EXECUTABLE_OUTPUT_PATH}/test-build.img ::/EFI/BOOT
    COMMAND mcopy -i ${EXECUTABLE_OUTPUT_PATH}/test-build.img ${EXECUTABLE_OUTPUT_PATH}/main.efi ::/EFI/BOOT
    COMMAND mcopy -i ${EXECUTABLE_OUTPUT_PATH}/test-build.img ${CMAKE_CURRENT_LIST_DIR}/startup.nsh ::
    COMMENT "Generating UEFI-compatible FAT32 boot media"
    DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/startup.nsh
    USES_TERMINAL
    VERBATIM
  )
  # Image generation relies on main.efi
  add_dependencies(image_raw Bootloader)
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
  set(OVMF_CODE "${CMAKE_CURRENT_LIST_DIR}/OVMFbin/OVMF_CODE-pure-efi.fd")
  set(OVMF_VARS "${CMAKE_CURRENT_LIST_DIR}/OVMFbin/OVMF_VARS_RADII.fd")
  list(
    APPEND QEMU_FLAGS
    -drive if=pflash,format=raw,unit=0,file=${OVMF_CODE},readonly=on
    -drive if=pflash,format=raw,unit=1,file=${OVMF_VARS}
  )
  file(
    COPY_FILE
    "${CMAKE_CURRENT_LIST_DIR}/OVMFbin/OVMF_VARS-pure-efi.fd"
    "${OVMF_VARS}"
  )
  add_custom_target(
    run_qemu_nodeps
    COMMAND ${QEMU_PROGRAM} ${QEMU_FLAGS}
    -hdb fat:floppy:rw:${BOOT_IMAGE_DIR}
    COMMENT "Running QEMU for ${ARCH} from directory treated as FAT32 floppy"
    USES_TERMINAL
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    DEPENDS
    ${CMAKE_CURRENT_LIST_DIR}/startup.nsh
    VERBATIM
  )
  add_custom_target(run_qemu)
  add_custom_target(
    runimg_qemu_nodeps
    COMMAND ${QEMU_PROGRAM} ${QEMU_FLAGS}
    -drive format=raw,file=${EXECUTABLE_OUTPUT_PATH}/test-build.img
    COMMENT "Running QEMU for ${ARCH} from FAT32 `test-build.img`"
    USES_TERMINAL
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    VERBATIM
  )
  add_custom_target(runimg_qemu)
  add_dependencies(run_qemu image_dir run_qemu_nodeps)
  if(TARGET image_raw)
    # QEMU boots from the image this target generates.
    add_dependencies(runimg_qemu image_raw)
  endif()
  add_dependencies(runimg_qemu image_raw runimg_qemu_nodeps)
endif()
