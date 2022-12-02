
#pragma once
#include "Files.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void homeStepper(AccelStepper& Stepper, int homePin)
{
    int move_finished = 1; // Used to check if move is completed
    long initial_homing = -1; // Used to Home Stepper at startup
    pinMode(homePin, INPUT_PULLUP);

    delay(5);

    //  Set Max Speed and Acceleration of each Steppers at startup for homing
    Stepper.setMaxSpeed(100.0); // Set Max Speed of Stepper (Slower to get better accuracy)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper

    // Start Homing procedure of Stepper Motor at startup

    while (digitalRead(homePin)) // Do WHILE switch not activated (NOT true)
    {
        // Make the Stepper move CCW
        // until the switch is activated
        Stepper.moveTo(initial_homing); // Set the position to move to
        initial_homing--; // Decrease by 1 for next move if needed
        Stepper.run(); // Non Blocking - move to position
        delay(5);
    }

    Stepper.setCurrentPosition(0); // Set the current position as zero for now
    Stepper.setMaxSpeed(100.0); // Set Max Speed of Stepper (Slower to get better accuracy)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper

    initial_homing = 1;
    while (!digitalRead(homePin)) {
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

    // int rnum;
    // srand(time(0));
    // rnum = rand() % (500 + 1 - 100) + 100;
    // Stepper.moveTo(rnum);
    // Stepper.runToPosition();

    Stepper.setMaxSpeed(200.0); // Set Max Speed of Stepper (Faster for regular movements)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void processIncoming(int incoming)
{
    unsigned char textBlock[] = "Hello to everyone who loves me \n";
    unsigned char textBlock2[] = "Goodbye to everyone who hates me \n";

    switch (incoming) {

    case 201:
        StepPtr = &Hstepper; // Hstepper
        MaxPtr = &H_MaxPos;
        ESP_BT.write(incoming); // Set app to use Hstepper
        debugln("Sending 201 back to app");
        currentStepper = 1;
        ESP_BT.write(resetSlider);
        break;
    case 202:
        StepPtr = &Vstepper; // Vstepper
        MaxPtr = &V_MaxPos;
        ESP_BT.write(incoming); // Set app to use Vstepper
        debugln("Sending 202 back to app");
        currentStepper = 2;
        ESP_BT.write(resetSlider);
        break;
    case 203:
        StepPtr = &Sstepper; // Sstepper
        MaxPtr = &S_MaxPos;
        ESP_BT.write(incoming); // Set app to use Sstepper
        debugln("Sending 203 back to app");
        currentStepper = 3;
        ESP_BT.write(resetSlider);
        break;
    case 204: //! Will eventually be used to control water on/off
        ESP_BT.write(incoming);
        debugln("Sending 204 back to app");
        break;
    case 205: // This is the SetMax for current stepper motor
        ESP_BT.write(incoming);
        debugln("Sending 205 back to app");
        setMax(StepPtr); // Save MaxPosition for currentStepper
        break;
    case 206: // This is the RESET MAX for current stepper motor
        ESP_BT.write(incoming);
        debugln("Sending 206 back to app");
        resetMax(); // Reset current stepper motor max position to 10000
        break;
    case 207: // Used to test sending Json from app
        getJson();
        ESP_BT.write(207);
        debugln("Sending 207 back to app");
        break;

    case 254: //! Testing blob text to WaterBot app
        ESP_BT.write(resetSlider); // reset slider poition to 100 (center) & clear buttons
        ESP_BT.write(textBlock, sizeof(textBlock));
        ESP_BT.write(textBlock2, sizeof(textBlock2)); //* Testing writing block to text back to app

        debugln("Sending text block back to app");
        break;

    case 255: //! disableOutputs for testing
        ESP_BT.write(resetSlider); // reset slider poition to 100 (center) & clear buttons
        ESP_BT.write(incoming);
        debugln("Sending 255 back to app");
        Hstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        Vstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        Sstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        break;
    default:
        processStepper(StepPtr, incoming);
        break;
    }
}

// //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void processStepper(AccelStepper* Stepper, int incoming)
{
    int stepperSpeed = 0;
    Stepper->setMaxSpeed(300);
    Stepper->setSpeed(stepperSpeed);
    incoming = incoming - 100;

    if (incoming >= -5 && incoming <= 5) {
        stepperSpeed = 0;
        ESP_BT.write(stepperSpeed);
    } else if (incoming >= -100 && incoming <= 100) {
        stepperSpeed = incoming;
        ESP_BT.write(abs(stepperSpeed)); // abs changes negative values to positive
    } else {
        stepperSpeed = 0;
        ESP_BT.write(stepperSpeed);
    }
    Stepper->setSpeed(stepperSpeed);
}

// //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void doStepLoop(AccelStepper* Stepper, long* MaxPos)
{

    if (Stepper->currentPosition() < -10) {
        Stepper->setMaxSpeed(100.0); // Set Max Speed of Stepper (Slower to get better accuracy)
        Stepper->setAcceleration(50.0); // Set Acceleration of Stepper
        Stepper->moveTo(home);
        Stepper->runToPosition();
        ESP_BT.write(resetSlider);
    } else if (Stepper->currentPosition() > (*MaxPos + 10)) {
        Stepper->setMaxSpeed(100.0); // Set Max Speed of Stepper (Slower to get better accuracy)
        Stepper->setAcceleration(50.0); // Set Acceleration of Stepper
        Stepper->moveTo(*MaxPos);
        Stepper->runToPosition();
        ESP_BT.write(resetSlider);
    } else
        Stepper->runSpeed();
}
