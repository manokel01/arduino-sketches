/* 
 * Arduino Nano 33 BLE Sense rev2 measures the following:
 * - temperature (±0.1°C)/ HS3003 sensor
 * - humidity (%RH)/ HS3003 sensor
 * - barometric pressure (260-1260 hPa) / LPS22HB sensor
 * - proximity (0-255)/ APDS9960 sensor
 * - light intensity (0-4097) / APDS9960 sensor
 * - RGB levels (0-4097) / APDS9960 sensor
 * - tilt/position (x, y axes) / BMI270_BMM150 sensors
 * - time elapsed (milliseconds)
 *
 * A timestamp (in ISO 8601 fomrat) is attached to the data. The time is synced to the Current Epoch Unix Timestamp
 * by sending the ltter T followed by ten digit time (as seconds since Jan 1 1970).
 *
 * Data with timestamp and device id are serialized and sent via serial as Json objects.
 */

#include <TimeLib.h>
#include <Arduino_HS300x.h>
#include <Arduino_LPS22HB.h>
#include "Arduino_BMI270_BMM150.h"
#include <Arduino_APDS9960.h>
#include <string.h>
#include <PDM.h>

// Demonstrates conversion
// from character array to string

#define TIME_HEADER "T"  // Header tag for serial time sync message
#define TIME_REQUEST 7   // ASCII bell character requests a time sync message

double temperature;
double humidity;
double pressure;
int proximity;
int r, g, b, light;  // red, green, blue, light intensity
int degreesX = 0;
int degreesY = 0;
long timeElapsed;
char timestampString[25];  // big enough for your longest string, including a null terminator
String deviceID = "ArduinoNano";

void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(13, OUTPUT);
  setSyncProvider(requestSync);  //set function to call when sync required
  Serial.println("To sync device send 'T + Current Epoch Unix Timestamp'");
  if (!HS300x.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1)
      ;
  }
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1)
      ;
  }
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor.");
  }
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
}

void loop() {
  timeElapsed = millis();
  // read temperature, humidity, pressure
  temperature = (int)(HS300x.readTemperature() * 100 + 0.5) / 100.0;  // rounding the float to a two decimal int
  humidity = (int)(HS300x.readHumidity() * 100 + 0.5) / 100.0;        // rounding the float to a two decimal int
  pressure = (int)(BARO.readPressure() * 100 + 0.5) / 100.0;          // rounding the float to a two decimal int

  // read the gyroscope
  float x, y, z;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    if (x > 0.1) {
      x = 100 * x;
      degreesX = map(x, 0, 97, 0, 90);
    }
    if (x < -0.1) {
      x = 100 * x;
      degreesX = (-1) * (map(x, 0, -100, 0, 90));
    }
    if (y > 0.1) {
      y = 100 * y;
      degreesY = map(y, 0, 97, 0, 90);
    }
    if (y < -0.1) {
      y = 100 * y;
      degreesY = (-1) * (map(y, 0, -100, 0, 90));
    }
  }

  // check if a color reading is available
  while (!APDS.colorAvailable()) {
    delay(5);
  }
  // read the color
  APDS.readColor(r, g, b, light);
  if (r > g & r > b) {
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
  } else if (g > r & g > b) {
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDB, HIGH);
  } else if (b > g & b > r) {
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
  } else {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
  }
  // read the proximity
  proximity = APDS.readProximity();

  if (Serial.available()) {
    processSyncMessage();
  }
  if (timeStatus() != timeNotSet) {
    // timestamp as ISO 8601 string date format
    snprintf(timestampString, sizeof(timestampString), "%4d-%02d-%02dT%2d:%2d:%2d+0200", year(), month(), day(), hour(), minute(), second());
  }

  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  // Serial.print(pressure);
  // Serial.print(",");
  // Serial.print(proximity);
  // Serial.print(",");
  // Serial.print(degreesX);
  // Serial.print(",");
  // Serial.print(degreesY);
  // Serial.print(",");
    Serial.print(light);
  // Serial.print(",");
  // Serial.print(r);
  // Serial.print(",");
  // Serial.print(g);
  // Serial.print(",");
  // Serial.print(b);
  // Serial.print(",");
  // Serial.print(timeElapsed);
  // Serial.print(",");
  // Serial.print(timestampString);
  // Serial.print(",");
  // Serial.print(deviceID);
  Serial.println();
  
  delay(1000);
}

// set the time utility function
void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600;  // Jan 1 2013
  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    if (pctime >= DEFAULT_TIME) {  // check the integer is a valid time (greater than Jan 1 2013)
      setTime(pctime);             // Sync Arduino clock to the time received on the serial port
    }
  }
}

time_t requestSync() {
  Serial.write(TIME_REQUEST);
  return 0;  // the time will be sent later in response to serial mesg
}
