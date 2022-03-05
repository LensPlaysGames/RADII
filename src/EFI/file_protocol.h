#ifndef RADII_BOOTLOADER_FILE_PROTOCOL_H
#define RADII_BOOTLOADER_FILE_PROTOCOL_H

#include "types.h"

#define EFI_FILE_INFO_ID \
  {0x9576e92,0x6d3f,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

#define EFI_FILE_SYSTEM_INFO_ID \
  {0x09576e93,0x6d3f,0x11d2,{0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}}

#define EFI_FILE_SYSTEM_VOLUME_LABEL_ID \
  {0xdb47d7d3,0xfe81,0x11d3,0x9a35, {0x00,0x90,0x27,0x3f,0xC1,0x4d}}

#define EFI_FILE_MODE_READ    0x0000000000000001
#define EFI_FILE_MODE_WRITE   0x0000000000000002
#define EFI_FILE_MODE_CREATE  0x8000000000000000

#define EFI_FILE_READ_ONLY   0x0000000000000001
#define EFI_FILE_HIDDEN      0x0000000000000002
#define EFI_FILE_SYSTEM      0x0000000000000004
#define EFI_FILE_RESERVED    0x0000000000000008
#define EFI_FILE_DIRECTORY   0x0000000000000010
#define EFI_FILE_ARCHIVE     0x0000000000000020
#define EFI_FILE_VALID_ATTR  0x0000000000000037

#define EFI_FILE_PROTOCOL_REVISION   0x00010000
#define EFI_FILE_PROTOCOL_REVISION2  0x00020000
#define EFI_FILE_PROTOCOL_LATEST_REVISION EFI_FILE_PROTOCOL_REVISION2

typedef struct {
  EFI_EVENT Event;
  EFI_STATUS Status;
  UINTN BufferSize;
  VOID *Buffer;
} EFI_FILE_IO_TOKEN;

typedef struct {
  UINT64 Size;
  UINT64 FileSize;
  UINT64 PhysicalSize;
  EFI_TIME CreateTime;
  EFI_TIME LastAccessTime;
  EFI_TIME ModificationTime;
  UINT64 Attribute;
  CHAR16 FileName[260]; // Supposed to be dynamic array, but that's a pain.
} EFI_FILE_INFO;

typedef struct {
  UINT64 Size;
  BOOLEAN ReadOnly;
  UINT64 VolumeSize;
  UINT64 FreeSpace;
  UINT32 BlockSize;
  CHAR16 VolumeLabel[260]; // Supposed to be dynamic array, but that's a pain.
} EFI_FILE_SYSTEM_INFO;

typedef struct {
  CHAR16 VolumeLabel[260]; // Supposed to be dynamic array, but that's a pain.
} EFI_FILE_SYSTEM_VOLUME_LABEL;

typedef struct _EFI_FILE_PROTOCOL {
  UINT64 Revision;
  EFI_STATUS (*Open)(struct _EFI_FILE_PROTOCOL *This
                     , struct _EFI_FILE_PROTOCOL **NewHandle
                     , CHAR16 *FileName
                     , UINT64 OpenMode
                     , UINT64 Attributes);
  EFI_STATUS (*Close)(struct _EFI_FILE_PROTOCOL *This);
  EFI_STATUS (*Delete)(struct _EFI_FILE_PROTOCOL *This);
  EFI_STATUS (*Read)(struct _EFI_FILE_PROTOCOL *This
                     , UINTN *BufferSize
                     , VOID *Buffer);
  EFI_STATUS (*Write)(struct _EFI_FILE_PROTOCOL *This
                      , UINTN *BufferSize
                      , VOID *Buffer);
  EFI_STATUS (*GetPosition)(struct _EFI_FILE_PROTOCOL *This
                            , UINT64 *Position);
  EFI_STATUS (*SetPosition)(struct _EFI_FILE_PROTOCOL *This
                            , UINT64 Position);
  EFI_STATUS (*GetInfo)(struct _EFI_FILE_PROTOCOL *This
                        , EFI_GUID *InformationType
                        , UINTN *BufferSize
                        , VOID *Buffer);
  EFI_STATUS (*SetInfo)(struct _EFI_FILE_PROTOCOL *This
                        , EFI_GUID *InformationType
                        , UINTN BufferSize
                        , VOID *Buffer);
  EFI_STATUS (*Flush)(struct _EFI_FILE_PROTOCOL *This);
  EFI_STATUS (*OpenEx)(struct _EFI_FILE_PROTOCOL *This
                       , struct _EFI_FILE_PROTOCOL **NewHandle
                       , CHAR16 *FileName
                       , UINT64 OpenMode
                       , UINT64 Attributes
                       , EFI_FILE_IO_TOKEN *Token);
  EFI_STATUS (*ReadEx)(struct _EFI_FILE_PROTOCOL *This
                       , EFI_FILE_IO_TOKEN *Token);
  EFI_STATUS (*WriteEx)(struct _EFI_FILE_PROTOCOL *This
                       , EFI_FILE_IO_TOKEN *Token);
  EFI_STATUS (*FlushEx)(struct _EFI_FILE_PROTOCOL *This
                       , EFI_FILE_IO_TOKEN *Token);
} EFI_FILE_PROTOCOL;

// This is done by gnu-efi, so I'll include it for compatibility sake.
typedef EFI_FILE_PROTOCOL EFI_FILE;

#endif /* RADII_BOOTLOADER_FILE_PROTOCOL_H */
