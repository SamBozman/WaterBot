#include "setup.h"

void loop()
{
    if (ESP_BT.available()) {
    float incoming = ESP_BT.read();  //Read byte from cell-phone
    processIncoming(incoming);
  }

  doStepLoop(); 
}