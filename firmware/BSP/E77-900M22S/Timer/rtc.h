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

#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32wlxx_ebyte.h>
#include <stm32wlxx_hal.h>

#define RTC_N_PREDIV_S 10
#define RTC_PREDIV_S ((1 << RTC_N_PREDIV_S) - 1)
#define RTC_PREDIV_A ((1 << (15 - RTC_N_PREDIV_S)) - 1)

extern RTC_HandleTypeDef hrtc;

HAL_StatusTypeDef MX_RTC_Init();

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */