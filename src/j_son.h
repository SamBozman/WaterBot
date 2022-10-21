#pragma once
#include "little_FS.h"

void makeWaterTarget(int id, String name, int Hs, int Hf, int Vs, int Vf, int spray, int rwt, bool W_on)
{
  const int capacity = 256;         // Buffer for StaticJsonDocument doc
  StaticJsonDocument<capacity> doc; // Declare StaticJsonDocument doc

  doc["ID"] = id; //(Int) ID  for watering target
  doc["Name"] = name;
  doc["H_Steps"] = Hs;       //(int) Horizontal steps from 'Home'
  doc["H_Fluctuation"] = Hf; //(int) back and forth fluctuation for coverage
  doc["V_Steps"] = Vs;       //(int) Vertical steps from 'Home'
  doc["V_Fluctuation"] = Vf; //(int) up and down fluctuation for coverage
  doc["Spray"] = spray;      //(int) Spray steps for diffused spray
  doc["RWT"] = rwt;          // Relative watering time
  doc["Water_On?"] = W_on;   // Turn water off ? (or leave on while moving to next spot)
  // Serialize the Json object and put into variable (output)
  serializeJson(doc, g_output);
}