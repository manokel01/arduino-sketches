#include <DHT.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 4          // what pin we're connected to
#define DHTTYPE DHT11     // DHT 11 
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino

float h = dht.readHumidity();
void setup(){
  Serial.begin(9600);
  while(!Serial);
  dht.begin();
}

void loop() {
  delay(1000);

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
  // reads the input on analog pin A0 (value between 0 and 1023)
  int analogValue = analogRead(A0);

  Serial.print("Light:");
  Serial.print(analogValue);   // the raw analog reading
  Serial.print("  Temperature:");
  Serial.print(t);
  Serial.print("  Humidity:");
  Serial.print(h);
  Serial.println();
}

