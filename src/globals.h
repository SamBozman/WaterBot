#pragma once

#include <AccelStepper.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoLog.h>
#include <LittleFS.h>

#include "BluetoothSerial.h"  // Bluetooth Serial -> works ONLY on ESP32
#include "FS.h"

#define Debug 1  // set to 0 if Serial prints not needed

#if Debug == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)    // defined to nothing
#define debugln(x)  // defined to nothing
#endif

#define waterPin 2  // Water off-LOW on - HIGH

#define MotorInterfaceType 8
#define home 0            // All steppers homing position is 0
#define maxPosition 2700  // TEMP for testing (needs to be set for each stepper)

// Hall Effect sensor pins
//#define hHomePin 4
//#define vHomePin 16
#define sHomePin 17

AccelStepper Hstepper = AccelStepper(MotorInterfaceType, 26, 33, 25, 32);
AccelStepper Vstepper = AccelStepper(MotorInterfaceType, 13, 14, 12, 27);
AccelStepper Sstepper = AccelStepper(MotorInterfaceType, 5, 19, 18, 21);

BluetoothSerial ESP_BT;  // init Class:
AccelStepper Stepper;  // Declare a variable to hold the current stepper object

#define FORMAT_LITTLEFS_IF_FAILED true
char g_output[256];  // Serialized water target object
char path[10];       // Used by function 'stochar()'

// All function declarations go here
void homeStepper(AccelStepper Stepper, int homePin);
void processIncoming(float incoming);
void processStepper(float incoming);
void doStepLoop();

void makeWaterTarget(int id, String name, long Hs, long Hf, long Vs, long Vf,
                     long spray, long rwt, bool W_on);
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
int listFiles(fs::FS &fs, const char *dirname, uint8_t levels);
void createDir(fs::FS &fs, const char *path);
void removeDir(fs::FS &fs, const char *path);
void readFile(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);
// SPIFFS-like write and delete file,
// better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
void writeFile2(fs::FS &fs, const char *path, const char *message);
void deleteFile2(fs::FS &fs, const char *path);
void testFileIO(fs::FS &fs, const char *path);
void mountLFS();
void createTestFiles(int n);  // Temporary
void removeTestFiles(int n);  // Temporary

//(String to char array) Uses global char array 'path'
void stochar(String s);