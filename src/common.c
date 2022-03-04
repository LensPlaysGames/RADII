#include "common.h"

EFI_SYSTEM_TABLE *SystemTable;
EFI_HANDLE *ImageHandle;

EFI_STATUS Initialize(EFI_SYSTEM_TABLE *ST, EFI_HANDLE *IH) {
  SystemTable = ST;
  ImageHandle = IH;
  return EFI_SUCCESS;
}
