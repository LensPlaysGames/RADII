#include "common.h"

#include "boot_information.h"
#include <efi.h>

EFI_SYSTEM_TABLE *SystemTable;
EFI_HANDLE *ImageHandle;

MemoryInformation MemoryInfo;

EFI_STATUS Initialize(EFI_SYSTEM_TABLE *ST, EFI_HANDLE *IH) {
  SystemTable = ST;
  ImageHandle = IH;

  SystemTable->BootServices->GetMemoryMap(&MemoryInfo.MapSizeInBytes, MemoryInfo.Map, &MemoryInfo.MapKey, &MemoryInfo.DescriptorSize, &MemoryInfo.DescriptorVersion);
  SystemTable->BootServices->AllocatePool(LOADER_DATA, MemoryInfo.MapSizeInBytes, (void**)&MemoryInfo.Map);
  SystemTable->BootServices->GetMemoryMap(&MemoryInfo.MapSizeInBytes, MemoryInfo.Map, &MemoryInfo.MapKey, &MemoryInfo.DescriptorSize, &MemoryInfo.DescriptorVersion);

  return EFI_SUCCESS;
}
