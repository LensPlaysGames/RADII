#include "loader.h"

#include "boot_information.h"
#include "common.h"
#include <efi.h>
#include "simple_print.h"

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

EFI_STATUS LoadElf64ProgramHeaders(EFI_FILE *ElfProgram, Elf64_Ehdr *ElfHeader, Elf64_Phdr *ProgramHeaders) {
  EFI_STATUS status;
  UINTN programHeaderSize = ElfHeader->e_phnum * ElfHeader->e_phentsize;
  status = ElfProgram->SetPosition(ElfProgram, ElfHeader->e_phoff);
  if (status)
	return status;

  status = BootServices->AllocatePool(LOADER_DATA, programHeaderSize, (void**)&ProgramHeaders);
  if (status)
	return status;

  status = ElfProgram->Read(ElfProgram, &programHeaderSize, ProgramHeaders);
  if (status)
	return status;

  for (
       Elf64_Phdr* phdr = ProgramHeaders;
       (char*)phdr < (char*)ProgramHeaders + programHeaderSize;
       phdr = (Elf64_Phdr*)((char*)phdr + ElfHeader->e_phentsize))
    {
      if (phdr->p_type == PT_LOAD) {
        // Allocate pages for program
        int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
        Elf64_Addr segment = phdr->p_paddr;
        status = BootServices->AllocatePages(ALLOCATE_ADDRESS, LOADER_DATA, pages, &segment);
		if (status)
		  return status;

        status = ElfProgram->SetPosition(ElfProgram, phdr->p_offset);
		if (status)
		  return status;

        UINTN programSize = phdr->p_filesz;
        status = ElfProgram->Read(ElfProgram, &programSize, (VOID*)segment);
		if (status)
		  return status; 
     }
    }
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

EFI_STATUS EnterElf64Kernel(EFI_FILE *Kernel, BootInformation *BootInfo) {
  if (Kernel == NULL || BootInfo == NULL)
	return EFI_INVALID_PARAMETER;

  EFI_STATUS status;
  Elf64_Ehdr header;

  status = LoadElf64Header(Kernel, &header);
  if (status)
    return status;

  status = VerifyElf64Header(&header);
  if (status)
    return EFI_INVALID_PARAMETER;

  Elf64_Phdr *programHeaders = NULL;
  if (LoadElf64ProgramHeaders(Kernel, &header, programHeaders))
    return EFI_LOAD_ERROR; 

  // Ensure boot information is up to date with most recent memory map.
  BootServices->GetMemoryMap(&MemoryInfo.MapSizeInBytes
							 , MemoryInfo.Map
							 , &MemoryInfo.MapKey
							 , &MemoryInfo.BytesPerMemoryDescriptor
							 , &MemoryInfo.DescriptorVersion);
  // Allocating the memory map itself may (in rare cases)
  //   increase the size of the map by two descriptors.
  MemoryInfo.MapSizeInBytes += MemoryInfo.BytesPerMemoryDescriptor * 2;
  BootServices->AllocatePool(LOADER_DATA
							 , MemoryInfo.MapSizeInBytes
							 , (VOID**)&MemoryInfo.Map);
  BootServices->GetMemoryMap(&MemoryInfo.MapSizeInBytes
							 , MemoryInfo.Map
							 , &MemoryInfo.MapKey
							 , &MemoryInfo.BytesPerMemoryDescriptor
							 , &MemoryInfo.DescriptorVersion);

  // Exit boot services, as kernel is not meant to ever exit back to here.
  BootServices->ExitBootServices(ImageHandle, MemoryInfo.MapKey);

  // Declare program entry point as a function pointer, then call that function.
  VOID (__attribute__((sysv_abi)) *KernelEntry)(BootInformation *) = ((VOID (__attribute__((sysv_abi)) *)(BootInformation *))header.e_entry);
  KernelEntry(BootInfo);

  return EFI_SUCCESS;
}
