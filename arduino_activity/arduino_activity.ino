
// Arduino code to measure accelerometer and gyroscope values and send them via BLE to an Android device

#include <ArduinoBLE.h>
#include "Arduino_BMI270_BMM150.h"

BLEService accelService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create a service
BLECharacteristic accelChar("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify, 6); // create a characteristic for the accelerometer data

void setup() {
  Serial.begin(9600);
  
  // initialize the BLE module
  if (!BLE.begin()) {
    Serial.println("Failed to start BLE module!");
    while (1);
  }
  
  // initialize the accelerometer sensor
  if (!IMU.begin()) {
    Serial.println("Failed to start accelerometer!");
    while (1);

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
  }
  
  // set up the service and characteristic
  BLE.setLocalName("Accelerometer");
  BLE.setAdvertisedService(accelService);
  accelService.addCharacteristic(accelChar);
  BLE.addService(accelService);
  accelChar.writeValue("000000");
  
  // start advertising the service
  BLE.advertise();
}

void loop() {
  float x, y, z;
  // read the accelerometer data
  // nrf_pwm_event_t accelEvent;
  // IMU.readAccelerometer(accelEvent);

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  
  // format the data as a string
  String data = String(x) + "," + String(y) + "," + String(z);
  
  // send the data over BLE
  accelChar.writeValue(data.c_str());
  
  // display the data on the Serial Monitor
  Serial.println(data);
  
  // delay for a short period of time before reading the accelerometer again
  delay(10);
  }
}
