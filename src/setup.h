#pragma once
#include "j_son.h"

void setup()
{
  Serial.begin(115200);
  mountLFS(); // Located in 'other_functions.h'
  listDir(LittleFS, "/", 1);
  createDir(LittleFS, "/mydir");
  writeFile(LittleFS, "/mydir/hello2.txt", "Hello2");
  writeFile(LittleFS, "/mydir/hello3.txt", "Hello3");
  readFile(LittleFS, "/mydir/hello2.txt");
  readFile(LittleFS, "/mydir/hello3.txt");
  deleteFile(LittleFS, "/mydir/hello2.txt");
  deleteFile(LittleFS, "/mydir/hello3.txt");
  removeDir(LittleFS, "/mydir");
  listDir(LittleFS, "/", 1);
}