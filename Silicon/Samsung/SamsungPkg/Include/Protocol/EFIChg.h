#ifndef _EFI_CHG_H_
#define _EFI_CHG_H_

//
// Global GUID for the Chg Protocol
//
#define EFI_CHG_PROTOCOL_GUID { 0x3C7596FA, 0x0058, 0x4496, { 0xA6, 0x48, 0x6D, 0x14, 0xBE, 0x40, 0x23, 0xB2 } }

//
// Declare forward reference to the Chg Protocol
//
typedef struct _EFI_CHG_PROTOCOL EFI_CHG_PROTOCOL;

//
// Charger Connection Types
//
#define CHARGER_CONNECTION_TYPE_DISCONNECTED 0
#define CHARGER_CONNECTION_TYPE_CONNECTED    1

/**
  This Function sets the Charging Current.

  @param[in] Value1                        - Unknown.
  @param[in] Value2                        - Unknown.

  @return EFI_SUCCESS                      - The Charging Current was set Successfully.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_CHARGING_CURRENT) (
  IN UINT32 Value1,
  IN UINT32 Value2
  );

/**
  This Function sets the Charging Current using the defined Type.

  @param[in] Type                          - Unknown.

  @return EFI_SUCCESS                      - The Charging Current was set Successfully.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_CHARGING_CURRENT_BY_TYPE) (
  IN UINT32 Type
  );

/**
  This Function sets the Input Current.

  @param[in] Value1                        - Unknown.
  @param[in] Value2                        - Unknown.

  @return EFI_SUCCESS                      - The Input Current was set Successfully.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_INPUT_CURRENT) (
  IN UINT32 Value1,
  IN UINT32 Value2
  );

/**
  This Function enables Charging Handling.

  @param[in] Value1                        - Unknown.
  @param[in] Value2                        - Unknown.

  @return EFI_SUCCESS                      - Successfully enabled Charging Handling.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_ENABLE_CHARGING) (
  IN UINT32 Value1,
  IN UINT32 Value2
  );

/**
  This Function Reconfigures Charging.

  @return EFI_SUCCESS                      - Successfully Reconfigured Charging.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_CHARGER_RECONFIG) ();

/**
  This Function checks the Battery Status.

  @return EFI_SUCCESS                      - Successfully Checked Battery Status.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_CHECK_BATTERY) ();

/**
  This Functions gets the Current Connection Type.

  @param[out] ConnectionType               - The Current Connection Type.

  @return EFI_SUCCESS                      - Successfully got the Connection Type.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_CHARGER_STATUS) (
  OUT UINT32 *ConnectionType
  );

/**
  This Function sets the Charger Watchdog Timer Value.

  @param[in] Time                          - The Watchdog Timer Value.

  @return EFI_SUCCESS                      - Successfully set the new Watchdog Timer Value.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_CHARGER_WATCHDOG_TIMER) (
  IN UINT32 Time
  );

/**
  This Function Executes a Watchdog Timer Kick.

  @return EFI_SUCCESS                      - Successfully executed a Watchdog Timer Kick.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_CHARGER_WATCHDOG_KICK) ();

/**
  This Function gets the Factory Mode.

  @param[out] Mode                         - The Factory Mode.

  @return EFI_SUCCESS                      - Successfully got the Factory Mode.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_FACTORY_MODE) (
  OUT UINT8 *Mode
  );

/**
  This Function gets the Factory Siso Mode.

  @param[out] Mode                         - The Factory Siso Mode.

  @return EFI_SUCCESS                      - Successfully got the Factory Siso Mode.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_FACTORY_SISO_MODE) (
  OUT UINT8 *SizoMode
  );

/**
  This Function Inits the Dev Charging.

  @return EFI_SUCCESS                      - Successfully Init Dev Charging.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_INIT_DEV_CHARGING) ();

/**
  This Function reads the Fuel Gauge VCell.

  @param[out] Value                        - Unknown.

  @return EFI_SUCCESS                      - Successfully Read the Fuel Gauge VCell.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_READ_FUEL_GAUGE_VCELL) (
  OUT UINT32 *Value
  );

/**
  This Function gets the Current Battery Temperature.

  @param[out] Temperature                  - The Current Battery Temperature. (Unknown Unit)

  @return EFI_SUCCESS                      - Successfully got the Current Battery Temperature.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_BATTERY_TEMPERATURE) (
  OUT UINT32 *Temperature
  );

/**
  This Function checks the Fuel Gauge Model Data.

  @param[out] Value                        - Unknown.

  @return EFI_SUCCESS                      - Successfully checked the Fuel Gauge Model Data.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_CHECK_FUEL_GAUGE_MODEL_DATA) (
  OUT UINT32 *Value
  );

/**
  This Function Inits the Fuel Gauge.

  @return EFI_SUCCESS                      - Succesfully init the Fuel Gauge.
**/
typedef
EFI_STATUS
(EFIAPI *EFI_INIT_FUEL_GAUGE) ();

//
// Define Protocol Functions
//
struct _EFI_CHG_PROTOCOL {
  UINT32                           Revision;
  EFI_SET_CHARGING_CURRENT         SetChargingCurrent;
  EFI_SET_CHARGING_CURRENT_BY_TYPE SetChargingCurrentByType;
  EFI_SET_INPUT_CURRENT            SetInputCurrent;
  EFI_ENABLE_CHARGING              EnableCharging;
  EFI_SET_CHARGER_RECONFIG         SetChargerReconfig;
  EFI_CHECK_BATTERY                CheckBattery;
  EFI_GET_CHARGER_STATUS           GetChargerStatus;
  EFI_SET_CHARGER_WATCHDOG_TIMER   SetChargerWatchdogTimer;
  EFI_SET_CHARGER_WATCHDOG_KICK    SetChargerWatchdogKick;
  EFI_GET_FACTORY_MODE             GetFactoryMode;
  EFI_GET_FACTORY_SISO:MODE        GetFactorySisoMode;
  EFI_INIT_DEV_CHARGING            InitDevCharging;
  EFI_READ_FUEL_GAUGE_VCELL        ReadFuelGaugeVCell;
  VOID                            *UnknownFunction14;
  VOID                            *UnknownFunction15;
  EFI_GET_BATTERY_TEMPERATURE      GetBatteryTemperature;
  EFI_CHECK_FUEL_GAUGE_MODEL_DATA  CheckFuelGaugeModelData;
  VOID                            *UnknownFunction18;
  EFI_INIT_FUEL_GAUGE              InitFuelGauge;
  VOID                            *UnknownFunction20;
};

extern EFI_GUID gEfiChgProtocolGuid;

#endif /* _EFI_CHG_H_ */
