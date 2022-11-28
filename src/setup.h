#pragma once
#include "Steppers.h"

void setup()
{
    Serial.begin(115200);

    mountLFS(); //* Return message to app if failed?
    ESP_BT.begin("ESP32_Control"); // Name of your Bluetooth interface
    if (!isDir(LittleFS, "/MAX", 1)) // used to store stepper max positions
        createDir(LittleFS, "/MAX");

    homeStepper(Hstepper, hHomePin);
    // homeStepper(Vstepper, vHomePin);
    // homeStepper(Sstepper, sHomePin);

    // Shutoff water
    // TODO start a timer to shut down steppers after a specific time period.
}