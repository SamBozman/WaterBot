#pragma once
#include "Steppers.h"

void setup()
{
    Serial.begin(115200);
    mountLFS(); //* Return message to app if failed?
    ESP_BT.begin("ESP32_Control"); // Name of your Bluetooth interface
    delay(5000); //! Temporary for testing
    // creat directorys for max positions and Target data
    if (!isDir(LittleFS, "/MAX", 1))
        createDir(LittleFS, "/MAX");
    if (!isDir(LittleFS, "/TARGETS", 1))
        createDir(LittleFS, "/TARGETS");

    int numFiles = 0;
    String s;
    numFiles = listFiles(LittleFS, "/MAX", 1, numFiles);
    if (!numFiles == 3) {
        debug("NOT ALL MAXIMUM POSITIONS HAVE BEEN SET: ");
    } else { // load all maximum positions
        s = "/MAX/M1";
        strcpy(path, s.c_str());
        loadMax(path);
        s = "/MAX/M2";
        strcpy(path, s.c_str());
        loadMax(path);
        s = "/MAX/M3";
        strcpy(path, s.c_str());
        loadMax(path);
    }

    // homeStepper(Hstepper, hHomePin);
    // homeStepper(Vstepper, vHomePin);
    // homeStepper(Sstepper, sHomePin);

    // Shutoff water
    // TODO start a timer to shut down steppers after a specific time period.
}