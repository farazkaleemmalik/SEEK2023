#include "ManualControl.hh"
#include <Servo.h>

float ControllerInput[8];    // Angle, speed, button 1-4 (Android)   or    Angle, speed, slider value, channel 0-3 (Windows) 
float MotorOutputs[5];       // Left Direction, Right Direction, Left Speed, Right Speed, Mini Servo Position
Servo Scoop;
#define MODE 10 // Input pin for which more we are in, 0 is autonomous and 1 is manual

int sensor1 = 2;
int sensor2 = 3;

float left_dir = 0;
float right_dir = 0;
float left_v;
float right_v;

float MotorOutputsAuto[4] = {left_dir, right_dir, 0, 0}; //Takes in a 4 element list (Left Direction, Right Direction, Left Speed, Right Speed) and applies the correct outputs to the 6 pins used to drive the motors. (these pins are determined by you at the begining of the code) 

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

  //Scoop.attach(SCP);

  pinMode(sensor1, INPUT);
  pinMode(sensor2, INPUT);
}

void loop() {
  if (digitalRead(MODE) == LOW) {
    // Autonomous code here
    int x1 = digitalRead(sensor1);
    int x2 = digitalRead(sensor2);
    Serial.print(x1);
    Serial.print(" ");
    Serial.print(x2);
    Serial.print("\n");

    if (x1 == LOW) {
      MotorOutputsAuto[2] = 200;
    } else {
      MotorOutputsAuto[2] = 0;
    }

    if (x2 == LOW) {
      MotorOutputsAuto[3] = 200;
    } else {
      MotorOutputsAuto[3] = 0;
    }
    ExecuteCommand_L298N(MotorOutputsAuto);
  } else {
    // put your main code here, to run repeatedly:
    GetBTCommand('\n', ControllerInput);  // '\n' for Windows and '#' for android 
    SimpleMapInput(MotorOutputs, ControllerInput);
    MotorOutputs[0] = MotorOutputs[0] ? 0 : 1;
    MotorOutputs[1] = MotorOutputs[1] ? 0 : 1;
    
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
