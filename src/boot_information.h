#ifndef RADII_BOOTLOADER_BOOT_INFORMATION_H
#define RADII_BOOTLOADER_BOOT_INFORMATION_H

#include <EFI/types.h>

typedef struct {
  EFI_MEMORY_DESCRIPTOR *Map;
  UINTN MapKey;
  UINTN MapSizeInBytes;
  UINTN BytesPerMemoryDescriptor;
  UINT32 DescriptorVersion;
} MemoryInformation;

typedef struct {
  void* BaseAddress;
  UINT64 BufferSize;
  UINTN PixelWidth;
  UINTN PixelHeight;
  UINTN PixelsPerScanLine;
} LinearPixelFramebuffer;

typedef struct {
  LinearPixelFramebuffer* Framebuffer;
} GraphicalInformation;

typedef struct {
  MemoryInformation    Memory;
  GraphicalInformation Gfx;
} BootInformation;

#endif /* RADII_BOOTLOADER_BOOT_INFORMATION_H */
