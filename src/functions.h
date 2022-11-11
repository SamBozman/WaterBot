#pragma once
#include "testFunctions.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void homeStepper(AccelStepper Stepper, int homePin)
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
        Stepper.run();                  // Start moving the stepper
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
        Stepper.run();
        initial_homing++;
        delay(5);
    }
    Stepper.setCurrentPosition(0);

    Serial.println("Homing Completed");
    Serial.println("");
    Stepper.setMaxSpeed(200.0);     // Set Max Speed of Stepper (Faster for regular movements)
    Stepper.setAcceleration(100.0); // Set Acceleration of Stepper
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void processIncoming(int incoming)
{
    switch (incoming)
    {

    case 201:
        Stepper = Hstepper;
        ESP_BT.write(incoming); // Call back to app
        debugln("Sending 201 back to app");
        break;
    case 202:
        Stepper = Vstepper;
        ESP_BT.write(incoming); // Call back to app
        debugln("Sending 202 back to app");
        break;
    case 203:
        Stepper = Sstepper;
        ESP_BT.write(incoming); // Call back to app
        debugln("Sending 203 back to app");
        break;
    case 204:                   //! Will eventually be used to control water on/off
        ESP_BT.write(incoming); // Call back to app
        debugln("Sending 204 back to app");
        Hstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        Vstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        Sstepper.disableOutputs(); // TODO Temporary to shut off steppers for testing
        break;

    default:
        processStepper(incoming);
        break;
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void processStepper(int incoming)
{
    Stepper.setMaxSpeed(300);
    int stepperSpeed;
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void doStepLoop()
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void makeWaterTarget(int id, String name, long Hs, long Hf, long Vs, long Vf, long spray, long rwt, bool W_on)
{
    const int capacity = 256;         // Buffer for StaticJsonDocument doc
    StaticJsonDocument<capacity> doc; // Declare StaticJsonDocument doc

    doc["ID"] = id; //(Int) ID  for watering target
    doc["Name"] = name;
    doc["H_Steps"] = Hs;       //(int) Horizontal steps from 'Home'
    doc["H_Fluctuation"] = Hf; //(int) back and forth fluctuation for coverage
    doc["V_Steps"] = Vs;       //(int) Vertical steps from 'Home'
    doc["V_Fluctuation"] = Vf; //(int) up and down fluctuation for coverage
    doc["Spray"] = spray;      //(int) Spray steps for diffused spray
    doc["RWT"] = rwt;          // Relative watering time (1-5)
    doc["Water_On?"] = W_on;   // Turn water off ? (or leave on while moving to next spot)
    // Serialize the Json object and put into variable (output)
    serializeJson(doc, g_output);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void mountLFS()
{
    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        debugln("LittleFS Mount Failed");
        return;
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int listFiles(fs::FS &fs, const char *dirname, uint8_t levels, int numFiles)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        debugln("- failed to open directory");
        return 0;
    }
    if (!root.isDirectory())
    {
        debugln(" - not a directory");
        return 0;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            debug("  DIR : ");
            debugln(file.name());
            if (levels)
            {
                debug("Levels = : ");
                debugln(levels);
                numFiles = listFiles(fs, file.path(), levels - 1, numFiles);
            }
        }
        else
        {
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        debugln("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        debugln(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            debug("  DIR : ");
            debugln(file.name());
            if (levels)
            {
                listDir(fs, file.path(), levels - 1);
            }
        }
        else
        {
            debug("  FILE: ");
            debug(file.name());
            debug("\tSIZE: ");
            debugln(file.size());
        }
        file = root.openNextFile();
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void createDir(fs::FS &fs, const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
        debugln("Dir created");
    }
    else
    {
        debugln("mkdir failed");
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void removeDir(fs::FS &fs, const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        debugln("Dir removed");
    }
    else
    {
        debugln("rmdir failed");
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        debugln("- failed to open file for reading");
        return;
    }

    debug("- read from file: ");
    while (file.available())
    {
        Serial.write(file.read());
    }
    debugln("");
    file.close();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        debugln("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        debugln("- file written");
    }
    else
    {
        debugln("- write failed");
    }
    file.close();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        debugln("- failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        debugln("- message appended");
    }
    else
    {
        debugln("- append failed");
    }
    file.close();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        debugln("- file renamed");
    }
    else
    {
        debugln("- rename failed");
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file: %s\r\n", path);
    if (fs.remove(path))
    {
        debugln("- file deleted");
    }
    else
    {
        debugln("- delete failed");
    }
}

// SPIFFS-like write and delete file, better use #define
// CONFIG_LITTLEFS_SPIFFS_COMPAT 1

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void writeFile2(fs::FS &fs, const char *path, const char *message)
{
    if (!fs.exists(path))
    {
        if (strchr(path, '/'))
        {
            Serial.printf("Create missing folders of: %s\r\n", path);
            char *pathStr = strdup(path);
            if (pathStr)
            {
                char *ptr = strchr(pathStr, '/');
                while (ptr)
                {
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
    if (!file)
    {
        debugln("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        debugln("- file written");
    }
    else
    {
        debugln("- write failed");
    }
    file.close();
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void deleteFile2(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

    if (fs.remove(path))
    {
        debugln("- file deleted");
    }
    else
    {
        debugln("- delete failed");
    }

    char *pathStr = strdup(path);
    if (pathStr)
    {
        char *ptr = strrchr(pathStr, '/');
        if (ptr)
        {
            Serial.printf("Removing all empty folders on path: %s\r\n", path);
        }
        while (ptr)
        {
            *ptr = 0;
            fs.rmdir(pathStr);
            ptr = strrchr(pathStr, '/');
        }
        free(pathStr);
    }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void testFileIO(fs::FS &fs, const char *path)
{
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        debugln("- failed to open file for writing");
        return;
    }

    size_t i;
    debug("- writing");
    uint32_t start = millis();
    for (i = 0; i < 2048; i++)
    {
        if ((i & 0x001F) == 0x001F)
        {
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
    if (file && !file.isDirectory())
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        debug("- reading");
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F)
            {
                debug(".");
            }
            len -= toRead;
        }
        debugln("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    }
    else
    {
        debugln("- failed to open file for reading");
    }
}
