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

/// Matches `resource_table_header.h`
typedef struct resource_table_header_t {
  // The unique 16-byte signature (preferred GUID/UUID, but do what you wish).
  unsigned char Signature[16];
  // The size of this header, in bytes.
  uint64_t Size;
  // The length of the entire resource, including this header.
  uint64_t Length;
  // Should always be zero, for now.
  uint8_t Version;
} ResourceTableHeader;

/// Matches `boot_information.h`.
typedef struct {
  ResourceTableHeader Header;
  EFI_MEMORY_DESCRIPTOR *Map;
  unsigned int MapKey;
  unsigned int MapSizeInBytes;
  unsigned int BytesPerMemoryDescriptor;
  unsigned int DescriptorSize;
  uint32_t DescriptorVersion;
} MemoryInformation;
typedef struct {
  ResourceTableHeader Header;
  void* BaseAddress;
  uint64_t BufferSize;
  unsigned int PixelWidth;
  unsigned int PixelHeight;
  unsigned int PixelsPerScanLine;
} LinearPixelFramebuffer;
typedef struct {
  ResourceTableHeader Header;
  LinearPixelFramebuffer* Framebuffer;
} GraphicalInformation;

void _start(ResourceTableHeader *BootInfo) {
  (void)BootInfo; // Unused

  // TODO: Parse some sort of information from resource table to prove it works, at least somewhat.

  // Do nothing, forever :)
  while (1)
    asm ("hlt");
}
