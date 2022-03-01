#ifndef RADII_BOOTLOADER_TYPES_H
#define RADII_BOOTLOADER_TYPES_H

#include <stdbool.h>
#include <stdint.h>

typedef bool        BOOLEAN;
typedef int         INTN;
typedef unsigned    UINTN;
typedef int8_t      INT8;
typedef uint8_t     UINT8;
typedef int16_t     INT16;
typedef uint16_t    UINT16;
typedef int32_t     INT32;
typedef uint32_t    UINT32;
typedef int64_t     INT64;
typedef uint64_t    UINT64;
typedef char        CHAR8;
typedef uint16_t    CHAR16;
typedef void        VOID;

typedef UINTN       EFI_STATUS;
typedef VOID*       EFI_HANDLE;
typedef VOID*       EFI_EVENT;
typedef UINT64      EFI_LOGICAL_BLOCK_ADDRESS;
typedef UINT64      EFI_LBA; // Equivalent to above.
typedef UINTN       EFI_TASK_PRIORITY_LEVEL;
typedef UINTN       EFI_TPL; // Equivalent to above.

typedef struct {
  UINT32 data0;
  UINT16 data1;
  UINT16 data2;
  UINT8  data3[8];
} EFI_GUID;

// UEFI Spec. V2.9, Section 4.2
typedef struct {
  UINT64 Signature;
  UINT32 Revision;
  UINT32 HeaderSize;
  UINT32 CRC32;
  UINT32 Reserved;
} EFI_TABLE_HEADER;

enum EFI_ALLOCATE_TYPE {
  ALLOCATE_ANY_PAGES,
  ALLOCATE_MAX_ADDRESS,
  ALLOCATE_ADDRESS,
  MAX_ALLOCATE_TYPE,
};

struct EFI_MEMORY_DESCRIPTOR {
  UINT32 Type;
  UINT64 PhysicalAddress;
  UINT64 VirtualAddress;
  UINT64 NumberOfPages;
  UINT64 Attributes;
};

enum EFI_MEMORY_TYPE {
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
  MAX_MEMORY_TYPE,
};

#endif /* RADII_BOOTLOADER_TYPES_H */
