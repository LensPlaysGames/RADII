cmake_minimum_required(VERSION 3.14)

# Use cross compiler to generate 64-bit ELF executables.
# This is necessary because our bootloader explicity loads executables
#   of this format, so it's handy to be able to produce them ;^).
set(CMAKE_C_COMPILER x86_64-linux-gnu-gcc)

# Output files to current directory.
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR})

project(RadiiBootloaderExampleKernel VERSION 0.0.1 LANGUAGES C)

set(
  SOURCES
  kernel.c
)

add_executable(
  Kernel
  ${SOURCES}
)
set_target_properties(Kernel PROPERTIES OUTPUT_NAME kernel.elf)
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
  -T ${CMAKE_SOURCE_DIR}/kernel.ld
  -static
  -Bsymbolic
  -nostdlib
)

# Build bootloader as part of the build system.
# Yes, it's odd that a subdirectory is the parent, let's not think about it too hard.
set(REPO_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
add_subdirectory(${REPO_DIR} "bootloader")
add_dependencies(Kernel Bootloader)


# Install kernel executable in UEFI-compatible boot image.
if(TARGET image_raw)
  add_custom_command(
	OUTPUT ${REPO_DIR}/bin/test-build.img
	POST_BUILD
	COMMAND mcopy
	-i ${REPO_DIR}/bin/test-build.img
	${EXECUTABLE_OUTPUT_PATH}/kernel.elf
	::
	COMMENT "Installing kernel into boot image"
	DEPENDS image_raw
	USES_TERMINAL
	VERBATIM
  )
endif()
