#pragma once
#include "functions.h"

void setup() {
  Serial.begin(115200);
  mountLFS();
  
  ESP_BT.begin("ESP32_Control"); // Name of your Bluetooth interface -> will show up on your phone
  debugln("Stepper Setup");

  
  //homeStepper(Hstepper, hHomePin);
  //homeStepper(Vstepper, vHomePin);
  homeStepper(Sstepper, sHomePin);
 
  // Shutoff water
  // Set homing for each stepper
  // TODO start a timer to shut down steppers after a specific time period.
  // Used to test writing and deleting test files
  // int num_files = 10;
  // delay(1000);
  // createTestFiles(num_files);
  // delay(1000);
  // removeTestFiles(num_files);


}