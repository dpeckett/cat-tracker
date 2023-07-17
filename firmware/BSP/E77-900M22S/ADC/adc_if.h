/*
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 * Header for ADC interface configuration
 */

#ifndef __ADC_IF_H__
#define __ADC_IF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "adc.h"

/**
 * @brief Battery level in mV
 */
#define BAT_CR2032 ((uint32_t)3000)

/**
 * @brief Maximum battery level in mV
 */
#define VDD_BAT BAT_CR2032

/**
 * @brief Minimum battery level in mV
 */
#define VDD_MIN 1800

/**
 * @brief  Initializes the ADC input
 */
void SYS_InitMeasurement(void);

/**
 * @brief DeInitializes the ADC
 */
void SYS_DeInitMeasurement(void);

/**
 * @brief  Get the current temperature
 * @return value temperature in degree Celsius( q7.8 )
 */
int16_t SYS_GetTemperatureLevel(void);

/**
 * @brief Get the current battery level
 * @return value battery level in linear scale
 */
uint16_t SYS_GetBatteryLevel(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_IF_H__ */
