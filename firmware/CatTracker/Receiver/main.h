#ifndef CAT_TRACKER_MAIN_H
#define CAT_TRACKER_MAIN_H

#include <stm32wlxx_hal.h>

#include <stm32_lpm.h>
#include <stm32_seq.h>
#include <stm32_timer.h>

namespace CatTracker {

constexpr uint32_t WakeupTimeoutMs = 30000;
constexpr uint32_t SearchPacketIntervalMs = 100;
constexpr uint32_t WokenUpPacketIntervalMs = 1000;

void Error_Handler();

} // namespace CatTracker

#endif // CAT_TRACKER_MAIN_H