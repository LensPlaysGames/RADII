#ifndef RADII_BOOTLOADER_GRAPHICS_OUTPUT_PROTOCOL_H
#define RADII_BOOTLOADER_GRAPHICS_OUTPUT_PROTOCOL_H

#include "types.h"

// UEFI Spec. V2.9 Section 12.9 Graphics Output Protocol

#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID \
  {0x9042a9de,0x23dc,0x4a38, {0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}}

typedef struct {
  UINT8 Blue;
  UINT8 Green;
  UINT8 Red;
  UINT8 Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;

typedef enum {
  EfiBltVideoFill,
  EfiBltVideoToBltBuffer,
  EfiBltBufferToVideo,
  EfiBltVideoToVideo,
  EfiGraphicsOutputBltOperationMax,
} EFI_GRAPHICS_OUTPUT_BLT_OPERATION;

typedef struct {
  UINT32 RedMask;
  UINT32 GreenMask;
  UINT32 BlueMask;
  UINT32 ReservedMask;
} EFI_PIXEL_BITMASK;

typedef enum {
  PixelRedGreenBlueReserved8BitPerColor,
  PixelBlueGreenRedReserved8BitPerColor,
  PixelBitMask,
  PixelBltOnly,
  PixelFormatMax,
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
  UINT32 Version;
  UINT32 HorizontalResolution;
  UINT32 VerticalResolution;
  EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
  EFI_PIXEL_BITMASK PixelInformation;
  UINT32 PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE_INFORMATION;

typedef struct {
  UINT32 MaxMode;
  UINT32 Mode;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE_INFORMATION *Info;
  UINTN SizeOfInfo;
  UINT64 FrameBufferBase;
  UINTN FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
  EFI_STATUS (*QueryMode)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This
						  , UINT32 ModeNumber
						  , UINTN *SizeOfInfo
						  , EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE_INFORMATION **Info);
  EFI_STATUS (*SetMode)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This
						, UINT32 ModeNumber);
  EFI_STATUS (*Blt)(EFI_GRAPHICS_OUTPUT_PROTOCOL *This
					, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer
					, EFI_GRAPHICS_OUTPUT_BLT_OPERATION BltOperation
					, UINTN SourceX
					, UINTN SourceY
					, UINTN DestinationX
					, UINTN DestinationY
					, UINTN Width
					, UINTN Height
					, UINTN Delta);
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

#endif /* RADII_BOOTLOADER_GRAPHICS_OUTPUT_PROTOCOL_H */
