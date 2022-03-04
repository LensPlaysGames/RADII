cmake_minimum_required(VERSION 3.14)
project(RadiiBootloader VERSION 0.0.1 LANGUAGES C)

# Use cross compiler.
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)

# Export compilation database in JSON format.
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Output files to `bin/` directory (created by `EnsureBinDirectory` target).
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

set(
HEADERS
    src/common.h
    src/EFI/file_protocol.h
    src/EFI/graphics_output_protocol.h
    src/EFI/loaded_image_protocol.h
    src/EFI/simple_file_system_protocol.h
    src/EFI/simple_text_protocol.h
    src/EFI/tables.h
    src/EFI/types.h
    src/file_operations.h
    src/loader.h
)

set(
SOURCES
    src/common.c
    src/file_operations.c
    src/loader.c
    src/main.c
)

add_executable(Bootloader ${SOURCES} ${HEADERS})
set_target_properties(Bootloader PROPERTIES OUTPUT_NAME main.efi)

# Command line options passed to `gcc`
target_compile_options(
Bootloader PUBLIC
    -ffreestanding
    -fno-stack-protector
    -fpic
    -fshort-wchar
    -mno-red-zone
    -Wall
    -Wextra
    -Werror
    -I${CMAKE_SOURCE_DIR}/src/EFI
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
add_custom_target(EnsureBinDirectory ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin)

# Native compilers on Windows always add the `.exe` extension; this automatically removes it.
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
add_custom_command(TARGET Bootloader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rename
        ${CMAKE_SOURCE_DIR}/bin/main.efi.exe
        ${CMAKE_SOURCE_DIR}/bin/main.efi
)
endif()
