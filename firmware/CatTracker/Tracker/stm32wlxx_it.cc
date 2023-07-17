#include "main.h"
#include "rtc.h"
#include "subghz_radio.h"

using namespace CatTracker;

// Non maskable interrupt.
extern "C" void NMI_Handler() { Error_Handler(); }

// Hard fault interrupt.
extern "C" void HardFault_Handler() { Error_Handler(); }

// Memory management fault.
extern "C" void MemManage_Handler() { Error_Handler(); }

// System service call via SWI instruction.
extern "C" void SVC_Handler() {}

// Debug monitor.
extern "C" void DebugMon_Handler() {}

// Pendable request for system service.
extern "C" void PendSV_Handler() {}

// System tick timer.
extern "C" void SysTick_Handler() {}

// RTC Tamper, RTC TimeStamp, LSECSS and RTC SSRU.
extern "C" void TAMP_STAMP_LSECSS_SSRU_IRQHandler() {
  HAL_RTCEx_SSRUIRQHandler(&hrtc);
}

// RTC Alarms (A and B).
extern "C" void RTC_Alarm_IRQHandler() { HAL_RTC_AlarmIRQHandler(&hrtc); }

// SUBGHZ Radio.
extern "C" void SUBGHZ_Radio_IRQHandler() {
  HAL_SUBGHZ_IRQHandler(SUBGHZRadio::getHandle());
}