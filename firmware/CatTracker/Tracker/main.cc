#include <array>
#include <cstdint>
#include <radio.h>

#include "adc_if.h"
#include "main.h"
#include "subghz_radio.h"
#include "time_util.h"

namespace CatTracker {

void radioProcessEvent();
void sendBeacon(void *Context);

HAL_StatusTypeDef configureSystemClock();

UTIL_TIMER_Object_t BeaconTimer;

extern "C" int main() {
  if (HAL_Init() != HAL_OK) {
    Error_Handler();
  }

  if (configureSystemClock() != HAL_OK) {
    Error_Handler();
  }

  // Initialize timer and RTC.
  UTIL_TIMER_Init();

  // Init low power manager.
  UTIL_LPM_Init();

  // Disable Stand-by mode.
  UTIL_LPM_SetOffMode((1 << CFG_LPM_APPLI_Id), UTIL_LPM_DISABLE);

  if (SUBGHZRadio::init() != HAL_OK) {
    Error_Handler();
  }

  // Small delay to allow the radio to settle.
  HAL_Delay(1000);

  // Register radio event handler.
  UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_SubGHz_ProcessEvent), UTIL_SEQ_RFU,
                   radioProcessEvent);

  // Kick off the radio event loop.
  sendBeacon(nullptr);

  for (;;) {
    UTIL_SEQ_Run(UTIL_SEQ_DEFAULT);
  }

  return 0;
}

void radioProcessEvent() {
  SUBGHZRadio::sleep();

  const auto NextTransmitMs =
      timeUntil(HAL_GetTick() + TransmitIntervalMs - SUBGHZRadio::wakeupTime());

  UTIL_TIMER_Create(&BeaconTimer, NextTransmitMs, UTIL_TIMER_ONESHOT,
                    sendBeacon, nullptr);
  UTIL_TIMER_Start(&BeaconTimer);
}

void sendBeacon(void *Context) {
  const auto DeviceID = HAL_GetUIDw0();

  SUBGHZRadio::Packet Beacon = {
      .Magic = SUBGHZRadio::PacketMagic,
      .Kind = SUBGHZRadio::PacketKind::Beacon,
  };

  auto ResponsePayload =
      reinterpret_cast<SUBGHZRadio::PayloadBeacon *>(Beacon.Payload);

  ResponsePayload->DeviceID = DeviceID;

  SUBGHZRadio::sendPacket(&Beacon);
}

HAL_StatusTypeDef configureSystemClock() {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure LSE Drive Capability.
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  // Configure the main internal regulator output voltage.
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Initializes the CPU, AHB and APB busses clocks.
  RCC_OscInitStruct.OscillatorType =
      RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSEDiv = RCC_HSE_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 12;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    return HAL_ERROR;
  }

  // Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers.
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3 | RCC_CLOCKTYPE_HCLK |
                                RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    return HAL_ERROR;
  }

  // Ensure that MSI is wake-up system clock.
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

  return HAL_OK;
}

void Error_Handler() { HAL_NVIC_SystemReset(); }

// redefines __weak function in stm32_seq.c such to enter low power.
extern "C" void UTIL_SEQ_Idle(void) {
  if (Radio.GetStatus() == RF_IDLE) {
    Radio.Sleep();
    UTIL_LPM_EnterLowPower();
  }
}

} // namespace CatTracker