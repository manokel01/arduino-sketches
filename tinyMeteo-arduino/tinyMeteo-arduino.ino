#include <Arduino_LPS22HB.h>
#include <Arduino_HS300x.h>
#include <EloquentTinyML.h>
#include "eloquent_tinyml/tensorflow.h"
#include <Array.h>
#include "model.h"

#define NUMBER_OF_INPUTS 3
#define NUMBER_OF_OUTPUTS 5
#define TENSOR_ARENA_SIZE 4*1024

Eloquent::TinyML::TensorFlow::TensorFlow<NUMBER_OF_INPUTS, NUMBER_OF_OUTPUTS, TENSOR_ARENA_SIZE> ml;

void setup() {
  Serial.begin(115200);

  Serial.println("Communication started");
  if (!HS300x.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
  ml.begin(model);
}

void loop() {
  float temperature = HS300x.readTemperature();
  float humidity = HS300x.readHumidity();
  float pressure = BARO.readPressure() * 10;

  float input[NUMBER_OF_INPUTS] = {temperature, humidity, pressure};
  float output[NUMBER_OF_OUTPUTS] = {0, 0, 0, 0, 0};
  ml.predict(input, output);

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.print(" °C - Humidity : ");
  Serial.print(humidity);
  Serial.print(" % - Pressure : ");
  Serial.println(pressure);
  Serial.print("The weather is : ");
  Serial.println(value(output));
  delay(1000);
}

String value(float out[5]) {
  Array<float> my_classes = Array<float>(out, NUMBER_OF_OUTPUTS);
  if (my_classes.getMax() == 0) {
    return "Clear";
  } else if (my_classes.getMax() == 1) {
    return "Cloudy";
  } else if (my_classes.getMax() == 2) {
    return "Moderate rain at times";
  } else if (my_classes.getMax() == 3) {
    return "Heavy rain at times";
  } else {
    return "Freezing rain / snow";
  }
}