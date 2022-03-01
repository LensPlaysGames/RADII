#ifndef RADII_BOOTLOADER_TYPES_H
#define RADII_BOOTLOADER_TYPES_H

#include <stddef>
#include <stdint>

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
typedef int128_t    INT128;
typedef uint128_t   UINT128;
typedef signed char CHAR8;
typedef short       CHAR16;
typedef void        VOID;

typedef UINTN       EFI_STATUS;
typedef VOID*       EFI_HANDLE;
typedef VOID*       EFI_EVENT;
typedef UINT64      EFI_LOGICAL_BLOCK_ADDRESS;
typedef UINT64      EFI_LBA; // Equivalent to above.
typedef UINTN       EFI_TASK_PRIORITY_LEVEL;
typedef UINTN       EFI_TPL; // Equivalent to above.

#endif /* RADII_BOOTLOADER_TYPES_H */
