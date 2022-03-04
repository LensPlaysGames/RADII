#include "file_operations.h"

#include "common.h"
#include <efi.h>

EFI_GUID EfiSimpleFileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
EFI_GUID EfiLoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;

EFI_STATUS LoadFileAtPath(CHAR16 *Path, EFI_FILE *File, EFI_FILE *Directory) {
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

  return Directory->Open(Directory, &File, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
}
