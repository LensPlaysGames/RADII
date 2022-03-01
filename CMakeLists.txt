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
    src/EFI/simple_text_protocol.h
    src/EFI/system_table.h
    src/EFI/types.h
)

set(
SOURCES
    src/main.c
)

add_executable(Bootloader ${SOURCES} ${HEADERS})
set_target_properties(Bootloader PROPERTIES OUTPUT_NAME main.efi)
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
)
target_link_options(
Bootloader PUBLIC
    -nostdlib
    -Wl,-dll
    -shared
    -Wl,--subsystem,10
    -e efi_main
)

add_custom_target(EnsureBinDirectory ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin)
