#include "EspNowSerial.h"

#include "Singleton.h"

EspNowSerialImpl& EspNowSerial = Singleton<EspNowSerialImpl>::getInstance();

void onDataReceived(const uint8_t* addr, const uint8_t* data, int len) {
    EspNowSerial.onReceived(addr, data, len);
}

void onDataSent(const uint8_t* addr, esp_now_send_status_t status) {
    EspNowSerial.onSent(addr, status);
}

bool EspNowSerialImpl::begin(HardwareSerial& serial) {
    bool result = true;
    result &= EspNowSerialBase::begin(serial);
    result &= setRecvCallback(onDataReceived);
    result &= setSendCallback(onDataSent);
    return result;
}

void EspNowSerialImpl::onReceived(const uint8_t* addr, const uint8_t* data,
                                  int len) {
#if defined(ESPNOW_RESPONDER)
    EspNowSerial.printMacAddress(addr);
    EspNowSerial.print("> ");
    EspNowSerial.write((char*)data, len);
    EspNowSerial.dump(data, len, 2, 16);
#endif
}

void EspNowSerialImpl::onSent(const uint8_t* addr,
                              esp_now_send_status_t status) {
}
