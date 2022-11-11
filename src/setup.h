#pragma once
#include "functions.h"

void setup() {
  Serial.begin(115200);
  mountLFS();
  ESP_BT.begin("ESP32_Control"); // Name of your Bluetooth interface
 
  homeStepper(Hstepper, hHomePin);
  homeStepper(Vstepper, vHomePin);
  homeStepper(Sstepper, sHomePin);

  // Shutoff water
  // TODO start a timer to shut down steppers after a specific time period.

}