#ifndef _BATT_CHARGER_DRIVER_H_
#define _BATT_CHARGER_DRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "i2c_hal_freertos.h"
#include <stdbool.h>
#include <stdint.h>

/*=========================================================================
    REGISTER BITS
    -----------------------------------------------------------------------*/

/*=========================================================================*/

/*=========================================================================
    CONVERSION FACTORS
    -----------------------------------------------------------------------*/
#define FG_SENSE_RESISTANCE 0.010f //10mOhms
#define MAX77658_FG_CURRENT_LSB 156.25f //uA
#define MAX77658_FUEL_GAUGE_VOLTAGE_LSB 0.078125f //mV
#define MAX77658_FUEL_GAUGE_REPCAP_CONV_FACTOR 0.5f
#define MAX77658_FUEL_GAUGE_DESIGNCAP_MULTI_FACTOR 2u // Inverse of MAX77658_FUEL_GAUGE_REPCAP_CONV_FACTOR as integer
/*=========================================================================



/*=========================================================================
    I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define MAX77658_MAIN_ADDRESS 0x48
#define MAX77658_FUELGAUGE_ADDRESS 0x36

/*=========================================================================*/

/*=========================================================================
    MAIN REGISTERS
    -----------------------------------------------------------------------*/
#define MAX77658_REG_CNFG_CHG_I 0x28
#define MAX77658_REG_CNFG_SBB_TOP 0x38
#define MAX77658_REG_CNFG_SBB0_A 0x39
#define MAX77658_REG_CNFG_SBB0_B 0x3A
#define MAX77658_REG_CNFG_SBB1_A 0x3B
#define MAX77658_REG_CNFG_SBB1_B 0x3C
#define MAX77658_REG_CNFG_SBB2_A 0x3D
#define MAX77658_REG_CNFG_SBB2_B 0x3E
#define MAX77658_REG_CNFG_DVS_SBB0_A 0x3F
#define MAX77658_REG_CNFG_LDO0_A 0x48
#define MAX77658_REG_CNFG_LDO0_B 0x49
#define MAX77658_REG_CNFG_LDO1_A 0x4A
#define MAX77658_REG_CNFG_LDO1_B 0x4B
#define MAX77658_REG_INT_GLBL0 0x00
#define MAX77658_REG_INT_GLBL1 0x04
#define MAX77658_REG_ERCFLAG 0x05
#define MAX77658_REG_STAT_GLBL 0x06
#define MAX77658_REG_INTM_GLBL0 0x08
#define MAX77658_REG_INTM_GLBL1 0x09
#define MAX77658_REG_CNFG_GLBL 0x10
#define MAX77658_REG_CNFG_GPIO0 0x11
#define MAX77658_REG_CNFG_GPIO1 0x12
#define MAX77658_REG_CNFG_GPIO2 0x13
#define MAX77658_REG_CID 0x14
#define MAX77658_REG_CNFG_WDT 0x17
#define MAX77658_REG_INT_CHG 0x01
#define MAX77658_REG_STAT_CHG_A 0x02
#define MAX77658_REG_STAT_CHG_B 0x03
#define MAX77658_REG_INT_M_CHG 0x07
#define MAX77658_REG_CNFG_CHG_A 0x20
#define MAX77658_REG_CNFG_CHG_B 0x21
#define MAX77658_REG_CNFG_CHG_C 0x22
#define MAX77658_REG_CNFG_CHG_D 0x23
#define MAX77658_REG_CNFG_CHG_E 0x24
#define MAX77658_REG_CNFG_CHG_F 0x25
#define MAX77658_REG_CNFG_CHG_G 0x26
#define MAX77658_REG_CNFG_CHG_H 0x27
/*=========================================================================*/

/*=========================================================================
    FUEL GAUGE REGISTERS
    -----------------------------------------------------------------------*/
#define MAX77658_FG_REG_STATUS 0x00
#define MAX77658_FG_REG_VALRTTH 0x01
#define MAX77658_FG_REG_TALRTTH 0x02
#define MAX77658_FG_REG_SALRTTH 0x03
#define MAX77658_FG_REG_FULLSOCTHR 0x13
#define MAX77658_FG_REG_DESIGNCAP 0x18
#define MAX77658_FG_REG_CONFIG 0x1D
#define MAX77658_FG_REG_ICHGTERM 0x1E
#define MAX77658_FG_REG_DEVNAME 0x21
#define MAX77658_FG_REG_LEARNCFG 0x28
#define MAX77658_FG_REG_FILTERCFG 0x29
#define MAX77658_FG_REG_VEMPTY 0x3A
#define MAX77658_FG_REG_POWER 0xB1
#define MAX77658_FG_REG_AVGPOWER 0xB3
#define MAX77658_FG_REG_IALRTTH 0xB4
#define MAX77658_FG_REG_CONFIG2 0xBB
#define MAX77658_FG_REG_TEMP 0x08
#define MAX77658_FG_REG_VCELL 0x09
#define MAX77658_FG_REG_CURRENT 0x0A
#define MAX77658_FG_REG_AVGCURRENT 0x0B
#define MAX77658_FG_REG_AVGTA 0x16
#define MAX77658_FG_REG_AVGVCELL 0x19
#define MAX77658_FG_REG_MAXMINTEMP 0x1A
#define MAX77658_FG_REG_MAXMINVOLT 0x1B
#define MAX77658_FG_REG_MAXMINCURR 0x1C
#define MAX77658_FG_REG_AIN0 0x27
#define MAX77658_FG_REG_TIMER 0x3E
#define MAX77658_FG_REG_TIMERH 0xBE
#define MAX77658_FG_REG_REPCAP 0x05
#define MAX77658_FG_REG_REPSOC 0x06
#define MAX77658_FG_REG_AVSOC 0x0E
#define MAX77658_FG_REG_FULLCAPREP 0x10
#define MAX77658_FG_REG_TTE 0x11
#define MAX77658_FG_REG_RCELL 0x14
#define MAX77658_FG_REG_CYCLES 0x17
#define MAX77658_FG_REG_AVCAP 0x1F
#define MAX77658_FG_REG_TTF 0x20
/*=========================================================================*/

/*=========================================================================
    OTHER
    -----------------------------------------------------------------------*/
#define NUM_MAX77658_INIT_REGS_MAIN 38
#define NUM_MAX77658_INIT_REGS_FG 0
#define MAX77658_WATCHDOG_RESET 0x2F
#define MAX77658_SFT_RESET 0x01
#define BATT_CHARGER_WDT_PERIOD 64000 //ms
#define CHARGING_CABLE_CONNECTED 0x0C
#define BATTERY_MAH 14000 //mAh
/*=========================================================================*/

typedef enum {
  fuel_gauge_success,
  fuel_gauge_i2c_error,
  fuel_gauge_busy,
  fuel_gauge_timeout
} fuel_gauge_errors_t;

typedef struct {
  int16_t Saved_RCOMP0;
  int16_t Saved_TempCo;
  int16_t Saved_FullCapRep;
  int16_t Saved_Cycles;
  int16_t Saved_FullCapNom;
} fram_fuel_gauge_params_t;

typedef enum {
  fuel_gauge_get_batt_percent,
  fuel_gauge_get_batt_voltage,
  fuel_gauge_get_remaining_capacity,
  fuel_gauge_get_battery_current,
} fuel_gauge_op_type_t;

typedef union {
  uint8_t batteryChargePercent;
  uint16_t battVoltage;
  uint16_t remainingCapacityMilliampHours;
  float batteryCurrentMilliamps;
} fuel_gauge_data_t;

typedef struct {
  fuel_gauge_op_type_t data_type;
  fuel_gauge_data_t data;
} fuel_gauge_callback_data_t;

typedef void (*fuelGauge_opDoneCallback_t)(fuel_gauge_errors_t outcome, fuel_gauge_callback_data_t *p_data);

//fuelgauge has a seperate I2C address and 16 bit registers
fuel_gauge_errors_t fuelGauge_init(void);
uint8_t fuelGauge_getSOC(fuelGauge_opDoneCallback_t cb); //returns battery charge in %
double fuelGauge_getBattVoltage(fuelGauge_opDoneCallback_t cb);
double fuelGauge_getRepCap(fuelGauge_opDoneCallback_t cb);      //capacity remaining in mAh
double fuelGauge_getBattCurrent(fuelGauge_opDoneCallback_t cb); //returns in mA

fuel_gauge_errors_t fuelGauge_loadParams(fram_fuel_gauge_params_t *p_params, fuelGauge_opDoneCallback_t cb);
fuel_gauge_errors_t fuelGauge_saveParams(fuelGauge_opDoneCallback_t cb);

#ifdef __cplusplus
}
#endif

#endif /*_BATT_CHARGER_DRIVER_H_*/