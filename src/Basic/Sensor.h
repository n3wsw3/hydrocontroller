#pragma once

struct SensorData {
  uint32_t waterLevel;
  uint32_t temperature;
  uint32_t ec;
  uint32_t ph;
};

/**
 * @brief Binary to Hex, Nullterminated c-string, allocates memory
 * 
 * @param key 
 * @return uint8_t* 
 */
char* btox(uint8_t const* key)
{
  uint8_t offset = 0;
  char* keyStr = (char*) malloc(2 * sizeof(SensorData) + 1);

  for (size_t i = 0; i < sizeof(SensorData); i++)
  {
    offset += sprintf(keyStr + offset, "%02X", key[i]);
  }
  sprintf(keyStr + offset, "%c", '\0');

  return keyStr;
}

int HexAsciiToDec(int value)
{
  if (value > 47 && value < 59)
  {
    value -= 48;
  }
  else if (value > 96 && value < 103)
  {
    value -= 97;
    value += 10;
  }
  else if (value > 64 && value < 71)
  {
    value -= 65;
    value += 10;
  }
  else
  {
    value = 0;
  }
  return value;
}

/**
 * @brief Hex to Binary, allocates memory
 * 
 * @param HexString 
 * @param size 
 * @return uint8_t* 
 */
uint8_t* htob(char* HexString, int size)
{
  uint8_t* returnBytes;
  returnBytes = (uint8_t*)malloc(size);
  int j = 0;

  for (int i = 0; i < size*2; i++)
  {
    if (i % 2 == 0)
    {
      int valueHigh = (int)(*(HexString + i));
      int valueLow = (int)(*(HexString + i + 1));

      valueHigh = HexAsciiToDec(valueHigh);
      valueLow = HexAsciiToDec(valueLow);

      valueHigh *= 16;
      int total = valueHigh + valueLow;
      *(returnBytes + j++) = (uint8_t)total;
    }
  }
  return returnBytes;
}
