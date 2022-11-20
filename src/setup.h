#pragma once
#include "Steppers.h"

void setup()
{
    Serial.begin(115200);

    mountLFS();                    //* Return message to app if failed?
    ESP_BT.begin("ESP32_Control"); // Name of your Bluetooth interface

    if (!isDir(LittleFS, "/WB_Dir", 1))
    {
        createDir(LittleFS, "/WB_Dir");
    }

    homeStepper(Hstepper, hHomePin);
    homeStepper(Vstepper, vHomePin);
    homeStepper(Sstepper, sHomePin);

    debug("Horizontal currentPosition is : ");
    debugln(Hstepper.currentPosition());
    debug("Vertical currentPosition is : ");
    debugln(Vstepper.currentPosition());
    debug("Spray currentPosition is : ");
    debugln(Sstepper.currentPosition());
    // Shutoff water
    // TODO start a timer to shut down steppers after a specific time period.
}