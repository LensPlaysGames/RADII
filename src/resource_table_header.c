#include <resource_table_header.h>

#include <EFI/types.h>

EFI_STATUS compare_signature
(
 const unsigned char *SignatureA,
 const unsigned char *SignatureB
 )
{
  if (SignatureA == NULL || SignatureB == NULL)
    return EFI_INVALID_PARAMETER;

  for (UINT8 i = 0; i < 16; ++i) {
    if (*SignatureA != *SignatureB)
      return EFI_COMPROMISED_DATA;

    SignatureA++;
    SignatureB++;
  }
  return EFI_SUCCESS;
}

VOID find_resource_table_header(ResourceTable *Table, const unsigned char *Signature, ResourceTableHeader *Header) {
  if (Table == NULL)
    return;

  if (Signature == NULL)
    return;

  Header = NULL;

  ResourceTableHeader *it = (ResourceTableHeader *)Table;
  EFI_STATUS status;
  while(Header == NULL) {
    status = compare_signature(Signature, it->Signature);
    if(status == EFI_SUCCESS) {
      Header = it;
      break;
    }
    if (it->Length == 0)
      break;

    it = (ResourceTableHeader *)((uintptr_t)it + it->Length);
  }
}
