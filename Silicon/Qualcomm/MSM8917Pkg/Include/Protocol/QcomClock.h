/* Copyright (c) 2013-2014, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of The Linux Foundation, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
 */
#ifndef _QCOM_CLOCK_H_
#define _QCOM_CLOCK_H_

#include <Device/Clock.h>

//
// Global ID for the Gpa Protocol
//
#define QCOM_CLOCK_PROTOCOL_GUID { 0x68bd657b, 0x24ed, 0x4e90, { 0x91, 0x05, 0xeb, 0xfe, 0xda, 0x62, 0x54, 0xf3 } }

//
// Declare forward reference to the Gpa Protocol
//
typedef struct _QCOM_CLOCK_PROTOCOL QCOM_CLOCK_PROTOCOL;

/**
  This Function Sets The Parent of the defined Clock.

  @param clk          The Clock.
  @param parent       The Parent.

  @return EFI_SUCCESS The Clock Parent has been set Successfully.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_SET_CLOCK_PARENT)(
  struct clk *clk, 
  struct clk *parent
  );

/**
  This Function Gets The Parent of the defined Clock.

  @param clk          The Clock.

  @return parent      The Parent of the defined Clock.

typedef 
INT32
(EFIAPI *EFI_GET_CLOCK_PARENT)(
  struct clk *clk
  );
*/

/**
  This Function Resets The defined Clock.

  @param clk          The Clock.
  @param action       The Reset Type.

  @return EFI_SUCCESS The Clock has been reset Successfully.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_RESET_CLOCK)(
  struct clk *clk,
  enum clk_reset_action action
  );

/**
  This Function Disables The defined Clock.

  @param clk          The Clock.

  @return EFI_SUCCESS The Clock has been disabled Successfully.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_DISABLE_CLOCK)(
  struct clk *clk
  );

/**
  This Function Enables The defined Clock.

  @param clk          The Clock.

  @return EFI_SUCCESS The Clock has been enabled Successfully.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_ENABLE_CLOCK)(
  struct clk *clk
  );

/**
  This Function Gets the Current Clock Rate of the defined Clock.

  @param clk          The Clock.

  @return ClockRate   The Clock Rate.
**/
typedef 
UINT32
(EFIAPI *EFI_GET_CLOCK_RATE)(
  struct clk *clk
  );

/**
  This Function Sets the Clock Rate of the defined Clock.

  @param clk          The Clock.
  @param ClockRate    The Clock Rate.

  @return EFI_SUCCESS The Clock Rate has been set Successfully.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_SET_CLOCK_RATE)(
  struct clk *clk,
  UINT32 ClockRate
  );

/**
  This Function Gets the defined Clock.

  @param cid          The Clock ID.

  @return Clock       The Clock.

typedef 
UINT32
(EFIAPI *EFI_GET_CLOCK)(
  CONST CHAR16 *cid
  );
*/

/**
  This Function Sets the defined Clock.

  @param id           The Clock ID.
  @param ClockRate    The ClockRate to Set.
  @param Enable       The bool Variable to Enable the Clock.

  @return EFI_SUCCESS The Clock has been set Successfully.
**/
typedef 
EFI_STATUS
(EFIAPI *EFI_SET_CLOCK)(
  CHAR16 *id,
  UINT32  ClockRate,
  BOOLEAN Enable
  );

//
// Define Protocol Functions
//
struct _QCOM_CLOCK_PROTOCOL {
  EFI_SET_CLOCK_PARENT        SetClockParent;
  //EFI_GET_CLOCK_PARENT        GetClockParent;
  EFI_RESET_CLOCK             ResetClock;
  EFI_DISABLE_CLOCK           DisableClock;
  EFI_ENABLE_CLOCK            EnableClock;
  EFI_GET_CLOCK_RATE          GetClockRate;
  EFI_SET_CLOCK_RATE          SetClockRate;
  //EFI_GET_CLOCK               GetClock;
  EFI_SET_CLOCK               SetClock;
};

extern EFI_GUID gQcomClockProtocolGuid;

#endif /* _QCOM_GPIO_TLMM_H_ */