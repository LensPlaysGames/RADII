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

#define SAL_SYSTEM_TABLE_GUID \
  {0xeb9d2d32,0x2d88,0x11d3, {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define SMBIOS_TABLE_GUID \
  {0xeb9d2d31,0x2d88,0x11d3, {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define SMBIOS3_TABLE_GUID \
  {0xf2fd1544, 0x9794, 0x4a2c, {0x99,0x2e,0xe5,0xbb,0xcf,0x20,0xe3,0x94})

#define MPS_TABLE_GUID \
  {0xeb9d2d2f,0x2d88,0x11d3, {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

// ACPI 2.0 or newer tables should use EFI_ACPI_TABLE_GUID
#define EFI_ACPI_TABLE_GUID \
  {0x8868e871,0xe4f1,0x11d3, {0xbc,0x22,0x00,0x80,0xc7,0x3c,0x88,0x81}}

#define EFI_ACPI_20_TABLE_GUID EFI_ACPI_TABLE_GUID

#define ACPI_TABLE_GUID \
  {0xeb9d2d30,0x2d88,0x11d3, {0x9a,0x16,0x00,0x90,0x27,0x3f,0xc1,0x4d}}

#define ACPI_10_TABLE_GUID ACPI_TABLE_GUID

#define EFI_JSON_CONFIG_DATA_TABLE_GUID \
  {0x87367f87, 0x1119, 0x41ce, {0xaa, 0xec, 0x8b, 0xe0, 0x11, 0x1f, 0x55, 0x8a}}

#define EFI_JSON_CAPSULE_DATA_TABLE_GUID \
  {0x35e7a725, 0x8dd2, 0x4cac, { 0x80, 0x11, 0x33, 0xcd, 0xa8, 0x10, 0x90, 0x56}}

#define EFI_JSON_CAPSULE_RESULT_TABLE_GUID \
  {0xdbc461c3, 0xb3de, 0x422a, {0xb9, 0xb4, 0x98, 0x86, 0xfd, 0x49, 0xa1, 0xe5}}

#define EFI_DTB_TABLE_GUID \
  {0xb1b621d5, 0xf19c, 0x41a5, {0x83, 0x0b, 0xd9, 0x15, 0x2c, 0x69, 0xaa, 0xe0}}

typedef struct {
  EFI_TABLE_HEADER Hdr;

  // Task Priority Services
  void (*unused0)(); // RaiseTPL()
  void (*unused1)(); // RestoreTPL()

  // Memory Services
  EFI_STATUS (*AllocatePages)(EFI_ALLOCATE_TYPE Type
                              , EFI_MEMORY_TYPE MemoryType
                              , UINTN Pages
                              , UINT64 *Memory);
  EFI_STATUS (*FreePages)(UINT64 Memory
                          , UINTN Pages);
  EFI_STATUS (*GetMemoryMap)(UINTN *MemoryMapSize
                             , EFI_MEMORY_DESCRIPTOR *MemoryMap
                             , UINTN *MapKey
                             , UINTN *DescriptorSize
                             , UINT32 *DescriptorVersion);
  EFI_STATUS (*AllocatePool)(EFI_MEMORY_TYPE MemoryType
                             , UINTN SizeInBytes
                             , VOID** Buffer);
  EFI_STATUS (*FreePool)(VOID* Buffer);

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
  EFI_STATUS (*HandleProtocol)(EFI_HANDLE Handle
							   , EFI_GUID *Protocol
							   , VOID **Interface);
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
  EFI_STATUS (*ExitBootServices)(EFI_HANDLE ImageHandle
								 , UINTN MapKey);

  // Miscellaneous Services
  EFI_STATUS (*unused26)(); // GetNextMonotonicCount()
  EFI_STATUS (*Stall)(UINTN Microseconds);
  EFI_STATUS (*SetWatchdogTimer)(UINTN Timeout
                                 , UINT64 WatchdogCode
                                 , UINTN DataSize
                                 , CHAR16 *WatchdogData);

  // DriverSupport Services
  EFI_STATUS (*unused29)(); // ConnectController()
  EFI_STATUS (*unused30)(); // DisconnectController()

  // Open & Close Protocol Services
  EFI_STATUS (*OpenProtocol)(EFI_HANDLE Handle
							 , EFI_GUID *Protocol
							 , VOID **Interface
							 , EFI_HANDLE AgentHandle
							 , EFI_HANDLE ControllerHandle
							 , UINT32 Attributes);
  EFI_STATUS (*CloseProtocol)(EFI_HANDLE Handle
							  , EFI_GUID *Protocol
							  , EFI_HANDLE AgentHandle
							  , EFI_HANDLE ControllerHandle);
  EFI_STATUS (*unused33)(); // OpenProtocolInformation()

  // Library Services
  EFI_STATUS (*ProtocolsPerHandle)(EFI_HANDLE Handle
								   , EFI_GUID ***ProtocolBuffer
								   , UINTN *ProtocolBufferCount);
  EFI_STATUS (*unused35)(); // LocateHandleBuffer()
  EFI_STATUS (*LocateProtocol)(EFI_GUID *Protocol
							   , VOID *Registration
							   , VOID **Interface);
  EFI_STATUS (*unused37)(); // InstallMultipleProtocolInterfaces()
  EFI_STATUS (*unused38)(); // UninstallMultipleProtocolInterfaces()
  
  // 32-bit CRC Services
  EFI_STATUS (*CalculateCrc32)(VOID *Data
                               , UINTN DataSize
                               , UINT32 *Crc32);

  // Miscellaneous Services
  EFI_STATUS (*CopyMem)(VOID *Destination
                        , VOID *Source
                        , UINTN Length);
  EFI_STATUS (*SetMem)(VOID *Buffer
                       , UINTN Size
                       , UINT8 Value);
  EFI_STATUS (*unused42)(); // CreateEventEx()
} EFI_BOOT_SERVICES;

typedef struct {
  EFI_GUID VendorGuid;
  VOID *VendorTable;
} EFI_CONFIGURATION_TABLE;

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
  EFI_CONFIGURATION_TABLE *ConfigurationTable;
} EFI_SYSTEM_TABLE;

#endif /* RADII_BOOTLOADER_SYSTEM_TABLE */
