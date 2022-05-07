#include <loader.h>

#include <boot_information.h>
#include <common.h>
#include <EFI/efi.h>
#include <simple_print.h>
#include <resource_table.h>

EFI_GUID EfiFileInfoGuid = EFI_FILE_INFO_ID;

EFI_STATUS LoadElf64Header(EFI_FILE *ElfProgram, Elf64_Ehdr *ElfHeader) {
  UINTN headerSize = sizeof(Elf64_Ehdr);
  EFI_STATUS status = ElfProgram->Read(ElfProgram, &headerSize, (VOID*)ElfHeader);
  return status;
}

/// Return zero when header is valid 64-bit ELF.
EFI_STATUS VerifyElf64Header(Elf64_Ehdr* ElfHeader) {
  if (ElfHeader->e_ident[EI_MAG0] != ELFMAG0
      || ElfHeader->e_ident[EI_MAG1] != *ELFMAG1
      || ElfHeader->e_ident[EI_MAG2] != *ELFMAG2
      || ElfHeader->e_ident[EI_MAG3] != *ELFMAG3
      || ElfHeader->e_ident[EI_CLASS] != ELFCLASS64
      || ElfHeader->e_ident[EI_DATA] != ELFDATA2LSB
      || ElfHeader->e_type != ET_EXEC
      || ElfHeader->e_machine != EM_X86_64
      || ElfHeader->e_version != EV_CURRENT)
    {
      return EFI_INVALID_PARAMETER;
    }
  return EFI_SUCCESS;
}

EFI_STATUS LoadElf64ProgramHeaders
(EFI_FILE *ElfProgram
 , Elf64_Ehdr *ElfHeader
 , Elf64_Phdr *ProgramHeaders
 )
{
  EFI_STATUS status;
  UINTN programHeaderSize = ElfHeader->e_phnum * ElfHeader->e_phentsize;
  status = ElfProgram->SetPosition(ElfProgram, ElfHeader->e_phoff);
  if (status) {
    Print(L"Could not set position within file when loading Elf64 program headers.\r\n");
    return status;
  }

  status = BootServices->AllocatePool(LOADER_DATA, programHeaderSize, (void**)&ProgramHeaders);
  if (status) {
    Print(L"Could not allocate memory pool for loading Elf64 program headers.\r\n");
    return status;
  }

  status = ElfProgram->Read(ElfProgram, &programHeaderSize, ProgramHeaders);
  if (status) {
    Print(L"Could not read headers when loading Elf64 program headers.\r\n");
    return status;
  }
  for (
       Elf64_Phdr* phdr = ProgramHeaders;
       (UINT8*)phdr < (UINT8*)ProgramHeaders + programHeaderSize;
       phdr++)
    {
      if (phdr->p_type == PT_LOAD) {
        int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
        // 0x1000 offset to cope with paddr of zero.
        Elf64_Addr segment = phdr->p_paddr != 0 ? phdr->p_paddr : 0x1000;
        EFI_MEMORY_TYPE MemoryType = LOADER_DATA;
        if (phdr->p_flags & PF_X)
          MemoryType = LOADER_CODE;

        status = BootServices->AllocatePages(ALLOCATE_ADDRESS
                                             , MemoryType
                                             , pages
                                             , &segment
                                             );
        if (status) {
          Print(L"Could not allocate pages for program header.\r\n");
          return status;
        }

        status = ElfProgram->SetPosition(ElfProgram, phdr->p_offset);
        if (status) {
          Print(L"Could not set position in file for program header.\r\n");
          return status;
        }

        UINTN programSize = phdr->p_filesz;
        status = ElfProgram->Read(ElfProgram, &programSize, (VOID*)segment);
        if (status) {
          Print(L"Could not read file at offset for program header.\r\n");
          return status;
        }
     }
    }
  Print(L"Successfully loaded program headers\r\n");
  return EFI_SUCCESS;
}

EFI_STATUS EnterElf64(EFI_FILE *ElfProgram) {
  if (ElfProgram == NULL)
    return EFI_INVALID_PARAMETER;

  EFI_STATUS status;

  Elf64_Ehdr header;
  status = LoadElf64Header(ElfProgram, &header);
  if (status)
    return status;

  status = VerifyElf64Header(&header);
  if (status)
    return status;

  // Load program headers into memory.
  Elf64_Phdr *programHeaders = NULL;
  status = LoadElf64ProgramHeaders(ElfProgram, &header, programHeaders);
  if (status)
    return status;

  // Declare program entry point as a function pointer, then call that function.
  __attribute__((sysv_abi)) void (*EntryPoint)() = ((__attribute__((sysv_abi)) void (*)()) header.e_entry);
  EntryPoint();
  
  return EFI_SUCCESS;
}

EFI_STATUS EnterElf64Kernel(EFI_FILE *Kernel) {
  if (Kernel == NULL) {
    Print(L"Kernel EFI_FILE passed to EnterElf64Kernel is NULL!\r\n");
    return EFI_INVALID_PARAMETER;
  }

  EFI_STATUS status;
  Elf64_Ehdr header;

  status = LoadElf64Header(Kernel, &header);
  if (status) {
    Print(L"Could not load Elf64Header from kernel file.\r\n");
    return status;
  }

  status = VerifyElf64Header(&header);
  if (status) {
    Print(L"Elf64 header read from kernel EFI_FILE is invalid!\r\n");
    return EFI_INVALID_PARAMETER;
  }

  Elf64_Phdr *programHeaders = NULL;
  if (LoadElf64ProgramHeaders(Kernel, &header, programHeaders)) {
    Print(L"Could not load Elf64 program headers from kernel file.\r\n");
    return EFI_LOAD_ERROR;
  }

  // Load EFI Memory map to pass to kernel.
  EFI_MEMORY_DESCRIPTOR* Map = NULL;
  UINTN MapSize;
  UINTN MapKey;
  UINTN DescriptorSize;
  UINT32 DescriptorVersion;
  BootServices->GetMemoryMap(&MapSize
                             , Map
                             , &MapKey
                             , &DescriptorSize
                             , &DescriptorVersion
                             );
  status = BootServices->AllocatePool(LOADER_DATA
                                      , MapSize
                                      , (VOID**)&Map
                                      );
  if (status) {
    Print(L"Could not allocate memory pool for EFI memory map.\r\n");
    return status;
  }
  BootServices->GetMemoryMap(&MapSize
                             , Map
                             , &MapKey
                             , &DescriptorSize
                             , &DescriptorVersion
                             );
  Print(L"EFI memory map successfully parsed\r\n");
  resource_table.Memory.Map = Map;
  resource_table.Memory.MapSizeInBytes = MapSize;
  resource_table.Memory.MapKey = MapKey;
  resource_table.Memory.BytesPerMemoryDescriptor = DescriptorSize;
  resource_table.Memory.DescriptorVersion = DescriptorVersion;

  // Exit boot services, as kernel is not meant to ever exit back to here.
  BootServices->ExitBootServices(ImageHandle, resource_table.Memory.MapKey);
  // Declare program entry point as a function pointer, then call that function.
  VOID (__attribute__((sysv_abi)) *KernelEntry)(ResourceTable *) = ((VOID (__attribute__((sysv_abi)) *)(ResourceTable *))header.e_entry);
  KernelEntry(&resource_table);

  return EFI_SUCCESS;
}
