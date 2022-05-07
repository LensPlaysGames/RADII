/* The RADII Bootloader 
 *   An EFI bootloader made in conjuction with LensorOS, 
 *     my other un-ending hobby project :^).
 *
 * TODO:
 * |-- Expand `table.json` to `config.json`, also include "kernel-virtual", "kernel-physical" addresses for ELF loader.
 * `-- Parse Configuration Table for ACPI Table, find RSDP and cache for kernel.
 */

#include <boot_information.h>
#include <common.h>
#include <EFI/efi.h>
#include <file_operations.h>
#include <loader.h>
#include <simple_print.h>
#include <resource_table.h>

LinearPixelFramebuffer framebuffer;
LinearPixelFramebuffer InitializeGOP() {
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_STATUS status = BootServices->LocateProtocol(&gopGuid, NULL, (void**)&gop);
    if(status) {
        Print(L"Unable to locate Graphics Output Protocol\r\n");
        framebuffer.BaseAddress = 0;
        return framebuffer;
    }
    Print(L"GOP located successfully\r\n");
    framebuffer.BaseAddress = (void*)gop->Mode->FrameBufferBase;
    framebuffer.BufferSize = gop->Mode->FrameBufferSize;
    framebuffer.PixelWidth = gop->Mode->Info->HorizontalResolution;
    framebuffer.PixelHeight = gop->Mode->Info->VerticalResolution;
    framebuffer.PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;
    return framebuffer;
}

EFI_STATUS efi_main(EFI_HANDLE *IH, EFI_SYSTEM_TABLE *ST) {
  /* At this point, the machine is in the state described
   *   in the UEFI Spec. V2.9, Section 2.3.4 x64 Platforms
   *
   * For a brief summary, the machine:
   *   - is in 64-bit long mode.
   *   - has paging enabled and memory is identity mapped (1:1 address-space).
   *   - has interrupts enabled but no handlers are installed.
   */
  Initialize(ST, IH);
  // The `L` is needed to signify a wide-character string (16-bit characters).
  Print(L"Hello, friends!\r\n");

  resource_table.Gfx.Framebuffer = InitializeGOP();
  if (resource_table.Gfx.Framebuffer.BaseAddress == 0) {
    Print(L"Error when getting GOP framebuffer.\r\n");
    return EFI_NOT_FOUND;
  }

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
