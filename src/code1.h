#pragma once
#include "globals.h"

void stochar(int x)
{
   String s = "/A" + String(x);
   int n = s.length();
   strcpy(path, s.c_str());
}

void createTestFiles(int n)
{
   for (int x = 1; x <= n; x++)
   {
      makeWaterTarget(x, "Test", random(4000), random(4000), random(4000), random(4000),
                      random(4000), random(4000), false);
      stochar(x);
      writeFile(LittleFS, path, g_output);
   }
}

void removeTestFiles(int n)
{
   for (int x = 1; x <= n; x++)
   {
      stochar(x);
      deleteFile(LittleFS, path);
   }
}