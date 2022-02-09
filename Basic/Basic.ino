#include "Arduino.h"
#include "ArduinoJson.h"

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600, SERIAL_8N1); // 8 data bits, no parity, 1 stop bit
}

// the loop routine runs over and over again forever:
void loop() {
  StaticJsonDocument<64> doc;
  doc["waterLevel"] = measure_waterLevel();
  doc["temperature"] = measure_temperature();
  doc["ec"] = measure_EC();
  doc["ph"] = measure_pH();

  serializeJson(doc, Serial);
  Serial.println();

  delay(5000);
}



double measure_waterLevel() {
  return 5 * sin(((float)millis()) / 10000) + 25;
}

double measure_temperature() {
  return 4 * sin(((float)millis()) / 5000) + 20;
}

double measure_EC() {
  return 2 * sin(((float)millis()) / 1000) + 2;
}

double measure_pH() {
  return 1.5 * sin(((float)millis()) / 2000) + 5.5;
}
