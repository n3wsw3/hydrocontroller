#include "Arduino.h"
#include "Sensor.h"

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600, SERIAL_8N1); // 8 data bits, no parity, 1 stop bit
}

// the loop routine runs over and over again forever:
void loop() {
  SensorData sd;
  sd.waterLevel = millis();
  sd.temperature = 35;
  sd.ph = 45;
  sd.ec = 1100;

  sendData(&sd);

  delay(30000);
}

void sendData(SensorData* sd) {
  char* data = btox((uint8_t*)sd);

  char buff[15+sizeof(SensorData)];
  sprintf(buff, "<SensorData_%s> ", data);

  Serial.print(buff);
  free(data);
}