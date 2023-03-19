#include <ConnectWifi.h>
#include <Arduino.h>
/* 
 * Connect to Wifi
 * @author bellevillain
 * @version 0.1
 */
void setup() {
    Serial.begin(115200);
    ConnectWifi("WIND_2.4G_9F29CF", "bkYA7AAYGE");
}

void loop() {
}
