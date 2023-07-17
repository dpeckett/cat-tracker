#include "uart.h"

#if defined(__ICCARM__)
/* New definition from EWARM V9, compatible with EWARM8 */
int iar_fputc(int ch);
#define PUTCHAR_PROTOTYPE extern "C" int iar_fputc(int ch)
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
/* ARM Compiler 5/6 */
#define PUTCHAR_PROTOTYPE extern "C" int fputc(int ch, FILE *f)
#elif defined(__GNUC__)
#define PUTCHAR_PROTOTYPE extern "C" int __io_putchar(int ch)
#endif /* __ICCARM__ */

namespace CatTracker {
namespace UART {

extern "C" {
UART_HandleTypeDef huart2;
}

HAL_StatusTypeDef init() {
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK) {
    return HAL_ERROR;
  }

  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    return HAL_ERROR;
  }

  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    return HAL_ERROR;
  }

  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK) {
    return HAL_ERROR;
  }

  return HAL_OK;
}

extern "C" void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if (huart->Instance == USART2) {
    // Initialize the peripherals clocks.
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
    PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    // Peripheral clock enable.
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();

    // USART2 GPIO Configuration
    // PA3 ------> USART2_RX
    // PA2 ------> USART2_TX
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

extern "C" void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART2) {
    // Peripheral clock disable.
    __HAL_RCC_USART2_CLK_DISABLE();

    // USART2 GPIO Configuration
    // PA3 ------> USART2_RX
    // PA2 ------> USART2_TX
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3 | GPIO_PIN_2);
  }
}

PUTCHAR_PROTOTYPE {
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

} // namespace UART
} // namespace CatTracker