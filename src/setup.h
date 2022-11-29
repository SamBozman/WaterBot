#pragma once
#include "Steppers.h"

void setup()
{

    Serial.begin(115200);

    mountLFS(); //* Return message to app if failed?
    ESP_BT.begin("ESP32_Control"); // Name of your Bluetooth interface
    delay(5000);
    // directory used to store stepper max positions
    if (!isDir(LittleFS, "/MAX", 1))
        createDir(LittleFS, "/MAX");

    // directoryused to Target data
    if (!isDir(LittleFS, "/TARGETS", 1))
        createDir(LittleFS, "/TARGETS");

    // homeStepper(Hstepper, hHomePin);
    //  homeStepper(Vstepper, vHomePin);
    //  homeStepper(Sstepper, sHomePin);

    // Shutoff water
    // TODO start a timer to shut down steppers after a specific time period.
}