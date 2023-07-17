#ifndef CAT_TRACKER_PACKET_HANDLER_H
#define CAT_TRACKER_PACKET_HANDLER_H

#include "subghz_radio.h"

namespace CatTracker {

using PacketHandler = decltype(
    static_cast<bool (*)(const SUBGHZRadio::Packet *, SUBGHZRadio::Packet *)>(
        nullptr));

PacketHandler getPacketHandler();

void transmitCompleted();
bool targetIsWokenUp();
uint32_t timeTargetRemainingAwake();
uint32_t timeUntilNextTransmitWindow();

} // namespace CatTracker

#endif // CAT_TRACKER_PACKET_HANDLER_H