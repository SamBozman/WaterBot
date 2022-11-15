
#pragma once
#include "Files.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void homeStepper(AccelStepper &Stepper, int homePin)
{
    int move_finished = 1;    // Used to check if move is completed
    long initial_homing = -1; // Used to Home Stepper at startup
    pinMode(homePin, INPUT_PULLUP);

    delay(5);

    //  Set Max Speed and Acceleration of each Steppers at startup for homing
    Stepper.setMaxSpeed(100.0);     // Set Max Speed of Stepper (Slower to get better accuracy)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper

    // Start Homing procedure of Stepper Motor at startup

    while (digitalRead(homePin)) // Do WHILE switch not activated (NOT true)
    {
        // Make the Stepper move CCW
        // until the switch is activated
        Stepper.moveTo(initial_homing); // Set the position to move to
        initial_homing--;               // Decrease by 1 for next move if needed
        Stepper.run();                  // Non Blocking - move to position
        delay(5);
    }

    Stepper.setCurrentPosition(0);  // Set the current position as zero for now
    Stepper.setMaxSpeed(100.0);     // Set Max Speed of Stepper (Slower to get better accuracy)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper

    initial_homing = 1;
    while (!digitalRead(homePin))
    {
        // Make the Stepper move CW until the
        // until the switch is deactivated
        Stepper.moveTo(initial_homing);
        Stepper.run(); // Non Blocking - move to position
        initial_homing++;
        delay(5);
    }
    Stepper.setCurrentPosition(0);
    Serial.println("Homing Completed");
    Serial.println("");

    int rnum;
    srand(time(0));
    rnum = rand() % (500 + 1 - 100) + 100;
    Stepper.moveTo(rnum);
    Stepper.runToPosition();

    Stepper.setMaxSpeed(200.0);     // Set Max Speed of Stepper (Faster for regular movements)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void processIncoming(int incoming)
{
    switch (incoming)
    {

    case 201:
        stepperNum = 1;         // Hstepper
        ESP_BT.write(appClear); // reset slider poition to 100 (center) & clear buttons
        ESP_BT.write(incoming); // Set app to use Hstepper
        debugln("Sending 201 back to app");
        break;
    case 202:
        stepperNum = 2;         // Vstepper
        ESP_BT.write(appClear); // reset slider poition to 100 (center) & clear buttons
        ESP_BT.write(incoming); // Set app to use Vstepper
        debugln("Sending 202 back to app");
        break;
    case 203:
        stepperNum = 3;         // Sstepper
        ESP_BT.write(appClear); // reset slider poition to 100 (center) & clear buttons
        ESP_BT.write(incoming); // Set app to use Sstepper
        debugln("Sending 203 back to app");
        break;
    case 204:                   //! Will eventually be used to control water on/off
        stepperNum = 0;         // No Stepper used here
        ESP_BT.write(appClear); // reset slider poition to 100 (center) & clear buttons
        ESP_BT.write(incoming); // //! Will eventually be used to control water on/off
        debugln("Sending 204 back to app");
        break;
    case 255:                   // disableOutputs
        stepperNum = 0;         // No Stepper used here
        ESP_BT.write(appClear); // reset slider poition to 100 (center) & clear buttons
        ESP_BT.write(incoming); // //! Will eventually be used to control water on/off
        debugln("Sending 255 back to app");
        Hstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        Vstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        Sstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        break;
    default:
        switch (stepperNum)
        {
        case 1:
            processStepper(Hstepper, incoming);
            break;
        case 2:
            processStepper(Vstepper, incoming);
            break;
        case 3:
            processStepper(Sstepper, incoming);
            break;
        default:
            break;
        }
        break;
    }
}

// //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void processStepper(AccelStepper &Stepper, int incoming)
{
    int stepperSpeed = 0;
    Stepper.setMaxSpeed(300);
    Stepper.setSpeed(stepperSpeed);
    incoming = incoming - 100;

    if (incoming >= -5 && incoming <= 5)
    {
        stepperSpeed = 0;
        ESP_BT.write(stepperSpeed);
    }
    else if (incoming >= -100 && incoming <= 100)
    {
        stepperSpeed = incoming;
        ESP_BT.write(abs(stepperSpeed)); // abs changes negative values to positive
    }
    else
    {
        stepperSpeed = 0;
        ESP_BT.write(stepperSpeed);
    }
    Stepper.setSpeed(stepperSpeed);
}

// //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void doStepLoop(AccelStepper &Stepper)
{
    debug("Current position is ");
    debugln(Stepper.currentPosition());

    if (Stepper.currentPosition() < -10)
    {
        Stepper.setMaxSpeed(100.0);    // Set Max Speed of Stepper (Slower to get better accuracy)
        Stepper.setAcceleration(50.0); // Set Acceleration of Stepper
        Stepper.moveTo(home);
        Stepper.runToPosition();
        ESP_BT.write(appClear);
    }
    else if (Stepper.currentPosition() > (maxPosition + 10))
    {
        Stepper.setMaxSpeed(100.0);    // Set Max Speed of Stepper (Slower to get better accuracy)
        Stepper.setAcceleration(50.0); // Set Acceleration of Stepper
        Stepper.moveTo(maxPosition);
        Stepper.runToPosition();
        ESP_BT.write(appClear);
    }
    else
        Stepper.runSpeed();
}
