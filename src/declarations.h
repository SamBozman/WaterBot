#pragma once
#include "globals.h"

class target;
void convertStringToInt();
void getJson();
void resetMax(AccelStepper* Stepper);
void setMax(AccelStepper* Stepper);
void loadMax(char* path, long* Maxptr);
void homeStepper(AccelStepper& Stepper, int homePin);
void processIncoming(int incoming);
void processStepper(AccelStepper* Stepper, int incoming);
void doStepLoop(AccelStepper* Stepper, long* MaxPos);

void makeWaterTarget(int id, String name, long Hs, long Hf, long Vs, long Vf, long Ss, long Sf, long rwt, bool W_on);
void listDir(fs::FS& fs, const char* dirname, uint8_t levels);
bool isDir(fs::FS& fs, const char* dirname, uint8_t levels);

int listFiles(fs::FS& fs, const char* dirname, uint8_t levels, int numFiles);
void createDir(fs::FS& fs, const char* path);
void removeDir(fs::FS& fs, const char* path);
void readFile(fs::FS& fs, const char* path);
void writeFile(fs::FS& fs, const char* path, const char* message);
void appendFile(fs::FS& fs, const char* path, const char* message);
void renameFile(fs::FS& fs, const char* path1, const char* path2);
void deleteFile(fs::FS& fs, const char* path);
// SPIFFS-like write and delete file,
// better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
void writeFile2(fs::FS& fs, const char* path, const char* message);
void deleteFile2(fs::FS& fs, const char* path);
void testFileIO(fs::FS& fs, const char* path);
void mountLFS();
void createTestFiles(int n); // Temporary
void removeTestFiles(int n); // Temporary

//(String to char array) Uses global char array 'path'
void stochar(String s);
