#include <simple_print.h>

#include <common.h>
#include <EFI/types.h>

VOID Print(CHAR16* str) {
  SystemTable->ConsoleOut->OutputString(SystemTable->ConsoleOut, str);
}
