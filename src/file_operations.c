#include "file_operations.h"

#include "common.h"
#include <efi.h>
#include "simple_print.h"

EFI_GUID EfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
EFI_GUID EfiLoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;

EFI_FILE* LoadFileAtPath(EFI_FILE *Directory, CHAR16 *Path) {
  static EFI_LOADED_IMAGE_PROTOCOL* loadedImage = NULL;
  EFI_STATUS status;
  if (loadedImage == NULL) {
    status = SystemTable->BootServices->HandleProtocol(ImageHandle
													   , &EfiLoadedImageProtocolGuid
													   , (VOID**)&loadedImage);
	if (status != EFI_SUCCESS)
	  return NULL;

    Print(L"Got loaded image.\r\n");
  }
  static EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystem = NULL;
  if (fileSystem == NULL) {
    status = SystemTable->BootServices->HandleProtocol(loadedImage->DeviceHandle
													   , &EfiSimpleFileSystemProtocolGuid
													   , (VOID**)&fileSystem);
    if (status != EFI_SUCCESS)
	  return NULL;

	Print(L"Got filesystem.\r\n");
  }
  // If no directory was passed, open root.
  if (Directory == NULL) {
    status = fileSystem->OpenVolume(fileSystem, &Directory);
	if (status != EFI_SUCCESS)
	  return NULL;

    Print(L"Opened root directory.\r\n");
  }
  
  EFI_FILE* loadedFile;
  status = Directory->Open(Directory, &loadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
  if (status == EFI_SUCCESS) {
    Print(L"Successfully loaded file.\r\n");
    return loadedFile;
  }
  Print(L"Could not load file\r\n");
  return NULL;
}
