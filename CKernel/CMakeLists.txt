cmake_minimum_required(VERSION 3.14)

# Use cross compiler to generate 64-bit ELF executables.
# This is necessary because our bootloader explicity loads executables
#   of this format, so it's handy to be able to produce them ;^).
set(CMAKE_C_COMPILER x86_64-linux-gnu-gcc)

# Output files to current directory.
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})

project(RadiiBootloaderExampleKernel VERSION 0.0.1 LANGUAGES C)

add_executable(Kernel)
target_sources(
  Kernel
  PRIVATE
  kernel.c
)
set_target_properties(
  Kernel
  PROPERTIES OUTPUT_NAME
  kernel.elf
)
target_compile_options(
  Kernel PRIVATE
  -ffreestanding
  -mno-red-zone
  -Wall
  -Wextra
  -Werror
)
target_link_options(
  Kernel PRIVATE
  -T ${CMAKE_CURRENT_LIST_DIR}/kernel.ld
  -static
  -Bsymbolic
  -nostdlib
)

# Build bootloader as part of the build system.
# Yes, it's odd that a subdirectory is the parent, let's not think about it too hard.
add_subdirectory(
  "${CMAKE_CURRENT_LIST_DIR}/.."
  "bootloader"
)
add_dependencies(Kernel Bootloader)


# Install kernel executable in UEFI-compatible boot image.
#  +--------------------+
#  |    image_dir_t     |
#  |  Gen. Boot Media   |
#  +----------+---------\
#             |          --\
#             |             ---\
#             |     +----------------------+
#             |     |    image_dir_ext     |
#             |     |  Install the kernel  |
#             |     +----------------------+
#             |             --/
#             |          --/
#  +----------+---------/
#  |     image_dir      |
#  |   Called by user   |
#  +--------------------+
add_custom_target(
  image_dir_ext
  COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_CURRENT_LIST_DIR}/kernel.elf" "${BOOT_IMAGE_DIR}/"
  COMMENT "Installing kernel into boot directory at ${BOOT_IMAGE_DIR}"
  VERBATIM USES_TERMINAL
)
add_dependencies(image_dir_ext image_dir_t)
add_dependencies(image_dir image_dir_ext)

if(TARGET image_raw)
  add_custom_target(
    image_raw_ext
    COMMAND mcopy
    -i ${BOOT_IMAGE_IMG}
    ${EXECUTABLE_OUTPUT_PATH}/kernel.elf
    ::
    DEPENDS image_raw_t
    COMMENT "Installing kernel onto boot image at ${BOOT_IMAGE_IMG}"
    VERBATIM USES_TERMINAL
  )
  add_dependencies(image_raw_ext image_raw_t)
  add_dependencies(image_raw image_raw_ext)
endif()
