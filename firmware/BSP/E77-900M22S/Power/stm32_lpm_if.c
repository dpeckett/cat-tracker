/*
 * Copyright (c) 2020-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 * Low layer function to enter/exit low power modes (stop, sleep)
 */

#include "stm32_lpm_if.h"

/**
 * @brief Power driver callbacks handler
 */
const struct UTIL_LPM_Driver_s UTIL_PowerDriver = {
    PWR_EnterSleepMode, PWR_ExitSleepMode,

    PWR_EnterStopMode,  PWR_ExitStopMode,

    PWR_EnterOffMode,   PWR_ExitOffMode,
};

void PWR_EnterOffMode(void) {}

void PWR_ExitOffMode(void) {}

void PWR_EnterStopMode(void) {
  HAL_SuspendTick();

  /* Clear Status Flag before entering STOP/STANDBY Mode */
  LL_PWR_ClearFlag_C1STOP_C1STB();

  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}

void PWR_ExitStopMode(void) {
  /* Resume sysTick : work around for debugger problem in dual core */
  HAL_ResumeTick();
}

void PWR_EnterSleepMode(void) {
  HAL_SuspendTick();

  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void PWR_ExitSleepMode(void) {
  HAL_ResumeTick();
}