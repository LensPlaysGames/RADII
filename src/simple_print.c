#include "simple_print.h"

#include "common.h"
#include <types.h>

VOID Print(CHAR16* str) {
  SystemTable->ConsoleOut->OutputString(SystemTable->ConsoleOut, str);
}
