#pragma once
#include "J_Funtions.h"


void setup()
{
  Serial.begin(115200);
  makeWaterTarget(2, "This is a Test", 1145, 60, 367, 45, 346, 5, false);

  debugln("Here is the data!");
  debugln(output);           // Print out “minified” JSON document
  serializeJsonPretty(doc, Serial); // Print out “prettified” JSON document
}