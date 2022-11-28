#pragma once
#include "globals.h"

// function to make a char path from a string
void stochar(int x)
{
    String s = "/A" + String(x); // Combine 2 strings
    int n = s.length(); // Get length of combined string
    strcpy(path, s.c_str()); // Convert it to a char array
}

// Create random water Target json files and write them to file system
void createTestFiles(int n)
{
    int numFiles = 0;
    for (int x = 1; x <= n; x++) {
        makeWaterTarget(x, "Test", random(4000), random(4000), random(4000), random(4000), random(4000),
            random(4000), random(4000), false);
        stochar(x); // Takes the int value of x and adds a '/A' to it to create a directory Path
        writeFile(LittleFS, path, g_output);
    }
    listDir(LittleFS, "/", 1);
    numFiles = listFiles(LittleFS, "/", 1, numFiles);
    debug("Number of files = ");
    debugln(numFiles);

}

// Remove random water Target json files from file system
void removeTestFiles(int n)
{
    for (int x = 1; x <= n; x++) {
        stochar(x);
        deleteFile(LittleFS, path);
    }
    listDir(LittleFS, "/", 1);
}