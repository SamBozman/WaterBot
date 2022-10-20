#pragma once
#include "j_son.h"

void setup()
{
  Serial.begin(115200);
  mountLFS(); // Located in 'other_functions.h'

  createDir(LittleFS, "/mydir");
  writeFile(LittleFS, "/mydir/hello2.txt", "Hello2");
  listDir(LittleFS, "/", 1);
  deleteFile(LittleFS, "/mydir/hello2.txt");
  removeDir(LittleFS, "/mydir");
  listDir(LittleFS, "/", 1);
}