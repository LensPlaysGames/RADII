#include "file_operations.h"

#include "common.h"
#include <efi.h>
#include "simple_print.h"

EFI_GUID EfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
EFI_GUID EfiLoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;

EFI_FILE* LoadFileAtPath(EFI_FILE *Directory, CHAR16 *Path) {
  static EFI_LOADED_IMAGE_PROTOCOL* loadedImage = NULL;
  if (loadedImage == NULL) {
    SystemTable->BootServices->HandleProtocol(ImageHandle
                                              , &EfiLoadedImageProtocolGuid
                                              , (void**)&loadedImage);
  }
  static EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystem = NULL;
  if (fileSystem == NULL) {
    SystemTable->BootServices->HandleProtocol(loadedImage->DeviceHandle
                                              , &EfiSimpleFileSystemProtocolGuid
                                              , (void**)&fileSystem);
  }
  // If no directory was passed, open root.
  if (Directory == NULL)
    fileSystem->OpenVolume(fileSystem, &Directory);
  
  EFI_FILE* loadedFile;
  EFI_STATUS status = Directory->Open(Directory, &loadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
  if (status == EFI_SUCCESS)
	return loadedFile;

  return NULL;
}
