/**
  Copyright (C) Samsung Electronics Co. LTD

  This software is proprietary of Samsung Electronics.
  No part of this software, either material or conceptual may be copied or distributed, transmitted,
  transcribed, stored in a retrieval system or translated into any human or computer language in any form by any means,
  electronic, mechanical, manual or otherwise, or disclosed
  to third parties without the express written permission of Samsung Electronics.
**/

#ifndef _EFI_SPEEDY_H_
#define _EFI_SPEEDY_H_

// Speedy Access Modes
#define ACCESS_BURST     0
#define ACCESS_RANDOM    1

// Speedy Read Directions
#define DIRECTION_READ   0
#define DIRECTION_WRITE  1

//
// Global GUID for the ESpeedy Protocol
//
#define EFI_SPEEDY_PROTOCOL_GUID { 0x8504E67D, 0xB696, 0x445C, { 0xBD, 0x86, 0x99, 0x74, 0x85, 0xB9, 0x4C, 0xD3 } }

//
// Declare forward Reference to the Speedy Protocol
//
typedef struct _EFI_SPEEDY_PROTOCOL EFI_SPEEDY_PROTOCOL;

/**
  This Function Resets Speedy Fifo.

  @param[in] BaseAddress                   - The Speedy Base Address.
**/
typedef
VOID
(EFIAPI *EFI_RESET_FIFO) (
  IN UINT32 BaseAddress
  );

/**
  This Function Reads from Speedy.

  @param[in]  BaseAddress                  - The Speedy Base Address.
  @param[in]  Slave                        - The Slave Address.
  @param[in]  Addr                         - The Address.
  @param[out] Data                         - The Data of Speedy.

  @return EFI_SUCCESS                      - Successfully Read Requested Data.
  @return EFI_INVALID_PARAMETER            - Some Parameters are Invalid.
  @return EFI_TIMEOUT                      - Speedy did not Respond to the Command in Time.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_READ) (
  IN  UINT32 BaseAddress,
  IN  UINT8  Slave,
  IN  UINT8  Addr,
  OUT UINT8 *Data
  );

/**
  This Function Burst Reads from Speedy.

  @param[in]  BaseAddress                  - The Speedy Base Address.
  @param[in]  Slave                        - The Slave Address.
  @param[in]  Addr                         - The Address.
  @param[out] Data                         - The Data of Speedy.
  @param[in]  Count                        - The Count of Reads.

  @return EFI_SUCCESS                      - Successfully Read Requested Data.
  @return EFI_INVALID_PARAMETER            - Some Parameters are Invalid.
  @return EFI_TIMEOUT                      - Speedy did not Respond to the Command in Time.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_BURST_READ) (
  IN  UINT32 BaseAddress,
  IN  UINT8  Slave,
  IN  UINT8  Addr,
  OUT UINT8 *Data,
  IN  UINT8  Count
  );

/**
  This Function Writes to Speedy.

  @param[in] BaseAddress                   - The Speedy Base Address.
  @param[in] Slave                         - The Slave Address.
  @param[in] Addr                          - The Address.
  @param[in] Data                          - The Data for Speedy.

  @return EFI_SUCCESS                      - Successfully Wrote Data.
  @return EFI_INVALID_PARAMETER            - Some Parameters are Invalid.
  @return EFI_TIMEOUT                      - Speedy did not Respond to the Command in Time.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_WRITE) (
  IN  UINT32 BaseAddress,
  IN  UINT8  Slave,
  IN  UINT8  Addr,
  IN  UINT8  Data
  );

/**
  This Function Burst Writes to Speedy.

  @param[in] BaseAddress                   - The Speedy Base Address.
  @param[in] Slave                         - The Slave Address.
  @param[in] Addr                          - The Address.
  @param[in] Data                          - The Data for Speedy.
  @param[in] Count                         - The Count of Writes.

  @return EFI_SUCCESS                      - Successfully Wrote Data.
  @return EFI_INVALID_PARAMETER            - Some Parameters are Invalid.
  @return EFI_TIMEOUT                      - Speedy did not Respond to the Command in Time.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_BURST_WRITE) (
  IN  UINT32 BaseAddress,
  IN  UINT8  Slave,
  IN  UINT8  Addr,
  IN  UINT8 *Data,
  IN  UINT8  Count
  );

//
// Define Protocol Functions
//
struct _EFI_SPEEDY_PROTOCOL {
  EFI_RESET_FIFO  ResetFifo;
  EFI_READ        Read;
  EFI_BURST_READ  BurstRead;
  EFI_WRITE       Write;
  EFI_BURST_WRITE BurstWrite;
};

extern EFI_GUID gEfiSpeedyProtocolGuid;

#endif /* _EFI_SPEEDY_H_ */
