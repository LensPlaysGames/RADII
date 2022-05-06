#ifndef RADII_BOOTLOADER_LOADER_H
#define RADII_BOOTLOADER_LOADER_H

#include <boot_information.h>
#include <EFI/efi.h>

/// Thanks to the following for the in-depth information.
///   http://www.sco.com/developers/gabi/latest/contents.html

#define ELFMAG0 0x7f
#define ELFMAG1 "E"
#define ELFMAG2 "L"
#define ELFMAG3 "F"

#define ELFCLASSNONE 0
#define ELFCLASS32   1
#define ELFCLASS64   2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define ELFOSABI_NONE     0
#define ELFOSABI_HPUX     1
#define ELFOSABI_NETBSD   2
#define ELFOSABI_GNU      3
#define ELFOSABI_LINUX    3
#define ELFOSABI_SOLARIS  6
#define ELFOSABI_AIX      7
#define ELFOSABI_IRIX     8
#define ELFOSABI_FREEBSD  9
#define ELFOSABI_TRU64    10
#define ELFOSABI_MODESTO  11
#define ELFOSABI_OPENBSD  12
#define ELFOSABI_OPENVMS  13
#define ELFOSABI_NSK      14
#define ELFOSABI_AROS     15
#define ELFOSABI_FENIXOS  16
#define ELFOSABI_CLOUDABI 17
#define ELFOSABI_OPENVOS  18

/// `Elf64_Ehdr.e_ident` index
#define EI_MAG0         0
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4
#define EI_DATA         5
#define EI_VERSION      6
#define EI_OSABI        7
#define EI_ABIVERSION   8
#define EI_PAD          9 // Padding bytes
#define EI_NIDENT       16

/// `Elf64_Ehdr.e_type` posiibilities.
#define ET_NONE   0      // No file type
#define ET_REL    1      // Relocatable
#define ET_EXEC   2      // Exucutable
#define ET_DYN    3      // Shared Object
#define ET_CORE   4
#define ET_LOOS   0xfe00 // Operating-system specific
#define ET_HIOS   0xfeff // Operating-system specific
#define ET_LOPROC 0xff00 // Processor specific
#define ET_HIPROC 0xffff // Processor specific

/// only some `Elf64_Ehdr.e_machine` possibilities, there are hundreds.
#define EM_NONE    0
#define EM_386     3   // Intel 80386
#define EM_PPC64   21  // 64-bit PowerPC
#define EM_IA_64   50  // Intel IA-64
#define EM_X86_64  62  // AMD X86-64
#define EM_AARCH64 183 // ARM 64-bit
#define EM_RISCV   243

/// `Elf64_Ehdr.e_version` possibilities.
#define EV_NONE    0
#define EV_CURRENT 1

/// `Elf64_Phdr.p_type`
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_TLS     7
#define PT_LOOS    0x60000000
#define PT_HIOS    0x6fffffff
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff

/// `Elf64_Phdr.p_flags` bit-masks
#define PF_X        0x1
#define PF_W        0x2
#define PF_R        0x4
#define PF_MASKOS   0x0ff00000
#define PF_MASKPROC 0xf0000000

/// 64-bit Executable and Linkable Format (ELF) types
typedef UINT64 Elf64_Addr;
typedef UINT64 Elf64_Off;
typedef UINT16 Elf64_Half;
typedef UINT32 Elf64_Word;
typedef INT32 Elf64_SWord;
typedef UINT64 Elf64_XWord;
typedef INT64 Elf64_SXWord;
typedef UINT8 Elf64_Byte;
typedef UINT16 Elf64_Section;

/// This structure is found at the very beginning of every ELF file.
typedef struct {
  unsigned char e_ident[EI_NIDENT];
  Elf64_Half    e_type;
  Elf64_Half    e_machine;
  Elf64_Word    e_version;
  Elf64_Addr    e_entry;
  Elf64_Off     e_phoff;
  Elf64_Off     e_shoff;
  Elf64_Word    e_flags;
  Elf64_Half    e_ehsize;
  Elf64_Half    e_phentsize;
  Elf64_Half    e_phnum;
  Elf64_Half    e_shentsize;
  Elf64_Half    e_shnum;
  Elf64_Half    e_shstrndx;
} Elf64_Ehdr;

typedef struct {
  Elf64_Word  p_type;
  Elf64_Word  p_flags;
  Elf64_Off   p_offset;
  Elf64_Addr  p_vaddr;
  Elf64_Addr  p_paddr;
  Elf64_XWord p_filesz;
  Elf64_XWord p_memsz;
  Elf64_XWord p_align;
} Elf64_Phdr;

EFI_STATUS EnterElf64(EFI_FILE *ElfProgram);
EFI_STATUS EnterElf64Kernel(EFI_FILE *Kernel);

#endif /* RADII_BOOTLOADER_LOADER_H */
