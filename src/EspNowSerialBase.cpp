
#include "EspNowSerialBase.h"

const char* EspNowSerialBase::NEWLINE = "\r\n";
const uint8_t EspNowSerialBase::BROADCAST_ADDRESS[ESP_NOW_ETH_ALEN] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

EspNowSerialBase::EspNowSerialBase(void)
    : _serial(nullptr), _channel(0), _address{0}, _buf{0} {
    uint8_t addr[ESP_NOW_ETH_ALEN] = {0};
    esp_read_mac(addr, ESP_MAC_WIFI_STA);
    memcpy(this->_address, addr, ESP_NOW_ETH_ALEN);
}

EspNowSerialBase::~EspNowSerialBase(void) {
}

const uint8_t* const EspNowSerialBase::getAddress(void) const {
    return this->_address;
}

bool EspNowSerialBase::begin(HardwareSerial& serial) {
    this->_serial = &serial;
    if (!WiFi.mode(WIFI_STA)) {
        return false;
    }
    if (!WiFi.disconnect()) {
        return false;
    }
    initEspNow();
    return true;
}

bool EspNowSerialBase::setChannel(const uint8_t channel) {
    this->_channel = channel;
    return true;
}

size_t EspNowSerialBase::write(const char* data, size_t len) {
    this->_serial->write(data, len);
#if defined(ESPNOW_INITIATOR)
    len = send(data, len);
#endif
    return len;
}

size_t EspNowSerialBase::printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    va_list args_copy;
    va_copy(args_copy, args);
    char* loc = this->_buf;
    int len = vsnprintf(loc, sizeof(this->_buf), format, args_copy);
    va_end(args_copy);
    if (len < 0) {
        return 0;
    }
    if (len >= sizeof(this->_buf)) {
        loc = (char*)malloc(len + 1);
        if (loc == nullptr) {
            return 0;
        }
        len = vsnprintf(loc, len + 1, format, args);
    }
    write(loc, len);
    if (loc != this->_buf) {
        free(loc);
    }
    return len;
}

size_t EspNowSerialBase::print(const char* str) {
    return printf("%s", str);
}

size_t EspNowSerialBase::println(const char* str) {
    return printf("%s%s", str, NEWLINE);
}

size_t EspNowSerialBase::println(void) {
    return printf("%s", NEWLINE);
}

void EspNowSerialBase::printMacAddress(const uint8_t* macAddr) {
    static char macStr[ESP_NOW_ETH_ALEN * 3 + 1] = {0};
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4],
             macAddr[5]);
    print(macStr);
}

void EspNowSerialBase::dump(const uint8_t* data, size_t len, uint8_t indent,
                            uint8_t width) {
    println();
    printf("Dump Length: %d", len);
    println();
    size_t p = 0;
    for (; p < len; ++p) {
        if (p % width == 0) {
            if (p != 0) {
                print("  |");
                for (size_t cp = p - width; cp < p; ++cp) {
                    printf("%c", isPrintable(data[cp]) ? data[cp] : '.');
                }
                print("|");
                println();
            }
            for (size_t i = 0; i < indent; ++i) {
                print(" ");
            }
        }
        printf("%02x ", data[p]);
    }
    if (len % width != 0) {
        uint8_t remain = width - len % width;
        for (size_t cp = 0; cp < remain; ++cp) {
            print("   ");
        }
        print("  |");
        for (size_t cp = len - len % width; cp < len; ++cp) {
            printf("%c", isPrintable(data[cp]) ? data[cp] : '.');
        }
        print("|");
        println();
    }
}

void EspNowSerialBase::initEspNow(void) {
    if (esp_now_init() != ESP_OK) {
        ESP.restart();
    }
}

void EspNowSerialBase::initPeer(const uint8_t* addr,
                                esp_now_peer_info_t& peer) {
    memset(&peer, 0, sizeof(peer));
    memcpy(&(peer.peer_addr), addr, ESP_NOW_ETH_ALEN);
    peer.channel = this->_channel;
    peer.encrypt = 0;
}

bool EspNowSerialBase::setRecvCallback(esp_now_recv_cb_t cb) {
    esp_err_t e = esp_now_register_recv_cb(cb);
    return e == ESP_OK;
}

bool EspNowSerialBase::setSendCallback(esp_now_send_cb_t cb) {
    esp_err_t e = esp_now_register_send_cb(cb);
    return e == ESP_OK;
}

bool EspNowSerialBase::registerPeer(const uint8_t* addr) {
    esp_now_peer_info_t peerInfo;
    initPeer(addr, peerInfo);
    esp_err_t e = esp_now_add_peer(&peerInfo);
    return e == ESP_OK;
}

bool EspNowSerialBase::unregisterPeer(const uint8_t* addr) {
    esp_err_t e = esp_now_del_peer(addr);
    return e == ESP_OK;
}

size_t EspNowSerialBase::send(const char* data, size_t len) {
    esp_err_t e = ESP_OK;
    esp_now_peer_info_t peer;
    size_t n = 0;
    for (size_t i = 0; i < len; i += n) {
        n = min(sizeof(this->_buf), len - i);
        memcpy(this->_buf, data + i, n);
        e = esp_now_fetch_peer(true, &peer);
        if (e == ESP_ERR_ESPNOW_NOT_FOUND) {  // Broadcast
            esp_now_send(BROADCAST_ADDRESS, (uint8_t*)this->_buf, n);
        } else {
            while (e == ESP_OK) {
                esp_now_send(peer.peer_addr, (uint8_t*)this->_buf, n);
                e = esp_now_fetch_peer(false, &peer);
            }
        }
    }
    return len;
}
