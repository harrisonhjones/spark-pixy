// Spark Pixy Library

// Name: 1D Servo Follower Fast Example

// Author: Harrison Jones

// Description: This example uses a single panning servo, spark core, and pixy 
// to follow an object. It uses a more advanced control algorithm to speed up
// how fast it can close in on objects

// Video: https://www.youtube.com/watch?v=q764IZ9fZNA

// Setup: 
// 1. Flash code to Spark Core
// 2. Connect pixy to SparkCore via SPI interface
// 3. Connect a standard servo to the Spark Core. Data on D0
// 4. Attach the pixy to the servo using custom hardware (hot glue?). Servo will
// start 'centered' so run the program and then attach it so the pixy is 
// centered
// 5. Using Pixy's PixyMon software define a object color
// 6. Make sure to the Pixy is running the "default program". You should be able
// to see object bounding boxes but not the actual image from the pixy. Example
// works best when only 1 object is detected by the pixy.
// 7. Connect to the Spark Core over serial.
// 8. Place the defined object in front of the pixy. The Pixy should move toward
// the object using the servo. Check the PixyMon software to see if a bounding 
// box is also visible

// Troubleshooting: see the issues page in the Github repo if you have a problem

// Begin User Defined Defines
#define SERVO_DEFAULT_POSITION 100  // You may need to change this for your own servo
#define SERVO_PIN D0                // The pin the servo is attached to
#define SERVO_MAX 180               // The furthest the sevro can turn
#define SERVO_MIN 20                // The furthest the servo can turn
#define SCALING_FACTOR 10           // How much to scale the error for movement. If servo turns the wrong way make this negative
#define OBJECT_TOLERANCE 10         // How many pixels off center the object can be
#define FRAME_SKIP 5                // How many frames to skip. Lower numbers cause faster response
// End User Defined Defines

// Include the two pixy libraries
#include <SPI.h>  
#include <Pixy.h>


SYSTEM_MODE(MANUAL);                // Set the system mode to manual. We don't need the cloud for this code

Pixy pixy;                          // Create our pixy object
Servo myservo;                      // Create our servo object

int pos = SERVO_DEFAULT_POSITION;   // Create an int to hold the servo's position
int i = 0;                          // Create an int to count for frame skipping. Frame skipping is used to artificially slow down the pixy loop

// Setup - Runs Once @ Startup
void setup()
{
    Serial.begin(9600);             // Initalize the USB Serial port
    myservo.attach(SERVO_PIN);      // Attach the servo object to the servo pin
    myservo.write(pos);             // Move the servo to the default position
    pixy.init();                    // Initalize the pixy object
}

// Loop - Runs over and over
void loop()
{ 
    // Variable Creation 
    uint16_t blocks;                // Create an unsigned int to hold the number of found blocks
    char buf[50];                   // Create a buffer for printing over serial
    int xErr = 0;                   // Create a int for tracking the error between center and the object

    blocks = pixy.getBlocks();      // Do the pixy stuff. Grab the numbers of blocks the pixy finds

    // If we have some blocks increment the frame counter (i) and if enough frames have passed move the servo if there is an error
    if (blocks)
    {
    i++;
        if (i%FRAME_SKIP==0)
        {
            // If the center of the first block (block 0) is outside of tolerance move the servo in the correct direction
            xErr = 150 - pixy.blocks[0].x;
            if((xErr > OBJECT_TOLERANCE) || (xErr < -OBJECT_TOLERANCE))
            {
                pos = pos - (xErr / SCALING_FACTOR);
                if (pos > SERVO_MAX)
                    pos = SERVO_MAX;
            }
            // Actually move the servo and then print our some debug info
            myservo.write(pos);
            sprintf(buf, "\tObject X: %d & Servo Pos: %d", pixy.blocks[0].x, pos);
            Serial.println(buf); 
        }
    }  
}