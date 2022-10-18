#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>

#define Debug 1 // set to 0 if Serial prints not needed

#if Debug == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)   // defined to nothing
#define debugln(x) // defined to nothing
#endif

char output[256];                 // Global Variable to hold serialized water target object
const int capacity = 256;         // Buffer for StaticJsonDocument doc
StaticJsonDocument<capacity> doc; // Declare StaticJsonDocument doc

void makeWaterTarget(int id, String desc, int Hs, int Hf, int Vs, int Vf, int spray, int rwt, bool W_on);