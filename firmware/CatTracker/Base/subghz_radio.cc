#include "subghz_radio.h"
#include "utilities_def.h"
#include <cstring>
#include <stm32_seq.h>

namespace CatTracker {

namespace SUBGHZRadio {

void OnTxDone();
void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
void OnTxTimeout();
void OnRxTimeout();
void OnRxError();

extern "C" {
SUBGHZ_HandleTypeDef hsubghz;
}

RadioEvents_t RadioEvents = {.TxDone = OnTxDone,
                             .TxTimeout = OnTxTimeout,
                             .RxDone = OnRxDone,
                             .RxTimeout = OnRxTimeout,
                             .RxError = OnRxError};

EventType EvType = EventType::RxTimeout;
Packet RxPacket, TxPacket;
int16_t LastRSSI = 0;

SUBGHZ_HandleTypeDef *getHandle() { return &hsubghz; }

EventType getEventType() { return EvType; }

Packet *getReceivedPacket() { return &RxPacket; }

int16_t getLastRSSI() { return LastRSSI; }

HAL_StatusTypeDef init() {
  Radio.Init(&RadioEvents);

  Radio.SetChannel(RF_FREQUENCY);

  Radio.SetTxConfig(
      MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
      LORA_CODINGRATE, LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
      sizeof(Packet), 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    sizeof(Packet), true, 0, 0, LORA_IQ_INVERSION_ON, true);

  Radio.SetMaxPayloadLength(MODEM_LORA, sizeof(Packet));

  return HAL_OK;
}

void sleep() { Radio.Sleep(); }

uint32_t packetAirTime() {
  return Radio.TimeOnAir(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                         LORA_CODINGRATE, LORA_PREAMBLE_LENGTH,
                         LORA_FIX_LENGTH_PAYLOAD_ON, sizeof(Packet), true);
}

uint32_t wakeupTime() { return Radio.GetWakeupTime(); }

void receivePacket(uint16_t TimeoutMs) { Radio.Rx(TimeoutMs); }

void sendPacket(Packet *SendPacket) {
  memcpy(&TxPacket, SendPacket, sizeof(Packet));
  Radio.Send((uint8_t *)&TxPacket, sizeof(Packet));
}

// override weak HAL function
extern "C" void MX_SUBGHZ_Init() {
  hsubghz.Init.BaudratePrescaler = SUBGHZSPI_BAUDRATEPRESCALER_4;
  HAL_SUBGHZ_Init(&hsubghz);
}

// override weak HAL function
extern "C" void HAL_SUBGHZ_MspInit(SUBGHZ_HandleTypeDef *Handle) {
  __HAL_RCC_SUBGHZSPI_CLK_ENABLE();

  HAL_NVIC_SetPriority(SUBGHZ_Radio_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SUBGHZ_Radio_IRQn);
}

// override weak HAL function
extern "C" void HAL_SUBGHZ_MspDeInit(SUBGHZ_HandleTypeDef *Handle) {
  __HAL_RCC_SUBGHZSPI_CLK_DISABLE();

  HAL_NVIC_DisableIRQ(SUBGHZ_Radio_IRQn);
}

void OnTxDone() {
  EvType = EventType::Tx;

  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_SubGHz_ProcessEvent), CFG_SEQ_Prio_0);
}

void OnRxDone(uint8_t *Payload, uint16_t Size, int16_t RSSI, int8_t SNR) {
  memcpy(&RxPacket, Payload, (Size < sizeof(Packet)) ? Size : sizeof(Packet));

  if (Size == sizeof(Packet) && RxPacket.Magic == PacketMagic) {
    EvType = EventType::Rx;
    LastRSSI = RSSI;
  } else {
    EvType = EventType::RxError;
  }

  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_SubGHz_ProcessEvent), CFG_SEQ_Prio_0);
}

void OnTxTimeout() {
  EvType = EventType::TxTimeout;

  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_SubGHz_ProcessEvent), CFG_SEQ_Prio_0);
}

void OnRxTimeout() {
  EvType = EventType::RxTimeout;

  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_SubGHz_ProcessEvent), CFG_SEQ_Prio_0);
}

void OnRxError() {
  EvType = EventType::RxError;

  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_SubGHz_ProcessEvent), CFG_SEQ_Prio_0);
}

} // namespace SUBGHZRadio

} // namespace CatTracker