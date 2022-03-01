/* The RADII Bootloader 
*    An EFI bootloader made in conjuction with LensorOS, 
*      my other un-ending hobby project :^).
*/

#include "EFI/types.h"
#include "EFI/system_table.h"

EFI_STATUS efi_main(EFI_HANDLE *imageHandle, EFI_SYSTEM_TABLE *systemTable) {
  (VOID)imageHandle; // unused
  systemTable->ConsoleOut->OutputString(systemTable->ConsoleOut, L"Hello, World!\r\n");
  return 0;
}
