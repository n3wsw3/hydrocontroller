#pragma once

#ifdef DEBUG
#define Log(x) Serial.println(x)
#else
#define Log(x)
#endif