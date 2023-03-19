#include <DHT.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include "time.h"
#include "esp_adc_cal.h"

#define DHTPIN 4 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

// Setting SSID and Password
const char* ssid = "WIND_2.4G_9F29CF";
const char* password = "bkYA7AAYGE";
//request the time from pool.ntp.org
const char* ntpServer = "pool.ntp.org";
//Change the time gmtOffset_sec variable to match your time zone (2 for Greece)
const long gmtOffset_sec = 2;
/*The daylightOffset_sec variable defines the offset in seconds for daylight saving time. 
It is generally one hour, that corresponds to 3600 seconds*/
const int daylightOffset_sec = 3600;
// reads the input on analog pin A0 (value between 0 and 1023)
int analogValue = analogRead(A0);


void setup(){
  Serial.begin(115200);

  //Start DHT sensor
  dht.begin();

  // Connect to Wi-Fi
  connectToWiFi();
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  Serial.println();
}

void loop() {
  delay(9000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat inde  x in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(t);
  Serial.print(F("°C  "));
  Serial.print(h);
  Serial.print(F("%  "));

  lightReading();

  printLocalTime();

  getEspInfo();
  get_network_info();

  Serial.println();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%H:%M:%S %d-%m-%Y");
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}

void get_network_info(){
  Serial.print(WiFi.macAddress());
  Serial.print("  ");
  Serial.print(WiFi.localIP());
}

void lightReading() {
  // We'll have a few threshholds, qualitatively determined
  if (analogValue < 10) {
    Serial.println(" Dark");
  } else if (analogValue < 200) {
    Serial.println("Dim");
  } else if (analogValue < 500) {
    Serial.println("Light");
  } else if (analogValue < 800) {
    Serial.println("Bright");
  } else {
    Serial.println("Very bright");
  }
}

void getEspInfo() {

String chipId;

esp_chip_info_t chip_info;
esp_chip_info(&chip_info);
 
//get chip id
chipId = String((uint32_t)ESP.getEfuseMac(), HEX);
chipId.toUpperCase();
 
Serial.printf("ESP32 Devkit v1 %s\n ", chipId.c_str());
Serial.println();
}

