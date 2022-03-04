#include "loader.h"

#include "boot_information.h"
#include "common.h"
#include <efi.h>

EFI_GUID EfiFileInfoGuid = EFI_FILE_INFO_ID;

EFI_STATUS LoadElf64Header(EFI_FILE *ElfProgram, Elf64_Ehdr *ElfHeader) {
  UINTN fileInfoSize = 0;
  EFI_FILE_INFO *fileInfo = NULL;
  // Get metadata size from the passed file.
  ElfProgram->GetInfo(ElfProgram, &EfiFileInfoGuid, &fileInfoSize, NULL);
  // Allocate memory pool for file metadata.
  SystemTable->BootServices->AllocatePool(LOADER_DATA, fileInfoSize, (void**)&fileInfo);
  // Fill memory with metadata from passed file.
  ElfProgram->GetInfo(ElfProgram, &EfiFileInfoGuid, &fileInfoSize, (void**)&fileInfo);
  // Read and verify file header as 64-bit ELF header.
  UINTN headerSize = sizeof(Elf64_Ehdr);
  ElfProgram->Read(ElfProgram, &headerSize, &ElfHeader);
  return EFI_SUCCESS;
}

/// Return zero when header is in valid 64-bit ELF.
EFI_STATUS VerifyElf64Header(Elf64_Ehdr* header) {
  if (header->e_ident[EI_MAG0] != ELFMAG0
      || header->e_ident[EI_MAG1] != *ELFMAG1
      || header->e_ident[EI_MAG2] != *ELFMAG2
      || header->e_ident[EI_MAG3] != *ELFMAG3
      || header->e_ident[EI_CLASS] != ELFCLASS64
      || header->e_ident[EI_DATA] != ELFDATA2LSB
      || header->e_type != ET_EXEC
      || header->e_machine != EM_X86_64
      || header->e_version != EV_CURRENT)
    {
      return EFI_INVALID_PARAMETER;
    }  
  return EFI_SUCCESS;
}

EFI_STATUS LoadElf64ProgramHeaders
(
 EFI_FILE *ElfProgram
 , Elf64_Ehdr *ElfHeader
 , Elf64_Phdr *ProgramHeaders
 )
{
  ElfProgram->SetPosition(ElfProgram, ElfHeader->e_phoff);
  UINTN programHeaderSize = ElfHeader->e_phnum * ElfHeader->e_phentsize;
  SystemTable->BootServices->AllocatePool(LOADER_DATA, programHeaderSize, (void**)&ProgramHeaders);
  ElfProgram->Read(ElfProgram, &programHeaderSize, ProgramHeaders);
  for (
       Elf64_Phdr* phdr = ProgramHeaders;
       (char*)phdr < (char*)ProgramHeaders + ElfHeader->e_phnum * ElfHeader->e_phentsize;
       phdr = (Elf64_Phdr*)((char*)phdr + ElfHeader->e_phentsize))
    {
      if (phdr->p_type == PT_LOAD) {
        // Allocate pages for program
        int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
        Elf64_Addr segment = phdr->p_paddr;
        SystemTable->BootServices->AllocatePages(ALLOCATE_ADDRESS, LOADER_DATA, pages, &segment);
        ElfProgram->SetPosition(ElfProgram, phdr->p_offset);
        UINTN programHeaderSize = phdr->p_filesz;
        ElfProgram->Read(ElfProgram, &programHeaderSize, (void*)segment);
      }
    }
  return EFI_SUCCESS;
}

EFI_STATUS EnterElf64(EFI_FILE *ElfProgram) {
  if (SystemTable == NULL
      || ElfProgram == NULL)
    {
      return EFI_INVALID_PARAMETER;   
    }

  Elf64_Ehdr header;
  if (LoadElf64Header(ElfProgram, &header))
    return EFI_LOAD_ERROR;

  if (VerifyElf64Header(&header))
    return EFI_INVALID_PARAMETER;

  // Load program headers into memory.
  Elf64_Phdr *programHeaders = NULL;
  if (LoadElf64ProgramHeaders(ElfProgram, &header, programHeaders))
    return EFI_LOAD_ERROR;

  // Declare program entry point as a function pointer, then call that function.
  __attribute__((sysv_abi)) void (*EntryPoint)() = ((__attribute__((sysv_abi)) void (*)()) header.e_entry);
  EntryPoint();
  
  return EFI_SUCCESS;
}

EFI_STATUS EnterElf64Kernel(EFI_FILE *Kernel, BootInformation* BootInfo) {
  if (Kernel == NULL
      || BootInfo == NULL)
    {
      return EFI_INVALID_PARAMETER;
    }

  Elf64_Ehdr header;
  if (LoadElf64Header(Kernel, &header))
    return EFI_LOAD_ERROR;

  if (VerifyElf64Header(&header))
    return EFI_INVALID_PARAMETER;

  // Load program headers into memory.
  Elf64_Phdr *programHeaders = NULL;
  if (LoadElf64ProgramHeaders(Kernel, &header, programHeaders))
    return EFI_LOAD_ERROR; 

  // Exit boot services, as kernel is not meant to ever exit back to here.
  SystemTable->BootServices->ExitBootServices(ImageHandle, MemoryInfo.MapKey);

  // Declare program entry point as a function pointer, then call that function.
  __attribute__((sysv_abi)) void (*EntryPoint)(BootInformation *) = ((__attribute__((sysv_abi)) void (*)(BootInformation *)) header.e_entry);
  EntryPoint(BootInfo);

  return EFI_SUCCESS;
}
