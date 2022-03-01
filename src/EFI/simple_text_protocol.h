#ifndef RADII_BOOTLOADER_SIMPLE_TEXT_PROTOCOL_H
#define RADII_BOOTLOADER_SIMPLE_TEXT_PROTOCOL_H

#include "types.h"

#define EFI_SIMPLE_TEXT_INPUT_PROTOCOL_GUID \
  {0x387477c1,0x69c7,0x11d2, {0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

#define EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID \
  {0x387477c2,0x69c7,0x11d2, {0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

typedef struct {
  UINT16 ScanCode;
  CHAR16 UnicodeChar;
} EFI_INPUT_KEY;

typedef struct {
  VOID (*unused0)();    // Reset()
  VOID (*unused1)();    // ReadKeyStroke()
  EFI_EVENT (*unused2); // WaitForKey()
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct {
  INT32 MaxMode;
  // Current Settings
  INT32 Mode;
  INT32 Attribute;
  INT32 CursorColumn;
  INT32 CursorRow;
  INT32 CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

typedef struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
  EFI_STATUS (*Reset)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *self, BOOLEAN);
  EFI_STATUS (*OutputString)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *self, CHAR16 *string);
  EFI_STATUS (*TestString)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *self, CHAR16 *string);
  EFI_STATUS (*unused3)(); // QueryMode()
  EFI_STATUS (*unused4)(); // SetMode()
  EFI_STATUS (*unused5)(); // SetAttribute()
  EFI_STATUS (*ClearScreen)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *self);
  EFI_STATUS (*unused7)(); // SetCursorPosition()
  EFI_STATUS (*unused8)(); // EnableCursor()
  SIMPLE_TEXT_OUTPUT_MODE *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

#endif /* RADII_BOOTLOADER_SIMPLE_TEXT_PROTOCOL_H */
