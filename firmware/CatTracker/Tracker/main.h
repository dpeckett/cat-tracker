#ifndef CAT_TRACKER_MAIN_H
#define CAT_TRACKER_MAIN_H

#include <stm32wlxx_hal.h>

#include <stm32_lpm.h>
#include <stm32_seq.h>
#include <stm32_timer.h>

namespace CatTracker {

constexpr uint32_t TransmitIntervalMs = 30000;

void Error_Handler();

} // namespace CatTracker

#endif // CAT_TRACKER_MAIN_H