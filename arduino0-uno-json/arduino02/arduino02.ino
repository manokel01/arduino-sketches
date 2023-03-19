/* 
 * Arduino UNO measures the following:
 * - light intensity (0-1023) / photoresistor
 * - temperature (±1°C)/ HDT11 module
 * - humidity (±1%RH)/ HDT11 module
 * - time elapsed (milliseconds)
 *
 * Data and device id are serialized and sent via serial as Json objects.
 * 
 * @author manokel01
 */

#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>

#define DHTPIN 4          // what pin we're connected to
#define DHTTYPE DHT11     // DHT 11 
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino
StaticJsonDocument<128> doc;

void setup(){
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(1000);
  // Reading temperature or humidity takes about 250 milliseconds.
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
  // reads the input on analog pin A0 (value between 0 and 1023)
  int analogValue = analogRead(A0);
  doc["light"] = analogValue;
  doc["temperature"] = t;
  doc["humidity"] = h;
  doc["time"] = millis();
  doc["deviceID"] = "ArduninoUNO";
  serializeJson(doc, Serial);
  Serial.println();
}

