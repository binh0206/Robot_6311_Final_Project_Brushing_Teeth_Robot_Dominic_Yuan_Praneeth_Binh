#include <math.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize Global Varaiables

   int zrobot = .3 * 100;   // z distance of robot in the Kinect frame
   int xrobot = .4 * 100;   // x distance of robot in the Kinect frame
   int yrobot = -.09 * 100; // y distance of robot in the Kinect frame
   int xface;               // Initialize x variable for distance to face in robot frame
   int zface;               // Initialize z variable for distance to face in robot frame
   int yface;               // Initialize y variable for distance to face in robot frame
   int center1 = 650;       // Define a center or start position for the robot
   int pina = 36;           // Motor 1 direction pin
   int pinb = 37;           // Motor 1 direction pin
   int pinc = 28;           // Motor 2 direction pin
   int pind = 29;           // Motor 2 direction pin
   int pinpot1 = A6;        // Potentiometer pin
   int potval;              // Potentiometer reading from robot
   int pot;                 // Desired potentiometer value
   float slope = 3.37;      // Experimental slope to convert theta to potentiometer value
   float pi = 3.14159265359;// Pi
   int motorspeed = 175;    // Motor Speed 1
   int motorspeed2 = 255;   // Motor Speed 2
   int tcount = 0;          // Used to track when the robot should start brushing
   float b[3];              // Array that holds values from serial port
   byte c[50];              // Read in bytes from data file (Communicates with c++) 
   String inputString = "";         
   bool stringComplete = false;
   float alpha = 16.39;     // Angle from joint 4 to toothbrush (Constant)
   float slope2 = 1/.091;   // Experimental slope to convert theta to potentiometer value

//   
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
// Setup the robot & Serial Port

void setup() {
  Serial.begin(9600);
  potval = analogRead(pinpot1);
  returntocenter();
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main Loop of The Robot

void loop() {

  
                while (Serial.available()) // Check if the serial port is available
                {
                  Serial.readBytes(c, 12); // Read character bytes from serial port
                          for (int i = 0; i < 3; i++) 
                          {
                            b[i] = *(float *)(c + i * sizeof(float)); // Converts characters to floats, attains x,y & z data from the Kinect
                          }
                          
                                    if(b[2] < .9) // Max range in which the robot can reach you
                                    {
                                                                      // Convert the points in the Kinect frame into the robot frame
                                    xface = (xrobot - b[0]*100);
                                    zface = b[1]*100 - yrobot;
                                    yface = b[2]*100 - zrobot;
                                    potval = analogRead(pinpot1);                 // Attain current potentiometer value
                                    float theta1 = (atan2(yface,xface)) * 180/pi; // Calculate the angle in degrees where the face should be
                                    pot = theta2pot(theta1);                      // Converted angle to potentiometer value
                                    moveCheck();                                  // Checks and moves to the desired location (User's face)
                                    adjustheight();                               // Adjusts height of robot
                                    brushCheck();                                 // Checks if the robot should start brushing teeth (Then begins to brush teeth)
                                    }                                                                                                             
                }
}
//            
//////////////////////////////////////////////////////////////////////////////////
// Code to Return to Center
void returntocenter()
{
 bool flag = true;
 while(flag)
 { 
Serial.println(potval);
potval = analogRead(pinpot1);
if(center1 - 10 <= potval && potval <= center1 + 10)
{
digitalWrite(pina, LOW);
digitalWrite(pinb, LOW);
Serial.println("Start Code");
flag = false;
}
else if(potval < center1)
{
digitalWrite(pina, LOW);
digitalWrite(pinb, HIGH);
analogWrite(11, motorspeed);
}
else
{
digitalWrite(pina, HIGH);
digitalWrite(pinb, LOW);
analogWrite(11, motorspeed);
}
}
delay(1000); 
}
//
////////////////////////////////////////////////////////////////////////////////////


















//////////////////////////////////////////////////////////////////////////////////
// Convert theta value to potentiometer
  int theta2pot(float theta)
  {
  int pot = center1 + (int)theta*slope;
  return pot;
  }
//
//////////////////////////////////////////////////////////////////////////////////




















//////////////////////////////////////////////////////////////////////////////////
//  Checks Where to Move and Begins to Move
    void moveCheck()
    {
    
    if(pot - 10 <= potval && potval <= pot + 10)
    {
    Serial.println("Stop");
    digitalWrite(pina, LOW);
    digitalWrite(pinb, LOW);
    delay(1000);
    tcount++;
    testcount++;
    }
    else if(pot < potval)
    {
    digitalWrite(pina, HIGH);
    digitalWrite(pinb, LOW);
    analogWrite(11, motorspeed);
    Serial.println("Moving Right");
    }
    
    else if(pot > potval)
    {
    digitalWrite(pina, LOW);
    digitalWrite(pinb, HIGH);
    analogWrite(11, motorspeed);
    Serial.println("Moving Left");
    }
    
    }
//
//////////////////////////////////////////////////////////////////////////////////















//////////////////////////////////////////////////////////////////////////////////
// Checks Where to Move and Begins to Move
void brushCheck()
{
if(tcount == 2)
{

                  for(int p = 0;p < 3;p++)
                  {
                  digitalWrite(pinc, LOW);
                  digitalWrite(pind,HIGH);
                  analogWrite(6, motorspeed2);
                  delay(500);
                  
                  digitalWrite(pinc, HIGH);
                  digitalWrite(pind,LOW);
                  analogWrite(6, motorspeed2);
                  delay(500);
                  }
                  digitalWrite(pinc, LOW);
                  digitalWrite(pind,LOW);
                  analogWrite(6, motorspeed2);
                  delay(500);
                  tcount = 0;              
}
}
//
//////////////////////////////////////////////////////////////////////////////////














///////////////////////////////////////////////////////////////////////////////////    
// Adjust Height of Robot Based off Face Location

  void adjustheight()
  {
  if(zface < 18 && zface > 14)
  {
  int potval2 = analogRead(A2);
  int height = zface - 16;
  float thetax = atan2(height,sqrt((19^2) - (height^2)))*(180/3.14);
  float theta2 = (thetax - alpha);
  float pot2 = slope2 * theta2 + 496; 
  Serial.print(pot2);
  Serial.print("  ");
  Serial.println(potval2);
  
  if(pot2 - 20 <= potval2 && potval2 <= pot2 + 20)
  {
  digitalWrite(28, LOW);
  digitalWrite(29, LOW);
  delay(1000);
  }
  else if(pot2 < potval2)
  {
  digitalWrite(28, HIGH);
  digitalWrite(29,LOW);
  analogWrite(6, 150); 
  }
  
  else if(pot2 > potval2)
  {
  digitalWrite(28, LOW);
  digitalWrite(29,HIGH);
  analogWrite(6, 150); 
  }
  }
  }
//
//////////////////////////////////////////////////////////////////////////////////////
