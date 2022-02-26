#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

class EspNowSerialBase {
public:
    static const char* NEWLINE;
    static const uint8_t BROADCAST_ADDRESS[ESP_NOW_ETH_ALEN];

    EspNowSerialBase(void);
    virtual ~EspNowSerialBase(void);

    virtual const uint8_t* const getAddress(void) const;
    virtual bool begin(HardwareSerial& serial = Serial);
    virtual bool setChannel(const uint8_t channel);

    virtual size_t write(const char* data, size_t len);
    virtual size_t printf(const char* format, ...);
    virtual size_t print(const char* str);
    virtual size_t println(const char* str);
    virtual size_t println(void);
    virtual void printMacAddress(const uint8_t* macAddr);
    virtual void dump(const uint8_t* data, size_t len, uint8_t indent,
                      uint8_t width);

    virtual bool registerPeer(const uint8_t* addr);
    virtual bool unregisterPeer(const uint8_t* addr);

    virtual void onReceived(const uint8_t* addr, const uint8_t* data,
                            int len) = 0;
    virtual void onSent(const uint8_t* addr, esp_now_send_status_t status) = 0;

protected:
    virtual void initEspNow(void);
    virtual void initPeer(const uint8_t* addr, esp_now_peer_info_t& peer);
    virtual size_t send(const char* data, size_t len);
    virtual bool setRecvCallback(esp_now_recv_cb_t cb);
    virtual bool setSendCallback(esp_now_send_cb_t cb);

private:
    HardwareSerial* _serial;
    uint8_t _channel;
    uint8_t _address[ESP_NOW_ETH_ALEN];
    char _buf[ESP_NOW_MAX_DATA_LEN];
};
