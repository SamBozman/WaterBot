#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include "FS.h"
#include <LittleFS.h>

#define Debug 1 // set to 0 if Serial prints not needed

#if Debug == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)   // defined to nothing
#define debugln(x) // defined to nothing
#endif

// Global Variable to hold serialized water target object
char g_output[256];

#define FORMAT_LITTLEFS_IF_FAILED true

// // All function declarations go here
// void makeWaterTarget(int id, String desc, int Hs, int Hf, int Vs, int Vf, int spray, int rwt, bool W_on);
// void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
// void createDir(fs::FS &fs, const char *path);
// void removeDir(fs::FS &fs, const char *path);
// void readFile(fs::FS &fs, const char *path);
// void writeFile(fs::FS &fs, const char *path, const char *message);
// void appendFile(fs::FS &fs, const char *path, const char *message);
// void renameFile(fs::FS &fs, const char *path1, const char *path2);
// void deleteFile(fs::FS &fs, const char *path);
// // SPIFFS-like write and delete file,
// // better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
// void writeFile2(fs::FS &fs, const char *path, const char *message);
// void deleteFile2(fs::FS &fs, const char *path);
// void testFileIO(fs::FS &fs, const char *path);
// void mountLFS();