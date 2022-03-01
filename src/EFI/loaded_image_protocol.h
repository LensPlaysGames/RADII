#ifndef RADII_BOOTLOADER_LOADED_IMAGE_PROTOCOL_H
#define RADII_BOOTLOADER_LOADED_IMAGE_PROTOCOL_H

#include "device_path_protocol.h"
#include "system_table.h"
#include "types.h"

#define EFI_LOADED_IMAGE_PROTOCOL_GUID \
  {0x5b1b31a1,0x9562,0x11d2, {0x8e,0x3f,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000

#define EFI_LOADED_IMAGE_DEVICE_PATH_PROTOCOL_GUID \
  {0xbc62157e,0x3e33,0x4fec, {0x99,0x20,0x2d,0x3b,0x36,0xd7,0x50,0xdf}}

typedef struct {
  UINT32 Revision;
  EFI_HANDLE ParentHandle;
  EFI_SYSTEM_TABLE *SystemTable;

  // Source location of the image
  EFI_HANDLE DeviceHandle;
  EFI_DEVICE_PATH_PROTOCOL *FilePath;
  VOID *Reserved;

  // Image's load options
  UINT32 LoadOptionsSize;
  VOID *LoadOptions;

  // Location where image was loaded
  VOID *ImageBase;
  UINT64 ImageSize;
  EFI_MEMORY_TYPE ImageCodeType;
  EFI_MEMORY_TYPE ImageDataType;

  EFI_STATUS (*Unload)(EFI_HANDLE ImageHandle);
} EFI_LOADED_IMAGE_PROTOCOL;

#endif
