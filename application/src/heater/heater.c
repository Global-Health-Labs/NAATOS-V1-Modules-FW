#include "heater.h"
#include "../motor.h"
#include "timers.h"

xQueueHandle heaterRxQueue;

int wdtTimeout = 0;

HeaterInterface samplePrepHeater_I = {
    .handleHeaterSensorDataRx = &samplePrepHandleHeaterSensorDataRx,
    .handleMotorDataRx = &handleSampleMotorDataRx,
    .handleHeaterZoneStateUpdate = &samplePrepHandleHeaterZoneStateUpdate,
    .resetHeaterPIDs = &samplePrepResetHeaterPIDs,
    .getPwmData = &getSamplePrepPwmData,
    .getOverTempStatus = &getSamplePrepOverTempStatus,
    .getHeaterRunningStatus = &getSamplePrepHeaterRunningStatus,
    .getOverTempData = &getSamplePrepOverTempData};

HeaterInterface powerModuleHeater_I = {
    .handleHeaterSensorDataRx = &powerModuleHandleHeaterSensorDataRx,
    .handleMotorDataRx = NULL,
    .handleHeaterZoneStateUpdate = &powerModuleHandleHeaterZoneStateUpdate,
    .resetHeaterPIDs = &powerModuleResetHeaterPIDs,
    .getPwmData = &getPowerModulePwmData,
    .getOverTempStatus = &getPowerModuleOverTempStatus,
    .getHeaterRunningStatus = &getPowerModuleHeaterRunningStatus,
    .getOverTempData = &getPowerModuleOverTempData};

void sendWdtHeaterValid() {
  BaseType_t xReturned;
  watchdog_time_update_t wdtUpdate = {};
  wdtUpdate.taskName = HEATER;
  wdtUpdate.valid = true;

  xReturned = xQueueSend(watchdog_rxTimesQueue, &wdtUpdate, 0);
  if (xReturned != pdPASS) {
    send_debug_log_message("LOG_TASK: Unable to send WDT update to watchdog_rxTimesQueue. in battery task");
  }
}

void heater_task(void *pvParameters) {
  BaseType_t xReturned;
  HeaterRxQueueMsg_t heaterRxMessage;

#ifdef SAMPLE_PREP_BOARD
  HeaterInterface *heaterInterface = &samplePrepHeater_I;
#else
  HeaterInterface *heaterInterface = &powerModuleHeater_I;
#endif
  heaterInterface->resetHeaterPIDs();

  for (;;) {
    xReturned = xQueueReceive(heaterRxQueue, &heaterRxMessage, portMAX_DELAY);
    if (xReturned != pdPASS) {
      send_debug_log_message("Unable to Rx data to heater queue");
    } else {
      switch (heaterRxMessage.type) {
      case HEATER_MSG_SLEEP:
        // Handle sleep message
        break;
      case HEATER_MSG_WAKE:
        // Handle wake message
        break;
      case HEATER_MSG_ZONE_STATE: {
        heaterInterface->handleHeaterZoneStateUpdate(heaterRxMessage);
        break;
      }
      case HEATER_MSG_TEMPERATURE_DATA: {
        if (heaterRxMessage.readTempFailed) {
          MainStateErrorQueueMsg_t main_err_msg;
          bool greaterThanMaxTemp = heaterInterface->getOverTempStatus();
          
          if(greaterThanMaxTemp) {
            main_err_msg.errType = ERR_OVERTEMP_EVENT;
            main_err_msg.overTempData = heaterInterface->getOverTempData();
          } else { // I2c read error
            main_err_msg.errType = ERR_TEMP_SENSOR_READ;
          }

          xReturned = xQueueSend(main_runErrorQueue, &greaterThanMaxTemp, 0);
          if (xReturned != pdPASS) {
            send_debug_log_message("HEATER_TASK: Unable to send run error for greater than max temp to main_runErrorQueue.");
          }
        } else {
          if (wdtTimeout++ > (1 / config.sample_rate)) { // send out once a second
            wdtTimeout = 0;
            sendWdtHeaterValid(); // update watchdog
          }
          heaterInterface->handleHeaterSensorDataRx(heaterRxMessage.tempData);
        }
        break;
      }
      case HEATER_MSG_MOTOR_DATA: {
        heaterInterface->handleMotorDataRx(heaterRxMessage.motorSpeed);
        break;
      }
      case HEATER_MSG_USB_SUSPEND:
        // Handle USB suspend message
        break;
      case HEATER_MSG_SENSOR_CONFIRM: {
        // Rx confirmation that sensor got heater update
        // Send the confirmation to the main state
        bool heaterRun = heaterInterface->getHeaterRunningStatus();
        xReturned = xQueueSend(main_runConfRespQueue, &heaterRun, 0);
        if (xReturned != pdPASS) {
          send_debug_log_message("HEATER_TASK: Unable to send run response to main_runConfRespQueue.");
        }
        break;
      }

      case HEATER_MSG_CONFIG_UPDATED:
        // Handle config updated message

        break;
      case HEATER_MSG_WDT_UPDATE:
        // Handle watchdog timer update message
        break;
      default:
        // Handle unknown message
        break;
      }
    }
  }
}