#pragma once
#include "globals.h"

class WATER_DATA {

public:
    struct target_data {
        short int id; // Computer created Target ID
        short int hp; // Horizontal position
        std::string name; // User defined name for the watering target
        short int vp; // Vertical Position
        short int sp; // Spray position – a needle to break up the spray stream
        short int hf; // Horizontal fluctuation -small variations in the horizontal position over time
        short int vf; // Vertical fluctuation -small variations in the vertical position over time
        short int sf; // Spray fluctuation -small variations in the spray needle position over time
        short int rwt; // Relative Watering Time (Certain plants will get more or less)
        bool water; // Do we want the water on or off while traversing to next target?

        bool operator<(const target_data& other) const
        {
            // Sort on hp ID, if they are equal use the id to sort
            if (hp < other.hp)
                return true;
            else if (hp > other.hp)
                return false;
            else
                return id < other.id;
        }
    } s_target_data;
};

class WATER_DATA_MANAGER {
public:
    // Define a sorted set of data elements
    typedef std::set<WATER_DATA::target_data> WATER_DATA_SET;
    WATER_DATA_SET water_data_set; // Create a Data set variable

    // Provide insert and delete access to it
    void insert_data(WATER_DATA::target_data& data); // Adds a new data entry
    void delete_data(int id); // Removes the entry by ID if it exists
    void print_all_data(void); // prints the entire data structure

private:
};

void stochar(String s); // Temporary
void createTestFiles(int n); // Temporary
void removeTestFiles(int n); // Temporary

void convertStringToInt();
void getJson();
void resetMax(AccelStepper* Stepper);
void setMax(AccelStepper* Stepper);
void loadMax(char* path, long* Maxptr);
void homeStepper(AccelStepper& Stepper, int homePin);
void processIncoming(int incoming);
void processStepper(AccelStepper* Stepper, int incoming);
void doStepLoop(AccelStepper* Stepper, long* MaxPos);

void makeWaterTarget(int id, String name, long Hs, long Hf, long Vs, long Vf, long Ss, long Sf, long rwt, bool W_on);
void listDir(fs::FS& fs, const char* dirname, uint8_t levels);
bool isDir(fs::FS& fs, const char* dirname, uint8_t levels);

int listFiles(fs::FS& fs, const char* dirname, uint8_t levels, int numFiles);
void createDir(fs::FS& fs, const char* path);
void removeDir(fs::FS& fs, const char* path);
void readFile(fs::FS& fs, const char* path);
void writeFile(fs::FS& fs, const char* path, const char* message);
void appendFile(fs::FS& fs, const char* path, const char* message);
void renameFile(fs::FS& fs, const char* path1, const char* path2);
void deleteFile(fs::FS& fs, const char* path);
// SPIFFS-like write and delete file,
// better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1
void writeFile2(fs::FS& fs, const char* path, const char* message);
void deleteFile2(fs::FS& fs, const char* path);
void testFileIO(fs::FS& fs, const char* path);
void mountLFS();
