#ifndef RADII_BOOTLOADER_BOOT_INFORMATION_H
#define RADII_BOOTLOADER_BOOT_INFORMATION_H

#include <EFI/types.h>
#include <resource_table_header.h>

// Pre-defined headers for resource table.

/// (U)EFI Memory Map
#define ResourceTableHeaderSignature_MemoryMap (unsigned char*)"MEMORYMAPMEMORY"
typedef struct memory_information_t {
  ResourceTableHeader Header;
  EFI_MEMORY_DESCRIPTOR *Map;
  UINTN MapKey;
  UINTN MapSizeInBytes;
  UINTN BytesPerMemoryDescriptor;
  UINT32 DescriptorVersion;
} MemoryInformation;

typedef struct linear_pixel_framebuffer_t {
  ResourceTableHeader Header;
  void* BaseAddress;
  UINT64 BufferSize;
  UINTN PixelWidth;
  UINTN PixelHeight;
  UINTN PixelsPerScanLine;
} LinearPixelFramebuffer;

typedef struct graphical_information_t {
  ResourceTableHeader Header;
  LinearPixelFramebuffer Framebuffer;
} GraphicalInformation;

#endif /* RADII_BOOTLOADER_BOOT_INFORMATION_H */
