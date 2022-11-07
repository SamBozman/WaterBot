#pragma once
#include "functions.h"

void setup() {
  Serial.begin(115200);
  mountLFS();

  // ESP_BT.begin("ESP32_Control"); // Name of your Bluetooth interface -> will
  // show up on your phone debugln("Stepper Setup");

  // homeStepper(Hstepper, hHomePin);
  // homeStepper(Vstepper, vHomePin);
  // homeStepper(Sstepper, sHomePin);

  // Shutoff water
  // Set homing for each stepper
  // TODO start a timer to shut down steppers after a specific time period.

  // Used to test writing and deleting test files
  // int num_files = 10;
  // delay(1000);
  // createTestFiles(num_files);
  // delay(1000);
  // removeTestFiles(num_files);
  int Files = 0;
  createDir(LittleFS, "/WB1");
  writeFile(LittleFS, "/WB1/hello2.txt", "Hello2");
  writeFile(LittleFS, "/WB1/hello3.txt", "Hello3");
  Files = listFiles(LittleFS, "/", 1, 0);
  debug("Files: = ");
  debugln(Files);
  deleteFile(LittleFS, "/WB1/hello2.txt");
  deleteFile(LittleFS, "/WB1/hello3.txt");
  removeDir(LittleFS, "/WB1");
  listDir(LittleFS, "/", 1);
}