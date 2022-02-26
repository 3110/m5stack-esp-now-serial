#include "main.h"

#include "EspNowSerial.h"

#if defined(ESPNOW_BROADCAST)
const uint8_t* peer = EspNowSerialBase::BROADCAST_ADDRESS;
#else
#if defined(ESPNOW_INITIATOR)
const uint8_t peer[ESP_NOW_ETH_ALEN] = {0x4c, 0x75, 0x25, 0xc4, 0xa4, 0x18};
#endif
#if defined(ESPNOW_RESPONDER)
const uint8_t peer[ESP_NOW_ETH_ALEN] = {0xd8, 0xa0, 0x1d, 0x58, 0x67, 0xa4};
#endif
#endif

void setup() {
    M5_BEGIN();
    EspNowSerial.begin();
    EspNowSerial.registerPeer(peer);
}

#if defined(ESPNOW_INITIATOR)
uint32_t counter = 0;
#endif

void loop() {
    M5.update();
#if defined(ESPNOW_INITIATOR)
    if (M5_BTN.wasPressed()) {
        ++counter;
        EspNowSerial.printf("%05d: Hello, world!", counter);
    }
#endif
    delay(100);
}