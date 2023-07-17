#ifndef CAT_TRACKER_UART_H
#define CAT_TRACKER_UART_H

#include <stm32wlxx_hal.h>

namespace CatTracker {
namespace UART {

HAL_StatusTypeDef init();

} // namespace UART
} // namespace CatTracker

#endif // CAT_TRACKER_UART_H