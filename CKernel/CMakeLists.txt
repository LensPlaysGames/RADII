cmake_minimum_required(VERSION 3.14)
project(RadiiBootloader VERSION 0.0.1 LANGUAGES C)

# Use cross compiler to generate 64-bit ELF executables.
# This is necessary because our bootloader explicity loads executables
#   of this format, so it's handy to be able to produce them ;^).
set(CMAKE_C_COMPILER x86_64-linux-gnu-gcc)

# Output files to current directory.
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

set(
  HEADERS
  # Headers would go here, IF I HAD ANY.
)

set(
  SOURCES
  kernel.c
)

add_executable(Kernel ${SOURCES} ${HEADERS})
set_target_properties(Kernel PROPERTIES OUTPUT_NAME kernel.elf)
target_compile_options(
  Kernel PUBLIC
  -ffreestanding
  -mno-red-zone
  -Wall
  -Wextra
  -Werror
)
target_link_options(
  Kernel PUBLIC
  -T ${CMAKE_SOURCE_DIR}/kernel.ld
  -static
  -Bsymbolic
  -nostdlib
)
