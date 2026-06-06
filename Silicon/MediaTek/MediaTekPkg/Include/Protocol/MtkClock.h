#ifndef _MTK_CLOCK_H_
#define _MTK_CLOCK_H_

//
// Declare forward Reference to the Clock Protocol
//
typedef struct _MTK_CLOCK_PROTOCOL MTK_CLOCK_PROTOCOL;

/**
  This Function retrieves clock id by clock name.

  @param[in]  ClockName                 - Name of the clock.
  @param[out] ClockId                   - Id of the clock.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_CLOCK_GET_ID) (
  IN  CONST CHAR8 *ClockName,
  OUT UINT32 *ClockId
  );

/**
  This Function retrieves clock name by clock id.

  @param[in]  ClockId                   - Id of the clock.
  @param[out] ClockName                 - Name of the clock.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_CLOCK_GET_NAME) (
  IN  UINT32 ClockId,
  OUT CONST CHAR8 **ClockName
  );

/**
  This Function enables or disables a clock.

  @param[in] ClockId                    - Id of the clock.
  @param[in] Enable                     - TRUE to enable the clock, FALSE to disable it.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_CLOCK_SET_ENABLE) (
  IN UINT32 ClockId,
  IN BOOLEAN Enable
  );

/**
  This Function retrieves the enable state of a clock.

  @param[in]  ClockId                   - Id of the clock.
  @param[out] Enabled                   - TRUE if the clock is enabled.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_CLOCK_IS_ENABLED) (
  IN  UINT32 ClockId,
  OUT BOOLEAN *Enabled
  );

/**
  This Function retrieves current frequency of a clock.

  @param[in]  ClockId                   - Id of the clock.
  @param[out] Frequency                 - Current clock frequency in Hz.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_CLOCK_GET_FREQ) (
  IN  UINT32 ClockId,
  OUT UINT64 *Frequency
  );

/**
  This Function configures the clock frequency.

  @param[in] ClockId                    - Id of the clock.
  @param[in] Frequency                  - Desired clock frequency in Hz.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_CLOCK_SET_FREQ) (
  IN UINT32 ClockId,
  IN UINT64 Frequency
  );


/**
  This Function retrieves current parent of a clock.

  @param[in]  ClockId                   - Id of the clock.
  @param[out] ParentId                  - Current parent clock id.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_CLOCK_GET_PARENT) (
  IN  UINT32 ClockId,
  OUT UINT32 *ParentId
  );

/**
  This Function selects a new parent clock.

  @param[in] ClockId                    - Id of the clock.
  @param[in] ParentId                   - New parent clock id.
**/
typedef
EFI_STATUS
(EFIAPI *MTK_CLOCK_SET_PARENT) (
  IN UINT32 ClockId,
  IN UINT32 ParentId
  );

//
// Define Protocol Functions
//
struct _MTK_CLOCK_PROTOCOL {
  MTK_CLOCK_GET_ID GetId;
  MTK_CLOCK_GET_NAME GetName;
  MTK_CLOCK_SET_ENABLE SetEnable;
  MTK_CLOCK_IS_ENABLED IsEnabled;
  MTK_CLOCK_GET_FREQ  GetFrequency;
  MTK_CLOCK_SET_FREQ SetFrequency;
  MTK_CLOCK_GET_PARENT GetParent;
  MTK_CLOCK_SET_PARENT SetParent;
};

extern EFI_GUID gMediaTekClockProtocolGuid;

#endif /* _MTK_CLOCK_H_ */