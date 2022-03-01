#ifndef RADII_BOOTLOADER_SYSTEM_TABLE
#define RADII_BOOTLOADER_SYSTEM_TABLE

// FIXME: Should this file be renamed to `tables.h` ???

#include "types.h"
#include "simple_text_protocol.h"

// UEFI Spec. V2.9, Section 4.3
#define EFI_SYSTEM_TABLE_SIGNATURE 0x5453595320494249
#define EFI_2_90_SYSTEM_TABLE_REVISION ((2 << 16) | (90))
#define EFI_2_80_SYSTEM_TABLE_REVISION ((2 << 16) | (80))
#define EFI_2_70_SYSTEM_TABLE_REVISION ((2 << 16) | (70))
#define EFI_2_60_SYSTEM_TABLE_REVISION ((2 << 16) | (60))
#define EFI_2_50_SYSTEM_TABLE_REVISION ((2 << 16) | (50))
#define EFI_2_40_SYSTEM_TABLE_REVISION ((2 << 16) | (40))
#define EFI_2_31_SYSTEM_TABLE_REVISION ((2 << 16) | (31))
#define EFI_2_30_SYSTEM_TABLE_REVISION ((2 << 16) | (30))
#define EFI_2_20_SYSTEM_TABLE_REVISION ((2 << 16) | (20))
#define EFI_2_10_SYSTEM_TABLE_REVISION ((2 << 16) | (10))
#define EFI_2_00_SYSTEM_TABLE_REVISION ((2 << 16) | (00))
#define EFI_1_10_SYSTEM_TABLE_REVISION ((1 << 16) | (10))
#define EFI_1_02_SYSTEM_TABLE_REVISION ((1 << 16) | (02))
#define EFI_SYSTEM_TABLE_REVISION EFI_2_90_SYSTEM_TABLE_REVISION
#define EFI_SPECIFICATION_VERSION EFI_SYSTEM_TABLE_REVISION

#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION EFI_SPECIFICATION_VERSION

typedef struct {
  EFI_TABLE_HEADER Hdr;

  // Task Priority Services
  void (*unused0)(); // RaiseTPL()
  void (*unused1)(); // RestoreTPL()

  // Memory Services
  EFI_STATUS (*AllocatePages)(enum EFI_ALLOCATE_TYPE, enum EFI_MEMORY_TYPE, UINTN pages, UINT64 *memory);
  EFI_STATUS (*FreePages)(UINT64, UINTN);
  EFI_STATUS (*GetMemoryMap)(UINTN *, struct EFI_MEMORY_DESCRIPTOR *, UINTN *, UINTN *, UINT32 *);
  EFI_STATUS (*AllocatePool)(enum EFI_MEMORY_TYPE, UINTN, VOID**);
  EFI_STATUS (*FreePool)(VOID*);

  // Event & Timer Services
  EFI_STATUS (*unused7)();  // CreateEvent()
  EFI_STATUS (*unused8)();  // SetTimer()
  EFI_STATUS (*unused9)();  // WaitForEvent()
  EFI_STATUS (*unused10)(); // SignalEvent()
  EFI_STATUS (*unused11)(); // CloseEvent()
  EFI_STATUS (*unused12)(); // CheckEvent()

  // Protocol Handler Services
  EFI_STATUS (*unused13)(); // InstallProtocolInterface()
  EFI_STATUS (*unused14)(); // ReinstallProtocolInterface()
  EFI_STATUS (*unused15)(); // UninstallProtocolInterface()
  EFI_STATUS (*unused16)(); // HandleProtocol()
  VOID* Reserved;
  EFI_STATUS (*unused17)(); // RegisterProtocolNotify()
  EFI_STATUS (*unused18)(); // LocateHandle()
  EFI_STATUS (*unused19)(); // LocateDevicePath()
  EFI_STATUS (*unused20)(); // InstallConfigurationTable()

  // Image Services
  EFI_STATUS (*unused21)(); // LoadImage()
  EFI_STATUS (*unused22)(); // StartImage()
  EFI_STATUS (*unused23)(); // Exit()
  EFI_STATUS (*unused24)(); // UnloadImage()
  EFI_STATUS (*ExitBootServices)(EFI_HANDLE, UINTN);

  // Miscellaneous Services
  EFI_STATUS (*unused26)(); // GetNextMonotonicCount()
  EFI_STATUS (*unused27)(); // Stall()
  EFI_STATUS (*unused28)(); // SetWatchdogTimer()

  // DriverSupport Services
  EFI_STATUS (*unused29)(); // ConnectController()
  EFI_STATUS (*unused30)(); // DisconnectController()

  // Open & Close Protocol Services
  EFI_STATUS (*OpenProtocol)(EFI_HANDLE, EFI_GUID *, VOID**, EFI_HANDLE, EFI_HANDLE, UINT32);
  EFI_STATUS (*CloseProtocol)(EFI_HANDLE, EFI_GUID *, EFI_HANDLE, EFI_HANDLE);
  EFI_STATUS (*unused33)(); // OpenProtocolInformation()

  // Library Services
  EFI_STATUS (*ProtocolsPerHandle)(EFI_HANDLE, EFI_GUID ***, UINTN *);
  EFI_STATUS (*unused35)(); // LocateHandleBuffer()
  EFI_STATUS (*unused36)(); // LocateProtocol()
  EFI_STATUS (*unused37)(); // InstallMultipleProtocolInterfaces()
  EFI_STATUS (*unused38)(); // UninstallMultipleProtocolInterfaces()
  
  // 32-bit CRC Services
  EFI_STATUS (*unused39)(); // CalculateCrc32()

  // Miscellaneous Services
  EFI_STATUS (*unused40)(); // CopyMem()
  EFI_STATUS (*unused41)(); // SetMem()
  EFI_STATUS (*unused42)(); // CreateEventEx()
} EFI_BOOT_SERVICES;

// UEFI Spec. V2.9, Section 4.3
typedef struct {
  EFI_TABLE_HEADER Hdr;
  CHAR16 *FirmwareVendor;
  UINT32 FirmwareRevision;
  EFI_HANDLE ConsoleInHandle;
  EFI_SIMPLE_TEXT_INPUT_PROTOCOL *ConsoleIn;
  EFI_HANDLE ConsoleOutHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConsoleOut;
  EFI_HANDLE StandardErrorHandle;
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StandardError;
  VOID *unused8;  // RuntimeServices
  EFI_BOOT_SERVICES *BootServices;
  UINTN NumberOfTableEntries;
  VOID *unused11; // ConfigurationTable
} EFI_SYSTEM_TABLE;

#endif /* RADII_BOOTLOADER_SYSTEM_TABLE */
