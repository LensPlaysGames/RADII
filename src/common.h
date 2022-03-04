#ifndef RADII_BOOTLOADER_COMMON_H
#define RADII_BOOTLOADER_COMMON_H

#include "boot_information.h"
#include <efi.h>

extern EFI_SYSTEM_TABLE *SystemTable;
extern EFI_HANDLE *ImageHandle;

extern MemoryInformation MemoryInfo;

EFI_STATUS Initialize(EFI_SYSTEM_TABLE *ST, EFI_HANDLE *IH);

#endif /* RADII_BOOTLOADER_COMMON_H */
