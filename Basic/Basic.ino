#include "Arduino.h"
#include "ArduinoJson.h"
#include "Handler.h"
#define DEBUG
#include "Debug_Log.h"

int pumps[] = {2, 3, 4, 5, 6};

double water_level = 0;
double temperature = 25;
double ph = 0;

// This "ghetto" multi-threading solution works ONLY if the combined running time of all handler functions
// is less than the lowest wait-time.
Handler handlers[] = {{"Measure EC", &measure_EC, 40U}, {"Update Sensor Data", &update_sensor_data, 1000}, {"Send Data", &send_data, 30UL * 60 * 1000}};

#define ecPin A1

void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600); // 8 data bits, no parity, 1 stop bit

  // Initialize pump pins;
  for (int pin : pumps)
  {
    Log((String) "Initializing pump: " + pin);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  pinMode(ecPin, INPUT);
}

// the loop routine runs over and over again forever:
void loop()
{
  for (auto& handler : handlers)
  {
    if (handler.should_run())
    {
      Log((String) "Running handler: " + handler.name);
      handler.run();
    }
  }
}

void update_sensor_data()
{
  measure_waterLevel();
  measure_temperature();
  // measure_EC();
  measure_pH();

  Log((String) "wl: " + water_level + " t: " + temperature + " ec: " + get_EC() + " ph: " + ph);
}

void send_data()
{
  StaticJsonDocument<64> doc;
  doc["waterLevel"] = water_level;
  doc["temperature"] = temperature;
  doc["ec"] = get_EC();
  doc["ph"] = ph;

  serializeJson(doc, Serial);
  Serial.println();
}

void measure_waterLevel()
{
  water_level = 5 * sin(((float)millis()) / 10000) + 25;
}

void measure_temperature()
{
  // temperature = 4 * sin(((float)millis()) / 5000) + 20;
}

#define EC_BUFFER_COUNT 30
#define VREF 5.0f
int ecbuffer[EC_BUFFER_COUNT];
int ecBufferIndex = 0;

// https://web.archive.org/web/20210506145611/https://wiki.keyestudio.com/KS0429_keyestudio_TDS_Meter_V1.0
void measure_EC()
{
  ecbuffer[ecBufferIndex++ % EC_BUFFER_COUNT] = analogRead(ecPin);
}

float get_EC()
{
  float averageVoltage = getMedianNum(ecbuffer, EC_BUFFER_COUNT) * VREF / 1024.0f;
  // Compensate for temperature differences.
  // Currently the temperature won't change since we don't measure temperature
  // But if we wanted to measure temperature in the future, this will automatically
  // change to adapt.
  float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0f);
  float compensationVoltage = averageVoltage / compensationCoefficient;
  // tds ppm
  float tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * 0.5;
  float ec = tdsValue * 1000.0f / 550.0f;
  return ec;
}

void measure_pH()
{
  ph = 1.5 * sin(((float)millis()) / 2000) + 5.5;
}

// VERY FUCKING UGLY CODE!
// Stolen from keyestudio wiki
// Don't blame me.
// I don't know how it works
int getMedianNum(int bArray[], int iFilterLen)
{
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}
