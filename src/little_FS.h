#pragma onceSerial.print
#include "globals.h"

void mountLFS()
{
    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        debugln("LittleFS Mount Failed");
        return;
    }
}

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

// SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

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
