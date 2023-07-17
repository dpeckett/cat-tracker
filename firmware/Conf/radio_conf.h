#ifndef __RADIO_CONF_H__
#define __RADIO_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mw_log_conf.h"    /* mw trace configuration */
#include "radio_board_if.h" /* interface layer between MW and Radio Board/Module */
#include "stm32_mem.h"      /* RADIO_MEMSET8 def in this file */
#include "utilities_def.h" /* Definitions for modules requiring utilities */

void MX_SUBGHZ_Init(void);

/**
 * @brief drive value used anytime radio is NOT in TX low power mode
 * @note override the default configuration of radio_driver.c
 */
#define SMPS_DRIVE_SETTING_DEFAULT SMPS_DRV_40

/**
 * @brief drive value used anytime radio is in TX low power mode
 *        TX low power mode is the worst case because the PA sinks from SMPS
 *        while in high power mode, current is sunk directly from the battery
 * @note override the default configuration of radio_driver.c
 */
#define SMPS_DRIVE_SETTING_MAX SMPS_DRV_60

/**
 * @brief Provides the frequency of the chip running on the radio and the
 * frequency step
 * @remark These defines are used for computing the frequency divider to set the
 * RF frequency
 * @note override the default configuration of radio_driver.c
 */
#define XTAL_FREQ (32000000UL)

/**
 * @brief in XO mode, set internal capacitor (from 0x00 to 0x2F starting 11.2pF
 * with 0.47pF steps)
 * @note override the default configuration of radio_driver.c
 */
#define XTAL_DEFAULT_CAP_VALUE (0x20UL)

/**
 * @brief voltage of vdd tcxo.
 * @note override the default configuration of radio_driver.c
 */
#define TCXO_CTRL_VOLTAGE TCXO_CTRL_1_7V

/**
 * @brief Radio maximum wakeup time (in ms)
 * @note override the default configuration of radio_driver.c
 */
#define RF_WAKEUP_TIME (1UL)

/**
 * @brief DCDC is enabled
 * @remark this define is only used if the DCDC is present on the board
 * @note override the default configuration of radio_driver.c
 */
#define DCDC_ENABLE (1UL)

#ifndef CRITICAL_SECTION_BEGIN
#define CRITICAL_SECTION_BEGIN() UTILS_ENTER_CRITICAL_SECTION()
#endif /* !CRITICAL_SECTION_BEGIN */
#ifndef CRITICAL_SECTION_END
#define CRITICAL_SECTION_END() UTILS_EXIT_CRITICAL_SECTION()
#endif /* !CRITICAL_SECTION_END */

/**
 * @brief SUBGHZ interface init to radio Middleware
 */
#define RADIO_INIT MX_SUBGHZ_Init

/**
 * @brief Delay interface to radio Middleware
 */
#define RADIO_DELAY_MS HAL_Delay

/**
 * @brief Memset utilities interface to radio Middleware
 */
#define RADIO_MEMSET8(dest, value, size) UTIL_MEM_set_8(dest, value, size)

/**
 * @brief Memcpy utilities interface to radio Middleware
 */
#define RADIO_MEMCPY8(dest, src, size) UTIL_MEM_cpy_8(dest, src, size)

#ifdef __cplusplus
}
#endif

#endif /* __RADIO_CONF_H__*/
