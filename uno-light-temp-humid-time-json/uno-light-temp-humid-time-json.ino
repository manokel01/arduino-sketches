/* 
 * Arduino UNO measures the following:
 * - light intensity (0-1023) / photoresistor
 * - temperature (±1°C)/ HDT11 module
 * - humidity (±1%RH)/ HDT11 module
 * - time elapsed (milliseconds)
 *
 * Data and device id are sent via serial as Json strings;
 * 
 * @author manokel01
 */

#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <string.h>
#include <ArduinoJson.h>

#define DHTPIN 4          // what pin we're connected to
#define DHTTYPE DHT11     // DHT 11 
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino
int light;
float temperature;
float humidity;
long time;
String deviceID = "ArduinoUNO";

void setup(){
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  time = millis();
  light = analogRead(A0);
  // Reading temperature or humidity takes about 250 milliseconds.
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  // float f = dht.readTemperature(true);
  // float hif = dht.computeHeatIndex(f, h);
  // float hic = dht.computeHeatIndex(t, h, false);
  DynamicJsonDocument doc(200);
  doc["light"] = light;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["time"] = time;
  doc["deviceID"] = deviceID;
  serializeJson(doc, Serial);
  Serial.println();
  delay(1000);
}

