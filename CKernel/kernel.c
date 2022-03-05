/* For build instructions, see the README.md 
 *   within the same directory as this file.
 */

#include <stdint.h>

/// Matches `EFI/types.h`.
// This is needed to parse the memory map passed by the RADII bootloader.
typedef enum {
  RESERVED,
  LOADER_CODE,
  LOADER_DATA,
  BOOT_SERVICES_CODE,
  BOOT_SERVICES_DATA,
  RUNTIME_SERVICES_CODE,
  RUNTIME_SERVICES_DATA,
  CONVENTIONAL,
  UNUSABLE,
  ACPI_RECLAIM,
  ACPI_NVS,
  MEMORY_MAPPED_IO,
  MEMORY_MAPPED_IO_PORT_SPACE,
  PAL_CODE,
  PERSISTENT_MEMORY,
  UNACCEPTED,
  MAX_MEMORY_TYPE,
} EFI_MEMORY_TYPE;
typedef struct {
  uint32_t Type;
  uint64_t PhysicalAddress;
  uint64_t VirtualAddress;
  uint64_t NumberOfPages;
  uint64_t Attributes;
} EFI_MEMORY_DESCRIPTOR;

/// Matches `boot_information.h`.
typedef struct {
  EFI_MEMORY_DESCRIPTOR *Map;
  unsigned int MapKey;
  unsigned int MapSizeInBytes;
  unsigned int BytesPerMemoryDescriptor;
  unsigned int DescriptorSize;
  uint32_t DescriptorVersion;
} MemoryInformation;
typedef struct {
  void* BaseAddress;
  uint64_t BufferSize;
  unsigned int PixelWidth;
  unsigned int PixelHeight;
  unsigned int PixelsPerScanLine;
} LinearPixelFramebuffer;
typedef struct {
  LinearPixelFramebuffer* Framebuffer;
} GraphicalInformation;
typedef struct {
  MemoryInformation Memory;
  GraphicalInformation Gfx;
} BootInformation;

void _start(BootInformation *BootInfo) {
  (void)BootInfo; // Unused
  // Do nothing, forever :)
  while (1) {
    asm ("hlt");
  }
}
