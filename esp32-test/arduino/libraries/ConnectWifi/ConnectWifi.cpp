/**
 * ConnectWifi.cpp - Library for connecting to Wifi.
 * 28/12/2022
 *
 * @author bellevillain
 * @version 0.1
 * @since 1.0
 */

#include <WiFi.h>
#include "ConnectWifi.h"
#include <Arduino.h>

ConnectWifi::ConnectWifi(char *ssid, char *password) 
{
  /*Serial.print("Connecting to ");
  Serial.print(ssid);*/
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(WiFi.status()); Serial.print(F(" "));
      delay(500);
      Serial.print("."); }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print(F("MAC Address: "));
  Serial.println(WiFi.macAddress());
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("RSSI: "));
  Serial.println(WiFi.RSSI());
  Serial.println();
}



