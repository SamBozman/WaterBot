#pragma once
#include "j_son.h"

void setup()
{
  Serial.begin(115200);
  mountLFS();

  debugln("Going to list Directories");
  listDir(LittleFS, "/", 1);
  debugln("Listed Directories");
  debugln("");

  debugln("Going to create files");
  createTestFiles(2);
  debugln("Files created");
  debugln("");

  debugln("Going to list Directories");
  listDir(LittleFS, "/", 1);
  debugln("Listed Directories");
  debugln("");

  debugln("Going to read A1");
  readFile(LittleFS, "/A1");
  debugln("Read A1");
  debugln("");
 
 debugln("Going to read A2");
  readFile(LittleFS, "/A2");
  debugln("Read A2");
  debugln("");

  debugln("Going to remove files");
  removeTestFiles(2);
  debugln("Files removed");

  debugln("Going to list Directories");
  listDir(LittleFS, "/", 1);
  debugln("Listed Directories");
}