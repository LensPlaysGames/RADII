#ifndef RADII_BOOTLOADER_SIMPLE_PRINT_H
#define RADII_BOOTLOADER_SIMPLE_PRINT_H

#include "common.h"
#include <types.h>

void Print(CHAR16* str) {
  SystemTable->ConsoleOut->OutputString(SystemTable->ConsoleOut, str);
}

#endif /* RADII_BOOTLOADER_SIMPLE_PRINT_H */
