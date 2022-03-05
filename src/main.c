/* The RADII Bootloader 
 *   An EFI bootloader made in conjuction with LensorOS, 
 *     my other un-ending hobby project :^).
 *
 * TODO:
 * |-- Implement Elf64_Hdr and family, ELF loader, etc.
 * `-- Parse Configuration Table for ACPI Table, find RSDP and cache for kernel.
 */

#include "boot_information.h"
#include "common.h"
#include <efi.h>
#include "file_operations.h"
#include "loader.h"
#include "simple_print.h"

EFI_STATUS efi_main(EFI_HANDLE *IH, EFI_SYSTEM_TABLE *ST) {
  Initialize(ST, IH);
  Print(L"Hello, World!\r\n");

  EFI_FILE *kernel = LoadFileAtPath(NULL, L"kernel.elf");
  if (kernel == NULL) {
    Print(L"Could not load `kernel.elf` in root directory.\r\n");
    return EFI_LOAD_ERROR;
  }
  Print(L"Successfully loaded kernel.\r\n");

  EFI_STATUS status = EnterElf64Kernel(kernel);
  if (status) {
	Print(L"Could not load and execute the kernel!\r\n");
	return status;
  }
  Print(L"Kernel returned.\r\n");
  return EFI_SUCCESS;
}
