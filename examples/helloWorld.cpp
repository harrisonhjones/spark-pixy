// Spark Pixy Library

// Name: Hello World Example

// Author: Harrison Jones

// Description: This example simply shows the current blocks found by the pixy

// Setup: 
// 1. Flash code to Spark Core
// 2. Connect pixy to SparkCore via SPI interface
// 3. Connect to the Spark Core over serial and press anykey. 
// 4. Place a defined object in front of the pixy. The Pixy should display info about the object

// Troubleshooting: see the issues page in the Github repo if you have a problem

// Begin User Defined Defines
#define FRAME_SKIP 25                // How many frames to skip. Lower numbers cause faster response
// End User Defined Defines

#include <SPI.h>  
#include <Pixy.h>

SYSTEM_MODE(MANUAL);                // Set the system mode to manual. We don't need the cloud for this code

Pixy pixy;                          // Create our pixy object
int i = 0;                          // Create an int to count for frame skipping. Frame skipping is used to artificially slow down the pixy loop

// Setup - Runs Once @ Startup
void setup()
{
    Serial.begin(9600);             // Initalize the USB Serial port
    pixy.init();                    // Initalize the pixy object
}

// Loop - Runs over and over
void loop()
{ 
    // Variable Creation 
    uint16_t blocks;                // Create an unsigned int to hold the number of found blocks
    char buf[50];                   // Create a buffer for printing over serial

    blocks = pixy.getBlocks();      // Do the pixy stuff. Grab the numbers of blocks the pixy finds
  
  // If we have some blocks increment the frame counter (i) and if enough frames have passed print out data about the found blocks to the serial port
  if (blocks)
  {
    i++;
    if (i%25==0)
    {
      sprintf(buf, "Detected %d:\n", blocks);
      Serial.print(buf);
      for (int j = 0; j < blocks; j++)
      {
        sprintf(buf, "\tblock %d:", j);
        Serial.print(buf); 
        pixy.blocks[j].print();
        Serial.println();
      }
    }
  }  
}