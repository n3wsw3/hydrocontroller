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

int measure_waterLevel() {
  return millis() / 1000;
}

int measure_temperature() {
  return 0;
}

int measure_EC() {
  return 0;
}

int measure_pH() {
  return 0;
}
