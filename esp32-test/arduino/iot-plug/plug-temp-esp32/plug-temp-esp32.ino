 /**
 * Measures temperature with Doit EPS32 Devkit v1 board and LM35 sensor. 
 * https://www.electronicshub.org/esp32-pinout/
 * Prints results in Celsius and Fahrenheit with timestamp.
 * Requests time from NTP server

 @author Khaled Magdy (Temp Sensor - https://deepbluembedded.com/esp32-temperature-sensor-lm35-interfacing-arduino/)
 @author Rui Santos (NTP time - https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/)
 @author Manolis Kelaidis
 */
 
#include <WiFi.h>
#include "time.h"
#include "esp_adc_cal.h"

#define LM35_Sensor1    35

// Setting SSID and Password
const char* ssid = "WIND_2.4G_9F29CF";
const char* password = "bkYA7AAYGE";

//Time Request
//request the time from pool.ntp.org
const char* ntpServer = "pool.ntp.org";
//Change the time gmtOffset_sec variable to match your time zone (2 for Greece)
const long gmtOffset_sec = 2;
/*The daylightOffset_sec variable defines the offset in seconds for daylight saving time. 
It is generally one hour, that corresponds to 3600 seconds*/
const int daylightOffset_sec = 3600;

//Temp sensor variables
int LM35_Raw_Sensor1 = 0;
float LM35_TempC_Sensor1 = 0.0;
float LM35_TempF_Sensor1 = 0.0;
float Voltage = 0.0; 

void setup(){
  Serial.begin(115200);

// Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop(){
  delay(10000);
  printLocalTime();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print("Time |");
  Serial.println(&timeinfo, " %H:%M:%S %d-%m-%Y ");

  //Temp Sensor Section
  // Read LM35_Sensor1 ADC Pin
  LM35_Raw_Sensor1 = analogRead(LM35_Sensor1);  
  // Calibrate ADC & Get Voltage (in mV)
  Voltage = readADC_Cal(LM35_Raw_Sensor1);
  // TempC = Voltage(mV) / 10
  LM35_TempC_Sensor1 = Voltage / 10;
  LM35_TempF_Sensor1 = (LM35_TempC_Sensor1 * 1.8) + 32;

  
 
  // Print The Temp Readings
  Serial.print("Temp | ");
  Serial.print(LM35_TempC_Sensor1);
  Serial.print(" °C  ");
  Serial.print(LM35_TempF_Sensor1);
  Serial.println(" °F");

  // Print Device ID
  Serial.println(WiFi.localIP());
  Serial.println();
}

 

uint32_t readADC_Cal(int ADC_Raw)
{
  esp_adc_cal_characteristics_t adc_chars;
  
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
  return(esp_adc_cal_raw_to_voltage(ADC_Raw, &adc_chars));
}
