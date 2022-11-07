#pragma once
#include "testFunctions.h"

//********************************************************************
void doStepLoop() {
  boolean pos = Stepper.speed() >= 0 ? true : false;

  if (pos == true && (maxPosition - Stepper.currentPosition() <= 200)) {
    Stepper.setMaxSpeed(200);
    Stepper.setAcceleration(100.0);  // this makes motor stop much quicker!
    Stepper.runToNewPosition(maxPosition);
    debug("Position after maxPosition: ");
    debugln(Stepper.currentPosition());

  } else if (pos == false && (Stepper.currentPosition() <= 200)) {
    Stepper.setMaxSpeed(200);
    Stepper.setAcceleration(100.0);  // this makes motor stop much quicker!
    Stepper.runToNewPosition(home);
    debug("Position after home: ");
    debugln(Stepper.currentPosition());

  } else {
    Stepper.setMaxSpeed(300);
    Stepper.runSpeed();
  }
}

//********************************************************************
void processStepper(float incoming) {
  float stepperSpeed;

  if (incoming <= 65 && incoming >= 55) {
    stepperSpeed = 0;
    ESP_BT.write(stepperSpeed);  // Write stepper speed back to cell phone app
    exit;
  } else {
    if (incoming > 65) {
      stepperSpeed = pow(1.12, incoming - 60);
      ESP_BT.write(stepperSpeed);  // Write stepper speed back to cell phone app
    }

    if (incoming < 55) {
      stepperSpeed = -(pow(1.12, -(incoming - 60)));
      ESP_BT.write(-stepperSpeed);  // Write stepper speed back to cell phone
                                    // app
    }
  }

  Stepper.setSpeed(stepperSpeed);
}

//********************************************************************
void processIncoming(float incoming) {
  int result = (int)floorf(incoming);
  switch (result) {
    case 201:
      Stepper = Hstepper;
      ESP_BT.write(result);  // Call back to app
      debugln("Sending 201");
      break;
    case 202:
      Stepper = Vstepper;
      ESP_BT.write(result);  // Call back to app
      debugln("Sending 202");
      break;
    case 203:
      Stepper = Sstepper;
      ESP_BT.write(result);  // Call back to app
      debugln("Sending 203");
      break;

    default:
      processStepper(incoming);
      break;
  }
  Stepper.setMaxSpeed(300);
  Stepper.setAcceleration(200.0);
}

//********************************************************************
void homeStepper(AccelStepper Stepper, int homePin) {
  int move_finished = 1;     // Used to check if move is completed
  long initial_homing = -1;  // Used to Home Stepper at startup
  pinMode(homePin, INPUT_PULLUP);
  Stepper.setMaxSpeed(100);
  Stepper.setAcceleration(25.0);  // 25 steps per second

  Serial.print("Stepper is Homing ");
  Serial.print("Micro Switch is: ");
  Serial.println(digitalRead(homePin));

  while (digitalRead(homePin))  // Do WHILE switch not activated (NOT true)
  {
    Serial.print("Micro Switch is: ");
    Serial.println(digitalRead(homePin));
    // Make the Stepper move CCW until the switch is activated
    Stepper.moveTo(initial_homing);  // Set the position to move to
    initial_homing--;                // Decrease by 1 for next move if needed
    Stepper.run();                   // Start moving the stepper
    delay(5);
  }
  // ****DO NOT USE setCurrentPosition() WHILE STEPPER IS IN MOTION
  // ***************
  Stepper.setCurrentPosition(0);  // Set the current position as zero for now

  Stepper.setMaxSpeed(
      100.0);  // Set Max Speed of Stepper (Slower to get better accuracy)
  Stepper.setAcceleration(25.0);  // Set Acceleration of Stepper
  initial_homing = 1;

  while (!digitalRead(
      homePin)) {  // Make the Stepper move CW until the switch is deactivated
    Stepper.moveTo(initial_homing);
    Stepper.run();
    initial_homing++;
    delay(5);
  }
  Serial.println("Homing Completed");
  Serial.println("");
  Stepper.setCurrentPosition(0);
}

//********************************************************************
void makeWaterTarget(int id, String name, long Hs, long Hf, long Vs, long Vf,
                     long spray, long rwt, bool W_on) {
  const int capacity = 256;          // Buffer for StaticJsonDocument doc
  StaticJsonDocument<capacity> doc;  // Declare StaticJsonDocument doc

  doc["ID"] = id;  //(Int) ID  for watering target
  doc["Name"] = name;
  doc["H_Steps"] = Hs;        //(int) Horizontal steps from 'Home'
  doc["H_Fluctuation"] = Hf;  //(int) back and forth fluctuation for coverage
  doc["V_Steps"] = Vs;        //(int) Vertical steps from 'Home'
  doc["V_Fluctuation"] = Vf;  //(int) up and down fluctuation for coverage
  doc["Spray"] = spray;       //(int) Spray steps for diffused spray
  doc["RWT"] = rwt;           // Relative watering time (1-5)
  doc["Water_On?"] =
      W_on;  // Turn water off ? (or leave on while moving to next spot)
  // Serialize the Json object and put into variable (output)
  serializeJson(doc, g_output);
}

//********************************************************************
void mountLFS() {
  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    debugln("LittleFS Mount Failed");
    return;
  }
}

//********************************************************************
int listFiles(fs::FS &fs, const char *dirname, uint8_t levels, int numFiles) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    debugln("- failed to open directory");
    return 0;
  }
  if (!root.isDirectory()) {
    debugln(" - not a directory");
    return 0;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      debug("  DIR : ");
      debugln(file.name());
      if (levels) {
        debug("Levels = : ");
        debugln(levels);
        numFiles = listFiles(fs, file.path(), levels - 1, numFiles);
      }
    } else {
      debug("  FILE: ");
      debug(file.name());
      debug("\tSIZE: ");
      debugln(file.size());
      numFiles++;
    }
    file = root.openNextFile();
  }

  return numFiles;
}

//********************************************************************
void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    debugln("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    debugln(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      debug("  DIR : ");
      debugln(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      debug("  FILE: ");
      debug(file.name());
      debug("\tSIZE: ");
      debugln(file.size());
    }
    file = root.openNextFile();
  }
}

//********************************************************************
void createDir(fs::FS &fs, const char *path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    debugln("Dir created");
  } else {
    debugln("mkdir failed");
  }
}

//********************************************************************
void removeDir(fs::FS &fs, const char *path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    debugln("Dir removed");
  } else {
    debugln("rmdir failed");
  }
}

//********************************************************************
void readFile(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    debugln("- failed to open file for reading");
    return;
  }

  debug("- read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  debugln("");
  file.close();
}

//********************************************************************
void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    debugln("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    debugln("- file written");
  } else {
    debugln("- write failed");
  }
  file.close();
}

//********************************************************************
void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    debugln("- failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    debugln("- message appended");
  } else {
    debugln("- append failed");
  }
  file.close();
}

//********************************************************************
void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    debugln("- file renamed");
  } else {
    debugln("- rename failed");
  }
}

//********************************************************************
void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    debugln("- file deleted");
  } else {
    debugln("- delete failed");
  }
}

// SPIFFS-like write and delete file, better use #define
// CONFIG_LITTLEFS_SPIFFS_COMPAT 1

//********************************************************************
void writeFile2(fs::FS &fs, const char *path, const char *message) {
  if (!fs.exists(path)) {
    if (strchr(path, '/')) {
      Serial.printf("Create missing folders of: %s\r\n", path);
      char *pathStr = strdup(path);
      if (pathStr) {
        char *ptr = strchr(pathStr, '/');
        while (ptr) {
          *ptr = 0;
          fs.mkdir(pathStr);
          *ptr = '/';
          ptr = strchr(ptr + 1, '/');
        }
      }
      free(pathStr);
    }
  }

  Serial.printf("Writing file to: %s\r\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    debugln("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    debugln("- file written");
  } else {
    debugln("- write failed");
  }
  file.close();
}

//********************************************************************
void deleteFile2(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

  if (fs.remove(path)) {
    debugln("- file deleted");
  } else {
    debugln("- delete failed");
  }

  char *pathStr = strdup(path);
  if (pathStr) {
    char *ptr = strrchr(pathStr, '/');
    if (ptr) {
      Serial.printf("Removing all empty folders on path: %s\r\n", path);
    }
    while (ptr) {
      *ptr = 0;
      fs.rmdir(pathStr);
      ptr = strrchr(pathStr, '/');
    }
    free(pathStr);
  }
}

//********************************************************************
void testFileIO(fs::FS &fs, const char *path) {
  Serial.printf("Testing file I/O with %s\r\n", path);

  static uint8_t buf[512];
  size_t len = 0;
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    debugln("- failed to open file for writing");
    return;
  }

  size_t i;
  debug("- writing");
  uint32_t start = millis();
  for (i = 0; i < 2048; i++) {
    if ((i & 0x001F) == 0x001F) {
      debug(".");
    }
    file.write(buf, 512);
  }
  debugln("");
  uint32_t end = millis() - start;
  Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
  file.close();

  file = fs.open(path);
  start = millis();
  end = start;
  i = 0;
  if (file && !file.isDirectory()) {
    len = file.size();
    size_t flen = len;
    start = millis();
    debug("- reading");
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      if ((i++ & 0x001F) == 0x001F) {
        debug(".");
      }
      len -= toRead;
    }
    debugln("");
    end = millis() - start;
    Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
    file.close();
  } else {
    debugln("- failed to open file for reading");
  }
}