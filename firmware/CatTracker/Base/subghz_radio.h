#ifndef CAT_TRACKER_SUBGHZ_RADIO_H
#define CAT_TRACKER_SUBGHZ_RADIO_H

#include <cstdint>
#include <radio.h>
#include <stm32wlxx_hal.h>

// EU 868MHz (Sub-band P)
#define RF_FREQUENCY 869525000

// TX output power in dBm.
#define TX_OUTPUT_POWER 7

// Attempt to get a packet through in ~30ms.
#define LORA_BANDWIDTH                                                         \
  1 /* [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved]                       \
     */
#define LORA_SPREADING_FACTOR 7 /* [SF7..SF12] */
#define LORA_CODINGRATE 1       /* [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
#define LORA_PREAMBLE_LENGTH 8  /* Same for Tx and Rx */
#define LORA_SYMBOL_TIMEOUT 5   /* Symbols */
#define LORA_FIX_LENGTH_PAYLOAD_ON true
#define LORA_IQ_INVERSION_ON false

#define TX_TIMEOUT_VALUE 1000

namespace CatTracker {

namespace SUBGHZRadio {

// To account for the time it takes the transmitter to determine the channel is
// clear etc.
constexpr uint32_t ReceiveMarginMs = 150;

enum class EventType {
  Rx,
  RxTimeout,
  RxError,
  Tx,
  TxTimeout,
};

// Randomly chosen and unlikely to be found in the wild.
constexpr uint16_t PacketMagic = 0xBB72;

enum class PacketKind : uint8_t {
  Invalid = 0x00,
  Beacon = 0x01,
};

struct Packet {
  uint16_t Magic;
  PacketKind Kind;
  uint8_t Payload[8];
} __attribute__((packed));

struct PayloadBeacon {
  uint32_t DeviceID;
} __attribute__((packed));

HAL_StatusTypeDef init();
void sleep();

SUBGHZ_HandleTypeDef *getHandle();
EventType getEventType();
Packet *getReceivedPacket();
int16_t getLastRSSI();

void receivePacket(uint16_t TimeoutMs);
void sendPacket(Packet *Packet);

uint32_t packetAirTime();
uint32_t wakeupTime();

} // namespace SUBGHZRadio

} // namespace CatTracker

#endif // CAT_TRACKER_SUBGHZ_RADIO_H
