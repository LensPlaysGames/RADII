#ifndef RADII_RESOURCE_TABLE_HEADER_H
#define RADII_RESOURCE_TABLE_HEADER_H

#include <EFI/types.h>

/// The header found at the beginning of each
/// resource in the boot information structure.
///
/// If a version is higher than expected, the lower version's
/// values can still be expected to be in place, and valid.
/// Backwards compatibility is ensured, although the header may grow in size.
typedef struct resource_table_header_t {
  // The unique 16-byte signature (preferred GUID/UUID, but do what you wish).
  unsigned char Signature[16];
  // The size of this header, in bytes.
  UINT64 Size;
  // The length of the entire resource, including this header.
  UINT64 Length;
  // Should always be zero, for now.
  UINT8 Version;
} ResourceTableHeader;

#endif /* #ifndef RADII_RESOURCE_TABLE_HEADER_H */
