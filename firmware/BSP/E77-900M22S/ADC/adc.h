/*
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stm32wlxx_ebyte.h>
#include <stm32wlxx_hal.h>

extern ADC_HandleTypeDef hadc;

void MX_ADC_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
