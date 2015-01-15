// Spark Pixy Library

// Name: 1D Servo Follower Example

// Author: Harrison Jones

// Description: This example uses a single panning servo, spark core, and pixy 
// to follow an object. 

// Video: https://www.youtube.com/watch?v=l0pi_0FXljk

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
// 7. Connect to the Spark Core over serial and press anykey. The program will 
// start.
// 8. Place the defined object in front of the pixy. The Pixy should move toward
// the object using the servo. Check the PixyMon software to see if a bounding 
// box is also visible

// Troubleshooting: see the issues page in the Github repo if you have a problem

// Begin User Defined Defines
#define SERVO_DEFAULT_POSITION 100  // You may need to change this for your own servo
#define SERVO_PIN D0                // The pin the servo is attached to
#define SERVO_MAX 180               // The furthest the sevro can turn
#define SERVO_MIN 20                // The furthest the servo can turn
#define SERVO_STEPS 2               // How far the servo turns each update. If servo turns the wrong way make this negative
#define OBJECT_TOLERANCE 10         // How many pixels off center the object can be
#define FRAME_SKIP 5         // How many frames to skip (kind of))
// End User Defined Defines


#include <SPI.h>  
#include <Pixy.h>

SYSTEM_MODE(MANUAL);
Pixy pixy;

Servo myservo; 
int pos = SERVO_DEFAULT_POSITION;    // variable to store the servo position

void setup()
{

  Serial.begin(9600);
  myservo.attach(SERVO_PIN);  // attaches the servo on the A0 pin to the servo object
  myservo.write(pos);
  
  while(!Serial.available());
  Serial.println("Program Start");
  
  pixy.init();
}

void loop()
{ 
  static int i = 0;
  int j;
  uint16_t blocks;
  char buf[50]; 
  
  blocks = pixy.getBlocks();
  
  // sprintf(buf, "Detected %d:", blocks);
  // Serial.println(buf);
  
  if (blocks)
  {
    i++;
    
    if (i%FRAME_SKIP==0)
    {
        if(pixy.blocks[0].x > (150 + OBJECT_TOLERANCE))
        {
            pos = pos + SERVO_STEPS;
            if (pos > SERVO_MAX)
                pos = SERVO_MAX;
        }
        if(pixy.blocks[0].x < (150 - OBJECT_TOLERANCE))
        {
            pos = pos - SERVO_STEPS;
            if (pos < SERVO_MIN)
                pos = SERVO_MIN;
        }
        myservo.write(pos);
        sprintf(buf, "\tObject X: %d & Servo Pos: %d", pixy.blocks[0].x, pos);
        Serial.println(buf); 
    }
  }  
}

