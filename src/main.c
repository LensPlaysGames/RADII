/* The RADII Bootloader 
 *   An EFI bootloader made in conjuction with LensorOS, 
 *     my other un-ending hobby project :^).
 *
 * TODO:
 * |-- Implement Elf64_Hdr and family, ELF loader, etc.
 * `-- Parse Configuration Table for ACPI Table, find RSDP and cache for kernel.
 */

#include "common.h"
#include <efi.h>

EFI_STATUS efi_main(EFI_HANDLE *IH, EFI_SYSTEM_TABLE *ST) {
  Initialize(ST, IH);
  SystemTable->ConsoleOut->OutputString(SystemTable->ConsoleOut, L"Hello, World!\r\n");
  return 0;
}
