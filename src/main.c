/* The RADII Bootloader 
 *   An EFI bootloader made in conjuction with LensorOS, 
 *     my other un-ending hobby project :^).
 *
 * TODO:
 * `-- Parse Configuration Table for ACPI Table, find RSDP and cache for kernel.
 */

#include <boot_information.h>
#include <common.h>
#include <EFI/efi.h>
#include <file_operations.h>
#include <loader.h>
#include <simple_print.h>

EFI_STATUS efi_main(EFI_HANDLE *IH, EFI_SYSTEM_TABLE *ST) {
  /* At this point, the machine is in the state described
   *   in the UEFI Spec. V2.9, Section 2.3.4 x64 Platforms
   *
   * For a brief summary, the machine:
   *   - Is in 64-bit long mode.
   *   - Has paging enabled and UEFI memory map is identity mapped.
   *   - Has interrupts enabled but no handlers are installed.
   */
  
  Initialize(ST, IH);
  // The `L` is needed to signify a wide-character string (16-bit characters).
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
