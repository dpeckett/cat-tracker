/*
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at: www.st.com/SLA0044.
 *
 * Configure RTC Alarm, Tick and Calendar manager.
 */

#include <math.h>
#include <stm32wlxx_ebyte.h>
#include <stm32wlxx_hal.h>
#include <stm32wlxx_ll_rtc.h>

#include "rtc.h"
#include "timer_if.h"
#include "utilities_def.h"

/**
 * @brief RTC handle
 */
extern RTC_HandleTypeDef hrtc;

/**
 * @brief Timer driver callbacks handler
 */
const UTIL_TIMER_Driver_s UTIL_TimerDriver = {
    TIMER_IF_Init,
    NULL,

    TIMER_IF_StartTimer,
    TIMER_IF_StopTimer,

    TIMER_IF_SetTimerContext,
    TIMER_IF_GetTimerContext,

    TIMER_IF_GetTimerElapsedTime,
    TIMER_IF_GetTimerValue,
    TIMER_IF_GetMinimumTimeout,

    TIMER_IF_Convert_ms2Tick,
    TIMER_IF_Convert_Tick2ms,
};

/**
 * @brief SysTime driver callbacks handler
 */
const UTIL_SYSTIM_Driver_s UTIL_SYSTIMDriver = {
    TIMER_IF_BkUp_Write_Seconds,
    TIMER_IF_BkUp_Read_Seconds,
    TIMER_IF_BkUp_Write_SubSeconds,
    TIMER_IF_BkUp_Read_SubSeconds,
    TIMER_IF_GetTime,
};

/**
 * @brief Minimum timeout delay of Alarm in ticks
 */
#define MIN_ALARM_DELAY 3

/**
 * @brief Backup seconds register
 */
#define RTC_BKP_SECONDS RTC_BKP_DR0

/**
 * @brief Backup subseconds register
 */
#define RTC_BKP_SUBSECONDS RTC_BKP_DR1

/**
 * @brief Backup msbticks register
 */
#define RTC_BKP_MSBTICKS RTC_BKP_DR2

/* #define RTIF_DEBUG */

/**
 * @brief Map UTIL_TIMER_IRQ can be overridden in utilities_conf.h to Map on
 * Task rather then Isr
 */
#ifndef UTIL_TIMER_IRQ_MAP_INIT
#define UTIL_TIMER_IRQ_MAP_INIT()
#endif /* UTIL_TIMER_IRQ_MAP_INIT */

#ifndef UTIL_TIMER_IRQ_MAP_PROCESS
#define UTIL_TIMER_IRQ_MAP_PROCESS() UTIL_TIMER_IRQ_Handler()
#endif /* UTIL_TIMER_IRQ_MAP_PROCESS */

/* Private macro -------------------------------------------------------------*/
#ifdef RTIF_DEBUG
#include "sys_app.h" /*for app_log*/
/**
 * @brief Post the RTC log string format to the circular queue for printing in
 * using the polling mode
 */
#define TIMER_IF_DBG_PRINTF(...)                                               \
  do {                                                                         \
    {                                                                          \
      UTIL_ADV_TRACE_COND_FSend(VLEVEL_ALWAYS, T_REG_OFF, TS_OFF,              \
                                __VA_ARGS__);                                  \
    }                                                                          \
  } while (0);
#else
/**
 * @brief not used
 */
#define TIMER_IF_DBG_PRINTF(...)
#endif /* RTIF_DEBUG */

/**
 * @brief Indicates if the RTC is already Initialized or not
 */
static bool RTC_Initialized = false;

/**
 * @brief RtcTimerContext
 */
static uint32_t RtcTimerContext = 0;

/**
 * @brief Get rtc timer Value in rtc tick
 * @return val the rtc timer value (upcounting)
 */
static inline uint32_t GetTimerTicks(void);

/**
 * @brief Writes MSBticks to backup register
 * Absolute RTC time in tick is (MSBticks)<<32 + (32bits binary counter)
 * @note MSBticks incremented every time the 32bits RTC timer wraps around
 * (~44days)
 * @param[in] MSBticks
 */
static void TIMER_IF_BkUp_Write_MSBticks(uint32_t MSBticks);

/**
 * @brief Reads MSBticks from backup register
 * Absolute RTC time in tick is (MSBticks)<<32 + (32bits binary counter)
 * @note MSBticks incremented every time the 32bits RTC timer wraps around
 * (~44days)
 * @retval MSBticks
 */
static uint32_t TIMER_IF_BkUp_Read_MSBticks(void);

UTIL_TIMER_Status_t TIMER_IF_Init(void) {
  UTIL_TIMER_Status_t ret = UTIL_TIMER_OK;
  if (RTC_Initialized == false) {
    hrtc.IsEnabled.RtcFeatures = UINT32_MAX;
    /*Init RTC*/
    if (MX_RTC_Init() != HAL_OK) {
      return UTIL_TIMER_UNKNOWN_ERROR;
    }

    /*Stop Timer */
    TIMER_IF_StopTimer();
    /** DeActivate the Alarm A enabled by STM32CubeMX during MX_RTC_Init() */
    HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
    /*overload RTC feature enable*/
    hrtc.IsEnabled.RtcFeatures = UINT32_MAX;

    /*Enable Direct Read of the calendar registers (not through Shadow) */
    HAL_RTCEx_EnableBypassShadow(&hrtc);
    /*Initialize MSB ticks*/
    TIMER_IF_BkUp_Write_MSBticks(0);

    TIMER_IF_SetTimerContext();

    /* Register a task to associate to UTIL_TIMER_Irq() interrupt */
    UTIL_TIMER_IRQ_MAP_INIT();

    RTC_Initialized = true;
  }

  return ret;
}

UTIL_TIMER_Status_t TIMER_IF_StartTimer(uint32_t timeout) {
  UTIL_TIMER_Status_t ret = UTIL_TIMER_OK;
  RTC_AlarmTypeDef sAlarm = {0};
  /*Stop timer if one is already started*/
  TIMER_IF_StopTimer();
  timeout += RtcTimerContext;

  TIMER_IF_DBG_PRINTF("Start timer: time=%d, alarm=%d\n\r", GetTimerTicks(),
                      timeout);
  /* starts timer*/
  sAlarm.BinaryAutoClr = RTC_ALARMSUBSECONDBIN_AUTOCLR_NO;
  sAlarm.AlarmTime.SubSeconds = UINT32_MAX - timeout;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDBINMASK_NONE;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK) {
    return UTIL_TIMER_HW_ERROR;
  }

  return ret;
}

UTIL_TIMER_Status_t TIMER_IF_StopTimer(void) {
  UTIL_TIMER_Status_t ret = UTIL_TIMER_OK;
  /* Clear RTC Alarm Flag */
  __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);
  /* Disable the Alarm A interrupt */
  HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);
  /*overload RTC feature enable*/
  hrtc.IsEnabled.RtcFeatures = UINT32_MAX;
  return ret;
}

uint32_t TIMER_IF_SetTimerContext(void) {
  /*store time context*/
  RtcTimerContext = GetTimerTicks();

  TIMER_IF_DBG_PRINTF("TIMER_IF_SetTimerContext=%d\n\r", RtcTimerContext);
  /*return time context*/
  return RtcTimerContext;
}

uint32_t TIMER_IF_GetTimerContext(void) {
  TIMER_IF_DBG_PRINTF("TIMER_IF_GetTimerContext=%d\n\r", RtcTimerContext);
  /*return time context*/
  return RtcTimerContext;
}

uint32_t TIMER_IF_GetTimerElapsedTime(void) {
  uint32_t ret = 0;
  ret = ((uint32_t)(GetTimerTicks() - RtcTimerContext));
  return ret;
}

uint32_t TIMER_IF_GetTimerValue(void) {
  uint32_t ret = 0;
  if (RTC_Initialized == true) {
    ret = GetTimerTicks();
  }

  return ret;
}

uint32_t TIMER_IF_GetMinimumTimeout(void) {
  uint32_t ret = 0;
  ret = (MIN_ALARM_DELAY);
  return ret;
}

uint32_t TIMER_IF_Convert_ms2Tick(uint32_t timeMilliSec) {
  uint32_t ret = 0;
  ret = ((uint32_t)((((uint64_t)timeMilliSec) << RTC_N_PREDIV_S) / 1000));
  return ret;
}

uint32_t TIMER_IF_Convert_Tick2ms(uint32_t tick) {
  uint32_t ret = 0;
  ret = ((uint32_t)((((uint64_t)(tick)) * 1000) >> RTC_N_PREDIV_S));
  return ret;
}

void TIMER_IF_DelayMs(uint32_t delay) {
  uint32_t delayTicks = TIMER_IF_Convert_ms2Tick(delay);
  uint32_t timeout = GetTimerTicks();

  /* Wait delay ms */
  while (((GetTimerTicks() - timeout)) < delayTicks) {
    __NOP();
  }
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
  UTIL_TIMER_IRQ_MAP_PROCESS();
}

void HAL_RTCEx_SSRUEventCallback(RTC_HandleTypeDef *hrtc) {
  /*called every 48 days with 1024 ticks per seconds*/
  TIMER_IF_DBG_PRINTF(">>Handler SSRUnderflow at %d\n\r", GetTimerTicks());
  /*Increment MSBticks*/
  uint32_t MSB_ticks = TIMER_IF_BkUp_Read_MSBticks();
  TIMER_IF_BkUp_Write_MSBticks(MSB_ticks + 1);
}

uint32_t TIMER_IF_GetTime(uint16_t *mSeconds) {
  uint32_t seconds = 0;
  uint64_t ticks;
  uint32_t timerValueLsb = GetTimerTicks();
  uint32_t timerValueMSB = TIMER_IF_BkUp_Read_MSBticks();

  ticks = (((uint64_t)timerValueMSB) << 32) + timerValueLsb;

  seconds = (uint32_t)(ticks >> RTC_N_PREDIV_S);

  ticks = (uint32_t)ticks & RTC_PREDIV_S;

  *mSeconds = TIMER_IF_Convert_Tick2ms(ticks);

  return seconds;
}

void TIMER_IF_BkUp_Write_Seconds(uint32_t Seconds) {
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_SECONDS, Seconds);
}

void TIMER_IF_BkUp_Write_SubSeconds(uint32_t SubSeconds) {
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_SUBSECONDS, SubSeconds);
}

uint32_t TIMER_IF_BkUp_Read_Seconds(void) {
  uint32_t ret = 0;
  ret = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_SECONDS);
  return ret;
}

uint32_t TIMER_IF_BkUp_Read_SubSeconds(void) {
  uint32_t ret = 0;
  ret = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_SUBSECONDS);
  return ret;
}

static void TIMER_IF_BkUp_Write_MSBticks(uint32_t MSBticks) {
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_MSBTICKS, MSBticks);
}

static uint32_t TIMER_IF_BkUp_Read_MSBticks(void) {
  uint32_t MSBticks;
  MSBticks = HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_MSBTICKS);
  return MSBticks;
}

static inline uint32_t GetTimerTicks(void) {
  return (UINT32_MAX - LL_RTC_TIME_GetSubSecond(RTC));
}

/**
 * @note This function overwrites the __weak one from HAL
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {
  /*Don't enable SysTick if TIMER_IF is based on other counters (e.g. RTC) */
  return HAL_OK;
}

/**
 * @note This function overwrites the __weak one from HAL
 */
uint32_t HAL_GetTick(void) {
  uint32_t ret = 0;
  /* TIMER_IF can be based on other counter the SysTick e.g. RTC */
  if (!RTC_Initialized) {
    /* TIMER_IF_GetTimerValue should be used only once UTIL_TIMER_Init() is
     * initialized */
    /* If HAL_Delay or a TIMEOUT countdown is necessary during initialization
     * phase */
    /* please use temporarily another timebase source (SysTick or TIMx), which
     * implies also */
    /* to rework the above function HAL_InitTick() and to call HAL_IncTick() on
     * the timebase IRQ */
    /* Note: when TIMER_IF is based on RTC, stm32wlxx_hal_rtc.c calls this
     * function before TimeServer is functional */
    /* RTC TIMEOUT will not expire, i.e. if RTC has an hw problem it will keep
     * looping in the RTC_Init function */
  } else {
    ret = TIMER_IF_GetTimerValue();
  }

  return ret;
}

/**
 * @note This function overwrites the __weak one from HAL
 */
void HAL_Delay(__IO uint32_t Delay) {
  /* TIMER_IF can be based on other counter the SysTick e.g. RTC */
  TIMER_IF_DelayMs(Delay);
}