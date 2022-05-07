#include <common.h>

#include <boot_information.h>
#include <EFI/efi.h>
#include <resource_table.h>

EFI_SYSTEM_TABLE *SystemTable;
EFI_HANDLE *ImageHandle;
EFI_BOOT_SERVICES *BootServices;

CHAR16 *EFI_ERROR_STRINGS[] = {
  L"Success",
  L"Load Error",
  L"Invalid Parameter",
  L"Unsupported",
  L"Bad Buffer Size",
  L"Buffer Too Small",
  L"Not Ready",
  L"Device Error",
  L"Write Protected",
  L"Out of Resources",
  L"Volume Corrupted",
  L"Volume Full",
  L"No Media",
  L"Media Changed",
  L"Not Found",
  L"Access Denied",
  L"No Response",
  L"No Mapping",
  L"Timeout",
  L"Not Started",
  L"Already Started",
  L"ABORTED",
  L"ICMP_ERROR",
  L"TFTP_ERROR",
  L"PROTOCOL_ERROR",
  L"INCOMPATIBLE_VERSION",
  L"SECURITY_VIOLATION",
  L"CRC_ERROR",
  L"END_OF_MEDIA",
  L"END_OF_FILE",
  L"INVALID_LANGUAGE",
  L"COMPROMISED_DATA",
  L"IP_ADDRESS_CONFLICT",
  L"HTTP_ERROR",
};

EFI_STATUS Initialize(EFI_SYSTEM_TABLE *ST, EFI_HANDLE *IH) {
  SystemTable = ST;
  ImageHandle = IH;
  BootServices = SystemTable->BootServices;
  return EFI_SUCCESS;
}
