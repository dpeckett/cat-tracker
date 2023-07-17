/*
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 * Read status related to the chip (battery level, VREF, chip temperature).
 *
 */

#include "adc_if.h"

/**
 * @brief ADC handle
 */
extern ADC_HandleTypeDef hadc;

#define TEMPSENSOR_TYP_CAL1_V                                                  \
  ((int32_t)760) /*!< Internal temperature sensor, parameter V30 (unit: mV).   \
                    Refer to device datasheet for min/typ/max values. */
#define TEMPSENSOR_TYP_AVGSLOPE                                                \
  ((int32_t)2500) /*!< Internal temperature sensor, parameter Avg_Slope (unit: \
                     uV/DegCelsius). Refer to device datasheet for min/typ/max \
                     values. */

/**
 * @brief This function reads the ADC channel
 * @param channel channel number to read
 * @return adc measured level value
 */
static uint32_t ADC_ReadChannels(uint32_t channel);

void SYS_InitMeasurement(void) { hadc.Instance = ADC; }

void SYS_DeInitMeasurement(void) {}

int16_t SYS_GetTemperatureLevel(void) {
  __IO int16_t temperatureDegreeC = 0;
  uint32_t measuredLevel = 0;
  uint16_t batteryLevelmV = SYS_GetBatteryLevel();

  measuredLevel = ADC_ReadChannels(ADC_CHANNEL_TEMPSENSOR);

  /* convert ADC level to temperature */
  /* check whether device has temperature sensor calibrated in production */
  if (((int32_t)*TEMPSENSOR_CAL2_ADDR - (int32_t)*TEMPSENSOR_CAL1_ADDR) != 0) {
    /* Device with temperature sensor calibrated in production:
       use device optimized parameters */
    temperatureDegreeC = __LL_ADC_CALC_TEMPERATURE(
        batteryLevelmV, measuredLevel, LL_ADC_RESOLUTION_12B);
  } else {
    /* Device with temperature sensor not calibrated in production:
       use generic parameters */
    temperatureDegreeC = __LL_ADC_CALC_TEMPERATURE_TYP_PARAMS(
        TEMPSENSOR_TYP_AVGSLOPE, TEMPSENSOR_TYP_CAL1_V, TEMPSENSOR_CAL1_TEMP,
        batteryLevelmV, measuredLevel, LL_ADC_RESOLUTION_12B);
  }

  /* from int16 to q8.7*/
  temperatureDegreeC <<= 8;

  return (int16_t)temperatureDegreeC;
}

uint16_t SYS_GetBatteryLevel(void) {
  uint16_t batteryLevelmV = 0;
  uint32_t measuredLevel = 0;

  measuredLevel = ADC_ReadChannels(ADC_CHANNEL_VREFINT);

  if (measuredLevel == 0) {
    batteryLevelmV = 0;
  } else {
    if ((uint32_t)*VREFINT_CAL_ADDR != (uint32_t)0xFFFFU) {
      /* Device with Reference voltage calibrated in production:
         use device optimized parameters */
      batteryLevelmV =
          __LL_ADC_CALC_VREFANALOG_VOLTAGE(measuredLevel, ADC_RESOLUTION_12B);
    } else {
      /* Device with Reference voltage not calibrated in production:
         use generic parameters */
      batteryLevelmV = (VREFINT_CAL_VREF * 1510) / measuredLevel;
    }
  }

  return batteryLevelmV;
}

static uint32_t ADC_ReadChannels(uint32_t channel) {
  uint32_t ADCxConvertedValues = 0;
  ADC_ChannelConfTypeDef sConfig = {0};

  MX_ADC_Init();

  /* Start Calibration */
  if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK) {
    /* Calibration Error */
    return 0;
  }

  /* Configure Regular Channel */
  sConfig.Channel = channel;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
    return 0;
  }

  if (HAL_ADC_Start(&hadc) != HAL_OK) {
    /* Start Error */
    return 0;
  }
  /** Wait for end of conversion */
  HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);

  /** Wait for end of conversion */
  HAL_ADC_Stop(&hadc); /* it calls also ADC_Disable() */

  ADCxConvertedValues = HAL_ADC_GetValue(&hadc);

  HAL_ADC_DeInit(&hadc);

  return ADCxConvertedValues;
}
