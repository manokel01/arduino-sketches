/**
 * ConnectWifi.h - Library for connecting to Wifi.
 * 28/12/2022
 *
 * @author bellevillain
 * @version 0.1
 * @since 1.0
 */

#ifndef ARDUINO_CONNECTWIFI_H
#define ARDUINO_CONNECTWIFI_H
#include <Arduino.h>

class ConnectWifi {
    private:
        const char* ssid;
        const char* password;
    public:
        ConnectWifi(char* ssid, char* password);
};


#endif //ARDUINO_CONNECTWIFI_H
