/* The RADII Bootloader 
 *   An EFI bootloader made in conjuction with LensorOS, 
 *     my other un-ending hobby project :^).
 *
 * TODO:
 * |-- Implement Elf64_Hdr and family, ELF loader, etc.
 * `-- Parse Configuration Table for ACPI Table, find RSDP and cache for kernel.
 */

#include <efi.h>

EFI_STATUS efi_main(EFI_HANDLE *imageHandle, EFI_SYSTEM_TABLE *systemTable) {
  (VOID)imageHandle; // unused
  systemTable->ConsoleOut->OutputString(systemTable->ConsoleOut, L"Hello, World!\r\n");
  return 0;
}
