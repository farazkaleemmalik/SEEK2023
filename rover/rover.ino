#include "ManualControl.hh"
#include <Servo.h>

float ControllerInput[8];    // Angle, speed, button 1-4 (Android)   or    Angle, speed, slider value, channel 0-3 (Windows) 
float MotorOutputs[5];       // Left Direction, Right Direction, Left Speed, Right Speed, Mini Servo Position
Servo Scoop;
#define MODE 10 // Input pin for which more we are in, 0 is autonomous and 1 is manual


void setup() {
  Serial.begin(9600);           //Initializing the Serial Port to Baud rate 9600. You can change this to be faster if you wish.

  //Setting up pins:
  pinMode(DIR_L_F, OUTPUT);
  pinMode(DIR_L_B, OUTPUT);
  pinMode(PWM_L, OUTPUT);

  pinMode(DIR_R_F, OUTPUT);
  pinMode(DIR_R_B, OUTPUT);
  pinMode(PWM_R, OUTPUT);

  pinMode(MODE, INPUT);

  Scoop.attach(SCP);
}

void loop() {
  if (digitalRead(MODE) == LOW) {
    // Autonomous code here
  } else {
    // put your main code here, to run repeatedly:
    GetBTCommand('\n', ControllerInput);  // '\n' for Windows and '#' for android 
    SimpleMapInput(MotorOutputs, ControllerInput);
    Serial.print("Values: ");
    for (int i = 0; i < 4; i++) {
      Serial.print(MotorOutputs[i]);
      Serial.print(" ");
    }
    Serial.print("\n");
    // Apply drivetrain
    ExecuteCommand_L298N(MotorOutputs);
    // Apply scoop
    //Scoop.write(MotorOutputs[4]);
  }
  
}
