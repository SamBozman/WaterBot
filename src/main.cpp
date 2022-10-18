#include <Arduino.h>
#include <ArduinoJson.h>

char output[256]; // Declare a buffer to hold the result
const int capacity = 256; // ArduinoJson assistant 

StaticJsonDocument<capacity> doc;

void setup()
{
  Serial.begin(115200);
 
  doc["ID"] = 1; //(Int) ID  for watering target
  doc["Description"] = "Short Description of this watering target goes here";
  doc["H_Steps"] = 1145;     //(int) Horizontal steps from 'Home'
  doc["H_Fluctuation"] = 10; //(int) back and forth fluctuation for coverage
  doc["V_Steps"] = 415;      //(int) Vertical steps from 'Home'
  doc["V_Fluctuation"] = 10; //(int) up and down fluctuation for coverage
  doc["Spray"] = 234;        //(int) Spray steps for diffused spray
  doc["RWT"] = 5;            // Relative watering time
  doc["Water_On?"] = true;   // Turn water off ? (or leave on while moving to next spot)

  // Serialize the Json object and put into variable (output)
  serializeJson(doc, output);

  Serial.println("Here is the data!");
  Serial.println(output);           // Print out “minified” JSON document
  serializeJsonPretty(doc, Serial); // Print out “prettified” JSON document
}

void loop()
{
  // put your main code here, to run repeatedly:
}