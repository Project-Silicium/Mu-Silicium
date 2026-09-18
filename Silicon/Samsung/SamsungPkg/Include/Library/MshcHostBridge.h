/**
  Copyright (c) 2012, Samsung Electronics Co. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _MSHC_HOST_BRIDGE_H_
#define _MSHC_HOST_BRIDGE_H_

//
//  Clock Selection Timing Bits
//
#define CLKSEL_TIMING(Div, FineDrive, Drive, Sample) \
          ((((Div) & 0x7) << 24) | (((FineDrive) & 0x3) << 22) | \
           (((Drive) & 0x7) << 16) | ((Sample) & 0x7))

typedef struct {
  // SoC specific
  UINTN  BaseAddress;
  UINT32 ClkSelTiming;
  UINT32 ClkSelTimingSdr50;
  UINT32 ClkSelTimingSdr104;
  UINT8  ClkSelDivider;
  UINTN  CiuClockHz;

  // For boards that may not have iocoherency.
  BOOLEAN Coherent;

  // For eMMC.
  UINT8  BusWidth;

  // Read out of the Controller
  UINTN  FifoOffset;
  UINTN  FifoItemBytes;
  UINTN  ClkSelOffset;
} MSHC_HOST;

/**
 * Board specific initialization.
 * @param[in,out] Host           The Host which needs the board specific init.
 *
 * @retval EFI_SUCCESS           The Board has been initialized successfully.
 * @retval EFI_UNSUPPORTED       The Board cannot be initialized.
 **/
EFI_STATUS
MshcBoardInit (
  IN OUT MSHC_HOST *Host
  );

/**
 * Sets the Clock Frequency of the CIU.
 *
 * @param[in,out] Host           The Host whose CIU Clock is being set.
 * @param[in]     Frequency      The desired Clock Frequency.
 *
 * @retval EFI_SUCCESS           The CIU Clock is now at the Requested Frequency.
 * @retval EFI_UNSUPPORTED       The Board cannot Produce that Frequency.
 **/
EFI_STATUS
MshcBoardSetCiuClock (
  IN OUT MSHC_HOST *Host,
  IN     UINTN      Frequency
  );

/**
 * Board specific post reset hook.
 * @param[in,out] Host           The Host which needs the post reset handling.
 *
 * @retval None
 **/
VOID
MshcBoardPostReset (
  IN OUT MSHC_HOST *Host
  );

/**
  Sets the Voltage the Bus Signals at.

  A Card Starts at 3.3V and only Moves to 1.8V as part of the Switch into an
  Ultra High Speed Mode, which the Card and the Host have to do Together.

  @param[in,out] Host           The Host whose Bus Supply is Moving.
  @param[in]     Millivolts     Either 3300 or 1800.

  @retval EFI_SUCCESS           The Supply is now at the Requested Voltage.
  @retval EFI_UNSUPPORTED       The Board cannot Produce that Voltage.
**/
EFI_STATUS
MshcBoardSetVoltage (
  IN OUT MSHC_HOST *Host,
  IN     UINT32     Millivolts
  );

/**
  Takes Power away from the Card and Gives it back.

  This is the only way out of a Half Finished Voltage Switch, where the Card
  and the Host have stopped Agreeing about what the Bus is doing. On Return the
  Card is Unpowered no Longer but Un-Initialised, so the Caller has to Run
  Identification again from the Beginning.

  @param[in,out] Host           The Host whose Card is being Power Cycled.

  @retval EFI_SUCCESS           The Card has been Powered back up.
**/
EFI_STATUS
MshcBoardPowerCycle (
  IN OUT MSHC_HOST *Host
  );

/*
 * Checks if a Card is Present in the Slot.
 *
 * @retval TRUE                    A Card is Present.
 * @retval FALSE                   No Card is Present.
 */
BOOLEAN
MshcBoardIsCardPresent (
  VOID
  );

/*
 * Checks if the Card is Read-Only.
 *
 * @retval TRUE                    The Card is Read-Only.
 * @retval FALSE                   The Card is not Read-Only.
 */
BOOLEAN
MshcBoardIsReadOnly (
  VOID
  );

#endif /* _MSHC_HOST_BRIDGE_H_ */
