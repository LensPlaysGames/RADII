#include "types.h"

// TODO: Implement relocation function to relocate an ELF file.
// https://github.com/tqh/uefi-example/blob/master/platform/x86_64/relocation_func.cpp
// https://github.com/eruffaldi/uefiboot
EFI_STATUS _relocate (long ldbase, Elf64_Dyn *)
