/*
 * Copyright (c) 2020-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

#ifndef STM32WLXX_EBYTE_RADIO_H
#define STM32WLXX_EBYTE_RADIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32wlxx_ebyte_conf.h"
#include "stm32wlxx_ebyte_errno.h"

typedef enum {
  RADIO_SWITCH_OFF = 0,
  RADIO_SWITCH_RX = 1,
  RADIO_SWITCH_RFO_LP = 2,
  RADIO_SWITCH_RFO_HP = 3,
} BSP_RADIO_Switch_TypeDef;

typedef enum {
  RADIO_RFO_LP_MAXPOWER = 0,
  RADIO_RFO_HP_MAXPOWER,
} BSP_RADIO_RFOMaxPowerConfig_TypeDef;

#define RADIO_CONF_RFO_LP_HP 0U
#define RADIO_CONF_RFO_LP 1U
#define RADIO_CONF_RFO_HP 2U

#define RADIO_CONF_TCXO_NOT_SUPPORTED 0U
#define RADIO_CONF_TCXO_SUPPORTED 1U

#define RADIO_CONF_DCDC_NOT_SUPPORTED 0U
#define RADIO_CONF_DCDC_SUPPORTED 1U

#define RADIO_CONF_RFO_HP_MAX_22_dBm ((int32_t)22)
#define RADIO_CONF_RFO_HP_MAX_20_dBm ((int32_t)20)
#define RADIO_CONF_RFO_HP_MAX_17_dBm ((int32_t)17)
#define RADIO_CONF_RFO_HP_MAX_14_dBm ((int32_t)14)
#define RADIO_CONF_RFO_LP_MAX_15_dBm ((int32_t)15)
#define RADIO_CONF_RFO_LP_MAX_14_dBm ((int32_t)14)
#define RADIO_CONF_RFO_LP_MAX_10_dBm ((int32_t)10)

#define RF_SW_CTRL3_PIN GPIO_PIN_6
#define RF_SW_CTRL3_GPIO_PORT GPIOA
#define RF_SW_CTRL3_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define RF_SW_CTRL3_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()

#define RF_SW_CTRL1_PIN GPIO_PIN_7
#define RF_SW_CTRL1_GPIO_PORT GPIOA
#define RF_SW_CTRL1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define RF_SW_CTRL1_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define RF_SW_CTRL2_PIN GPIO_PIN_15
#define RF_SW_CTRL2_GPIO_PORT GPIOA
#define RF_SW_CTRL2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define RF_SW_CTRL2_GPIO_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()

int32_t BSP_RADIO_Init(void);
int32_t BSP_RADIO_DeInit(void);
int32_t BSP_RADIO_ConfigRFSwitch(BSP_RADIO_Switch_TypeDef Config);
int32_t BSP_RADIO_GetTxConfig(void);
int32_t BSP_RADIO_IsTCXO(void);
int32_t BSP_RADIO_IsDCDC(void);
int32_t
BSP_RADIO_GetRFOMaxPowerConfig(BSP_RADIO_RFOMaxPowerConfig_TypeDef Config);

#ifdef __cplusplus
}
#endif

#endif /* STM32WLXX_EBYTE_RADIO_H */
