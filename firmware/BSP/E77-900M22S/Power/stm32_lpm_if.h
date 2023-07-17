/*
 * Copyright (c) 2020-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 * Header file for Low Power Manager interface configuration
 */

#ifndef __STM32_LPM_IF_H__
#define __STM32_LPM_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32wlxx_ebyte.h>
#include <stm32wlxx_hal.h>

#include "stm32_lpm.h"

/**
 * @brief Enters Low Power Off Mode
 */
void PWR_EnterOffMode(void);

/**
 * @brief Exits Low Power Off Mode
 */
void PWR_ExitOffMode(void);

/**
 * @brief Enters Low Power Stop Mode
 * @note ARM exists the function when waking up
 */
void PWR_EnterStopMode(void);

/**
 * @brief Exits Low Power Stop Mode
 * @note Enable the pll at 32MHz
 */
void PWR_ExitStopMode(void);

/**
 * @brief Enters Low Power Sleep Mode
 * @note ARM exits the function when waking up
 */
void PWR_EnterSleepMode(void);

/**
 * @brief Exits Low Power Sleep Mode
 * @note ARM exits the function when waking up
 */
void PWR_ExitSleepMode(void);

#ifdef __cplusplus
}
#endif

#endif /*__STM32_LPM_IF_H__ */
