#ifndef RADII_BOOTLOADER_COMMON_H
#define RADII_BOOTLOADER_COMMON_H

#include <boot_information.h>
#include <EFI/efi.h>

extern BootInformation BootInfo;

extern EFI_SYSTEM_TABLE *SystemTable;
extern EFI_HANDLE *ImageHandle;
extern EFI_BOOT_SERVICES *BootServices;

extern MemoryInformation MemoryInfo;

EFI_STATUS Initialize(EFI_SYSTEM_TABLE *ST, EFI_HANDLE *IH);

extern CHAR16 *EFI_ERROR_STRINGS[];

#endif /* RADII_BOOTLOADER_COMMON_H */
