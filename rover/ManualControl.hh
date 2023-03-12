#ifndef ManualControl_h
#define ManualControl_h

#include <Arduino.h>
#include <string.h>


void GetBTCommand(char border, float * ans);                   //Outputs a list containing the raw controller inputs. This function is given a list pointer, and a character teling it which app is being used. 
void UnicaTranslateCommand(float * ans , String Input);          //Gets used by GetBTCommand() when '#' is sent as an input. This function decodes the string message and fills the "TranslatedCommand" list.  
void BluetoothPadTranslateCommand(float * ans , String Input);   //Gets used by GetBTCommand() when '\n' is sent as an input. This function decodes the string message and fills the "TranslatedCommand" list. 
void SimpleMapInput(float * ans, float * Input);                 //A pre write function That takes in TransatedCommand as an imput. It uses the Angle and Power inputs (first 2 indexes of the input list) and output both motor speed and direction information for the left and right motors.
void ExecuteCommand_L298N(float * Command);           //Takes in a 4 element list (Left Direction, Right Direction, Left Speed, Right Speed) and applies the correct outputs to the 6 pins used to drive the motors. (these pins are determined by you at the begining of the code) 

#define DIR_L_F 13    // Left Direction pin that will indicate forewards movement (1 for forewards, 0 for backwards).
#define DIR_L_B 12    // Left Direction pin that will indicate backwards movement (1 for backwards, 0 for forewards).
#define PWM_L 11     // Speed controll pin. *** This pin must be plugged into an output pin on the arduino that is labled PWM ***.

#define DIR_R_F 4     // Right Direction pin that will indicate forewards movement (1 for forewards, 0 for backwards)
#define DIR_R_B 2     // Left Direction pin that will indicate backwards movement (1 for backwards, 0 for forewards).
#define PWM_R 3       // Speed controll pin. *** This pin must be plugged into an output pin on the arduino that is labled PWM ***.

#endif