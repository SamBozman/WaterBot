#pragma once
#include "functions.h"

void setup()
{
  Serial.begin(115200);
  mountLFS();

  int num_files = 10; // Used to test writing and deleting test files
  delay(1000);
  createTestFiles(num_files);
  delay(1000);
  removeTestFiles(num_files);
}