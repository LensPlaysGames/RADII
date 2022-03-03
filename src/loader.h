#ifndef RADII_BOOTLOADER_LOADER_H
#define RADII_BOOTLOADER_LOADER_H

#include <efi.h>

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

/// `e_type` posiibilities.
#define ET_NONE   0      // No file type
#define ET_REL    1      // Relocatable
#define ET_EXEC   2      // Exucutable
#define ET_DYN    3      // Shared Object
#define ET_CORE   4
#define ET_LOOS   0xfe00 // Operating-system specific
#define ET_HIOS   0xfeff // Operating-system specific
#define ET_LOPROC 0xff00 // Processor specific
#define ET_HIPROC 0xffff // Processor specific

/// only some `e_machine` possibilities, there are hundreds.
#define EM_NONE    0
#define EM_386     3   // Intel 80386
#define EM_PPC64   21  // 64-bit PowerPC
#define EM_IA_64   50  // Intel IA-64
#define EM_X86_64  62  // AMD X86-64
#define EM_AARCH64 183 // ARM 64-bit
#define EM_RISCV   243

/// `e_version` possibilities.
#define EV_NONE    0
#define EV_CURRENT 1

/// Sections
#define SHN_UNDEF     0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC    0xff00
#define SHN_HIPROC    0xff1f
#define SHN_LOOS      0xff20
#define SHN_HIOS      0xff3f
#define SHN_ABS       0xfff1
#define SHN_COMMON    0xfff2
#define SHN_XINDEX    0xffff
#define SHN_HIRESERVE 0xffff

#define SHT_NULL          0
#define SHT_PROGBITS      1
#define SHT_SYMTAB        2
#define SHT_STRTAB        3
#define SHT_RELA          4
#define SHT_HASH          5
#define SHT_DYNAMIC       6
#define SHT_NOTE          7
#define SHT_NOBITS        8
#define SHT_REL           9
#define SHT_SHLIB         10
#define SHT_DYNSYM        11
#define SHT_INIT_ARRAY    14
#define SHT_FINI_ARRAY    15
#define SHT_PREINIT_ARRAY 16
#define SHT_GROUP         17
#define SHT_SYMTAB_SHNDX  18
#define SHT_LOOS          0x60000000
#define SHT_HIOS          0x6fffffff
#define SHT_LOPROC        0x70000000
#define SHT_HIPROC        0x7fffffff
#define SHT_LOUSER        0x80000000
#define SHT_HIUSER        0xffffffff

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

typedef struct {
  unsigned char e_ident[EI_NIDENT];
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;
  Elf64_Off e_phoff;
  Elf64_Off e_shoff;
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx; // FIXME: ???
} Elf64_Ehdr;

typedef struct {
  Elf64_Word sh_name;
  Elf64_Word sh_type;
  Elf64_Xword sh_flags;
  Elf64_Addr sh_addr;
  Elf64_Off sh_offset;
  Elf64_Xword sh_size;
  Elf64_Word sh_link;
  Elf64_Word sh_info;
  Elf64_Xword sh_addralign;
  Elf64_Xword sh_entsize;
} Elf64_Shdr;

#endif /* RADII_BOOTLOADER_LOADER_H */
