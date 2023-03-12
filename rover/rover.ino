#include "ManualControl.hh"

float ControllerInput[8];    // Angle, speed, button 1-4 (Android)   or    Angle, speed, slider value, channel 0-3 (Windows) 
float MotorOutputs[4];       // Left Direction, Right Direction, Left Speed, Right Speed.

void setup() {
  Serial.begin(9600);           //Initializing the Serial Port to Baud rate 9600. You can change this to be faster if you wish.

  //Setting up pins:
  pinMode(DIR_L_F, OUTPUT);
  pinMode(DIR_L_B, OUTPUT);
  pinMode(PWM_L, OUTPUT);

  pinMode(DIR_R_F, OUTPUT);
  pinMode(DIR_R_B, OUTPUT);
  pinMode(PWM_R, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  GetBTCommand('#', ControllerInput);  // '\n' for Windows and '#' for android 
  SimpleMapInput(MotorOutputs, ControllerInput);
  ExecuteCommand_L298N(MotorOutputs);
}
