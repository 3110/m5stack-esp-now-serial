#pragma once

#include "EspNowSerialBase.h"

class EspNowSerialImpl : public EspNowSerialBase {
public:
    virtual bool begin(HardwareSerial& serial = Serial);
    virtual void onReceived(const uint8_t* addr, const uint8_t* data, int len);
    virtual void onSent(const uint8_t* addr, esp_now_send_status_t status);
};

extern EspNowSerialImpl& EspNowSerial;

extern void onDataReceived(const uint8_t* addr, const uint8_t* data, int len);
extern void onDataSent(const uint8_t* addr, const uint8_t* data, int len);
