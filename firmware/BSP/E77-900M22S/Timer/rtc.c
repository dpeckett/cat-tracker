/*
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at: www.st.com/SLA0044.
 *
 * This file provides code for the configuration of the RTC instances.
 */

#include "rtc.h"

RTC_HandleTypeDef hrtc;

HAL_StatusTypeDef MX_RTC_Init() {
  RTC_AlarmTypeDef sAlarm = {0};

  /** Initialize RTC Only
   */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_PREDIV_A;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  hrtc.Init.BinMode = RTC_BINARY_ONLY;
  if (HAL_RTC_Init(&hrtc) != HAL_OK) {
    return HAL_ERROR;
  }

  /** Initialize RTC and set the Time and Date
   */
  if (HAL_RTCEx_SetSSRU_IT(&hrtc) != HAL_OK) {
    return HAL_ERROR;
  }

  /** Enable the Alarm A
   */
  sAlarm.BinaryAutoClr = RTC_ALARMSUBSECONDBIN_AUTOCLR_NO;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDBINMASK_NONE;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, 0) != HAL_OK) {
    return HAL_ERROR;
  }

  return HAL_OK;
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *rtcHandle) {

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if (rtcHandle->Instance == RTC) {
    /** Initializes the peripherals clocks
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_RTCAPB_CLK_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(TAMP_STAMP_LSECSS_SSRU_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TAMP_STAMP_LSECSS_SSRU_IRQn);
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef *rtcHandle) {

  if (rtcHandle->Instance == RTC) {
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
    __HAL_RCC_RTCAPB_CLK_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(TAMP_STAMP_LSECSS_SSRU_IRQn);
    HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);
  }
}