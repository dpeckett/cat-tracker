#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "packet_handler.h"

namespace CatTracker {

bool handleBeacon(const SUBGHZRadio::Packet *RxPacket,
                  SUBGHZRadio::Packet *TxPacket) {
  auto RequestPayload =
      reinterpret_cast<const SUBGHZRadio::PayloadBeacon *>(RxPacket->Payload);

  printf("Beacon device=0x%08x rssi=%d\n", RequestPayload->DeviceID,
         SUBGHZRadio::getLastRSSI());

  return false;
}

PacketHandler getPacketHandler() {
  return [](const SUBGHZRadio::Packet *RxPacket,
            SUBGHZRadio::Packet *TxPacket) -> bool {
    switch (RxPacket->Kind) {
    case SUBGHZRadio::PacketKind::Beacon:
      return handleBeacon(RxPacket, TxPacket);
    default:
      return false;
    }
  };
}

} // namespace CatTracker