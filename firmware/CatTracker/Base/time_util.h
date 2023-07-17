#ifndef CAT_TRACKER_TIME_UTIL_H
#define CAT_TRACKER_TIME_UTIL_H

#include <stm32wlxx_hal.h>

namespace CatTracker {

constexpr uint32_t TicksMax = UINT32_MAX;
constexpr uint32_t TicksHalfMax = TicksMax / 2;

inline uint32_t timeSince(const uint32_t PreviousTick) {
  const auto CurrentTick = HAL_GetTick();
  return (CurrentTick - PreviousTick) & TicksMax;
}

inline uint32_t timeUntil(const uint32_t FutureTick) {
  const auto CurrentTick = HAL_GetTick();
  const auto Delta = (FutureTick - CurrentTick) & TicksMax;

  // If the future tick is in the past (i.e., we've missed the deadline),
  // the delta will be a large number greater than TicksHalfMax due to
  // underflow. In such cases, we return 0, indicating there's no time left
  // until the future tick.
  return (Delta > TicksHalfMax) ? 0 : Delta;
}

} // namespace CatTracker

#endif // CAT_TRACKER_TIME_UTIL_H
