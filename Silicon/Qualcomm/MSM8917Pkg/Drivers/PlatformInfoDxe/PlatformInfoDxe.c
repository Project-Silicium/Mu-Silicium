/* Copyright (c) 2012-2016, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>

#include <Device/Baseband.h>

#include <Protocol/QcomSmem.h>
#include <Protocol/QcomPlatformInfo.h>

STATIC UINT8 FormatMajor = 0;
STATIC UINT8 FormatMinor = 0;

struct BoardData Board = {
  UNKNOWN,
  0,
  0,
  0,
  HW_PLATFORM_UNKNOWN,
  HW_PLATFORM_SUBTYPE_UNKNOWN,
  LINUX_MACHTYPE_UNKNOWN,
  BASEBAND_MSM,
  {{PMIC_IS_INVALID, 0, 0},
  {PMIC_IS_INVALID, 0, 0},
  {PMIC_IS_INVALID, 0, 0}},
  0,
  0,
  0,
  NULL,
};

UINT32
GetPlatformID()
{
  return Board.Platform;
}

UINT32
GetFoundryID()
{
  return Board.FoundryID;
}

UINT32
GetChipSerialNumber()
{
  return Board.ChipSerial;
}

UINT32
GetPlatformVersion()
{
  return Board.PlatformVersion;
}

UINT32
GetPlatformHW()
{
  return Board.PlatformHW;
}

UINT32
GetPlatformSubtype()
{
  return Board.PlatformSubtype;
}

UINT32
GetTarget()
{
  return Board.Target;
}

UINT32
GetBaseband()
{
  return Board.Baseband;
}

UINT32
GetPlatformHlosSubtype()
{
  return Board.PlatformHlosSubtype;
}

UINT32
GetNumPmic()
{
  return Board.NumPmics;
}

UINT32
GetPmicArrayOffset()
{
  return Board.PmicArrayOffset;
}

STATIC QCOM_PLATFORM_INFO_PROTOCOL mPlatformInfo = {
  GetPlatformID,
  GetFoundryID,
  GetChipSerialNumber,
  GetPlatformVersion,
  GetPlatformHW,
  GetPlatformSubtype,
  GetTarget,
  GetBaseband,
  GetPlatformHlosSubtype,
  GetNumPmic,
  GetPmicArrayOffset
};

STATIC
EFI_STATUS
GetBoardInformation()
{
  EFI_STATUS          Status        = EFI_SUCCESS;
  QCOM_SMEM_PROTOCOL *mSmemProtocol = NULL;
  UINT32              BoardInfoLen  = 0;
  UINT32              Format        = 0;
  UINT32              PmicType      = 0;
  UINT8               i             = 0;

  struct SmemBoardInfoV6  BoardInfoV6;
  struct SmemBoardInfoV7  BoardInfoV7;
  struct SmemBoardInfoV8  BoardInfoV8;
  struct SmemBoardInfoV9  BoardInfoV9;
  struct SmemBoardInfoV10 BoardInfoV10;
  struct SmemBoardInfoV11 BoardInfoV11;

  // Locate Smem Protocol
  Status = gBS->LocateProtocol(&gQcomSmemProtocolGuid, NULL, (VOID *)&mSmemProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Locate Smem Protocol! Status = %r\n", Status));
    goto exit;
  }

  Status = mSmemProtocol->ReadSmemAllocationOffset(SMEM_BOARD_INFO_LOCATION, &Format, sizeof(Format), 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Read Smem Allocation Offset! Status = %r\n"));
    goto exit;
  }

  //
  // Extract the Major & Minor Version Info.
  // Upper two Bytes: Major Info
  // Lower two Bytes: Minor Info
  //
  FormatMajor = (Format & 0xFFFF0000) >> 16;
  FormatMinor = Format & 0xFFFF;

  if (FormatMajor == 0x0) {
    DEBUG ((EFI_D_WARN, "Board Info Version is 0x%x\n", FormatMinor));

    if (FormatMinor == 0x6) {
      BoardInfoLen = sizeof(BoardInfoV6);

      Status = mSmemProtocol->ReadSmemAllocationEntry(SMEM_BOARD_INFO_LOCATION, &BoardInfoV6, BoardInfoLen);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Read Smem Allocation Entry! Status = %r\n", Status));
        goto exit;
      }

      Board.Platform        = BoardInfoV6.BoardInfoV3.MSMID;
      Board.PlatformVersion = BoardInfoV6.BoardInfoV3.MSMVersion;
      Board.PlatformHW      = BoardInfoV6.BoardInfoV3.HWPlatform;
      Board.PlatformSubtype = BoardInfoV6.PlatformSubtype;
    } else if (FormatMinor == 0x7) {
      BoardInfoLen = sizeof(BoardInfoV7);

      Status = mSmemProtocol->ReadSmemAllocationEntry(SMEM_BOARD_INFO_LOCATION, &BoardInfoV7, BoardInfoLen);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Read Smem Allocation Entry! Status = %r\n", Status));
        goto exit;
      }

      Board.Platform                = BoardInfoV7.BoardInfoV3.MSMID;
      Board.PlatformVersion         = BoardInfoV7.BoardInfoV3.MSMVersion;
      Board.PlatformHW              = BoardInfoV7.BoardInfoV3.HWPlatform;
      Board.PlatformSubtype         = BoardInfoV7.PlatformSubtype;
      Board.PmicInfo[0].PmicType    = BoardInfoV7.PmicType;
      Board.PmicInfo[0].PmicVersion = BoardInfoV7.PmicVersion;
    } else if (FormatMinor == 0x8) {
      BoardInfoLen = sizeof(BoardInfoV8);

      Status = mSmemProtocol->ReadSmemAllocationEntry(SMEM_BOARD_INFO_LOCATION, &BoardInfoV8, BoardInfoLen);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Read Smem Allocation Entry! Status = %r\n", Status));
        goto exit;
      }

      Board.Platform        = BoardInfoV8.BoardInfoV3.MSMID;
      Board.PlatformVersion = BoardInfoV8.BoardInfoV3.MSMVersion;
      Board.PlatformHW      = BoardInfoV8.BoardInfoV3.HWPlatform;
      Board.PlatformSubtype = BoardInfoV8.PlatformSubtype;

      //
      // Fill in Board.Target with VariantID Information.
      // Bit Num        |31  24 | 23   16            | 15   8             |7         0|
      // Board.Target = |subtype| plat_hw_ver major  | plat_hw_ver minor  |hw_platform|
      //
      Board.Target = (((BoardInfoV8.PlatformSubtype & 0xff) << 24) | (((BoardInfoV8.PlatformVersion >> 16) & 0xff) << 16) | ((BoardInfoV8.PlatformVersion & 0xff) << 8) | (BoardInfoV8.BoardInfoV3.HWPlatform & 0xff));

      for (i = 0; i < 3; i++) {
        Board.PmicInfo[i].PmicType    = BoardInfoV8.PmicInfo[i].PmicType;
        Board.PmicInfo[i].PmicVersion = BoardInfoV8.PmicInfo[i].PmicVersion;

        //
        // Fill in PmicBoardInfo with Omic Type and Pmic Version Information.
        // Bit Num         |31  24 | 23  16        | 15   8       |7        0|
        // PimcBoardInfo = |Unused | Major version | Minor version|PMIC_MODEL|
        //
        PmicType = BoardInfoV8.PmicInfo[i].PmicType == PMIC_IS_INVALID? 0 : BoardInfoV8.PmicInfo[i].PmicType;

        Board.PmicInfo[i].PmicTarget = (((BoardInfoV8.PmicInfo[i].PmicVersion >> 16) & 0xFF) << 16) | ((BoardInfoV8.PmicInfo[i].PmicVersion & 0xFF) << 8) | (PmicType & 0xFF);
      }
    } else if (FormatMinor == 0x9) {
      BoardInfoLen = sizeof(BoardInfoV9);

      Status = mSmemProtocol->ReadSmemAllocationEntry(SMEM_BOARD_INFO_LOCATION, &BoardInfoV9, BoardInfoLen);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Read Smem Allocation Entry! Status = %r\n", Status));
        goto exit;
      }

      Board.Platform        = BoardInfoV9.BoardInfoV3.MSMID;
      Board.PlatformVersion = BoardInfoV9.BoardInfoV3.MSMVersion;
      Board.PlatformHW      = BoardInfoV9.BoardInfoV3.HWPlatform;
      Board.PlatformSubtype = BoardInfoV9.PlatformSubtype;

      //
      // Fill in Board.Target with VariantID Information.
      // Bit Num        |31  24 | 23   16            | 15   8             |7         0|
      // Board.Target = |subtype| plat_hw_ver major  | plat_hw_ver minor  |hw_platform|
      //
      Board.Target = (((BoardInfoV9.PlatformSubtype & 0xff) << 24) | (((BoardInfoV9.PlatformVersion >> 16) & 0xff) << 16) | ((BoardInfoV9.PlatformVersion & 0xff) << 8) | (BoardInfoV9.BoardInfoV3.HWPlatform & 0xff));

      for (i = 0; i < 3; i++) {
        Board.PmicInfo[i].PmicType    = BoardInfoV9.PmicInfo[i].PmicType;
        Board.PmicInfo[i].PmicVersion = BoardInfoV9.PmicInfo[i].PmicVersion;

        //
        // Fill in PmicBoardInfo with Omic Type and Pmic Version Information.
        // Bit Num         |31  24 | 23  16        | 15   8       |7        0|
        // PimcBoardInfo = |Unused | Major version | Minor version|PMIC_MODEL|
        //
        PmicType = BoardInfoV9.PmicInfo[i].PmicType == PMIC_IS_INVALID? 0 : BoardInfoV9.PmicInfo[i].PmicType;

        Board.PmicInfo[i].PmicTarget = (((BoardInfoV9.PmicInfo[i].PmicVersion >> 16) & 0xFF) << 16) | ((BoardInfoV9.PmicInfo[i].PmicVersion & 0xFF) << 8) | (PmicType & 0xFF);
      }

      Board.FoundryID = BoardInfoV9.FoundryID;
    } else if (FormatMinor == 0xA) {
      BoardInfoLen = sizeof(BoardInfoV10);

      Status = mSmemProtocol->ReadSmemAllocationEntry(SMEM_BOARD_INFO_LOCATION, &BoardInfoV10, BoardInfoLen);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Read Smem Allocation Entry! Status = %r\n", Status));
        goto exit;
      }

      Board.Platform        = BoardInfoV10.BoardInfoV3.MSMID;
      Board.PlatformVersion = BoardInfoV10.BoardInfoV3.MSMVersion;
      Board.PlatformHW      = BoardInfoV10.BoardInfoV3.HWPlatform;
      Board.PlatformSubtype = BoardInfoV10.PlatformSubtype;

      //
      // Fill in Board.Target with VariantID Information.
      // Bit Num        |31  24 | 23   16            | 15   8             |7         0|
      // Board.Target = |subtype| plat_hw_ver major  | plat_hw_ver minor  |hw_platform|
      //
      Board.Target = (((BoardInfoV10.PlatformSubtype & 0xff) << 24) | (((BoardInfoV10.PlatformVersion >> 16) & 0xff) << 16) | ((BoardInfoV10.PlatformVersion & 0xff) << 8) | (BoardInfoV10.BoardInfoV3.HWPlatform & 0xff));

      for (i = 0; i < 3; i++) {
        Board.PmicInfo[i].PmicType    = BoardInfoV10.PmicInfo[i].PmicType;
        Board.PmicInfo[i].PmicVersion = BoardInfoV10.PmicInfo[i].PmicVersion;

        //
        // Fill in PmicBoardInfo with Omic Type and Pmic Version Information.
        // Bit Num         |31  24 | 23  16        | 15   8       |7        0|
        // PimcBoardInfo = |Unused | Major version | Minor version|PMIC_MODEL|
        //
        PmicType = BoardInfoV10.PmicInfo[i].PmicType == PMIC_IS_INVALID? 0 : BoardInfoV10.PmicInfo[i].PmicType;

        Board.PmicInfo[i].PmicTarget = (((BoardInfoV10.PmicInfo[i].PmicVersion >> 16) & 0xFF) << 16) | ((BoardInfoV10.PmicInfo[i].PmicVersion & 0xFF) << 8) | (PmicType & 0xFF);
      }

      Board.FoundryID  = BoardInfoV10.FoundryID;
      Board.ChipSerial = BoardInfoV10.ChipSerial;
    } else if (FormatMinor >= 0xB) {
      BoardInfoLen = sizeof(BoardInfoV11);

      Status = mSmemProtocol->ReadSmemAllocationEntry(SMEM_BOARD_INFO_LOCATION, &BoardInfoV11, BoardInfoLen);
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "Failed to Read Smem Allocation Entry! Status = %r\n", Status));
        goto exit;
      }

      Board.Platform        = BoardInfoV11.BoardInfoV3.MSMID;
      Board.PlatformVersion = BoardInfoV11.BoardInfoV3.MSMVersion;
      Board.PlatformHW      = BoardInfoV11.BoardInfoV3.HWPlatform;
      Board.PlatformSubtype = BoardInfoV11.PlatformSubtype;

      //
      // Fill in Board.Target with VariantID Information.
      // Bit Num        |31  24 | 23   16            | 15   8             |7         0|
      // Board.Target = |subtype| plat_hw_ver major  | plat_hw_ver minor  |hw_platform|
      //
      Board.Target = (((BoardInfoV11.PlatformSubtype & 0xff) << 24) | (((BoardInfoV11.PlatformVersion >> 16) & 0xff) << 16) | ((BoardInfoV11.PlatformVersion & 0xff) << 8) | (BoardInfoV11.BoardInfoV3.HWPlatform & 0xff));

      for (i = 0; i < 3; i++) {
        Board.PmicInfo[i].PmicType    = BoardInfoV11.PmicInfo[i].PmicType;
        Board.PmicInfo[i].PmicVersion = BoardInfoV11.PmicInfo[i].PmicVersion;

        //
        // Fill in PmicBoardInfo with Omic Type and Pmic Version Information.
        // Bit Num         |31  24 | 23  16        | 15   8       |7        0|
        // PimcBoardInfo = |Unused | Major version | Minor version|PMIC_MODEL|
        //
        PmicType = BoardInfoV11.PmicInfo[i].PmicType == PMIC_IS_INVALID? 0 : BoardInfoV11.PmicInfo[i].PmicType;

        Board.PmicInfo[i].PmicTarget = (((BoardInfoV11.PmicInfo[i].PmicVersion >> 16) & 0xFF) << 16) | ((BoardInfoV11.PmicInfo[i].PmicVersion & 0xFF) << 8) | (PmicType & 0xFF);
      }

      Board.FoundryID       = BoardInfoV11.FoundryID;
      Board.ChipSerial      = BoardInfoV11.ChipSerial;
      Board.NumPmics        = BoardInfoV11.NumPmics;
      Board.PmicArrayOffset = BoardInfoV11.PmicArrayOffset;
    }
  } else {
    DEBUG ((EFI_D_ERROR, "Unsupported Board Info Format Detected!\n"));
    Status = EFI_UNSUPPORTED;
    goto exit;
  }

exit:
  return Status;
}

EFI_STATUS
EFIAPI
PlatformInfoInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;

  // Get Board Infos
  Status = GetBoardInformation();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "Failed to Get Board Infos! Status = %r\n", Status));
    goto exit;
  }

  // Register Platform Info Protocol in a new Handle
  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gPlatformInfoProtocolGuid, &mPlatformInfo, NULL);
  ASSERT_EFI_ERROR (Status);

exit:
  return Status;
}