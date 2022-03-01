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

// UEFI Spec. V2.9, Section 8.3 Time Services, GetTime() Related Definitions
typedef struct {
  UINT16 Year;
  UINT8 Month;
  UINT8 Day;
  UINT8 Hour;
  UINT8 Minute;
  UINT8 Second;
  UINT8 Pad0;
  UINT32 Nanosecond;
  INT16 TimeZone;
  UINT8 Daylight;
  UINT8 Pad1;
} EFI_TIME;

// UEFI Spec. V2.9, Section 4.2
typedef struct {
  UINT64 Signature;
  UINT32 Revision;
  UINT32 HeaderSize;
  UINT32 CRC32;
  UINT32 Reserved;
} EFI_TABLE_HEADER;

// Memory attributes
// UEFI Spec. V2.9, Section 7.2, EFI_BOOT_SERVICES.GetMemoryMap() Related Definitions
#define EFI_MEMORY_UC  0x0000000000000001
#define EFI_MEMORY_WC  0x0000000000000002
#define EFI_MEMORY_WT  0x0000000000000004
#define EFI_MEMORY_WB  0x0000000000000008
#define EFI_MEMORY_UCE 0x0000000000000010
#define EFI_MEMORY_WP  0x0000000000001000
#define EFI_MEMORY_RP  0x0000000000002000
#define EFI_MEMORY_XP  0x0000000000004000
#define EFI_MEMORY_NV  0x0000000000008000
#define EFI_MEMORY_MORE_RELIABLE 0x0000000000010000
#define EFI_MEMORY_RO 0x0000000000020000
#define EFI_MEMORY_SP 0x0000000000040000
#define EFI_MEMORY_CPU_CRYPTO 0x0000000000080000
#define EFI_MEMORY_RUNTIME 0x8000000000000000
#define EFI_MEMORY_DESCRIPTOR_VERSION 1
typedef struct {
  UINT32 Type;
  UINT64 PhysicalAddress;
  UINT64 VirtualAddress;
  UINT64 NumberOfPages;
  UINT64 Attributes;
} EFI_MEMORY_DESCRIPTOR;

// UEFI Spec. V2.9, Section 7.2, EFI_BOOT_SERVICES.AllocatePages() Related Definitions
enum EFI_ALLOCATE_TYPE {
  ALLOCATE_ANY_PAGES,
  ALLOCATE_MAX_ADDRESS,
  ALLOCATE_ADDRESS,
  MAX_ALLOCATE_TYPE,
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
  UNACCEPTED,
  MAX_MEMORY_TYPE,
};

#endif /* RADII_BOOTLOADER_TYPES_H */
