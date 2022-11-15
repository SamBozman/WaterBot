#include "setup.h"

void loop()
{
    if (ESP_BT.available())
    {
        int incoming = ESP_BT.read(); // Read byte from cell-phone
        processIncoming(incoming);
    }
    switch (stepperNum)
    {
    case 1:
        doStepLoop(Hstepper);
        break;
    case 2:
        doStepLoop(Vstepper);
        break;
    case 3:
        doStepLoop(Sstepper);
        break;
    default:
        break;
    }
}