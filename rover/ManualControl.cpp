#include "ManualControl.hh"

void GetBTCommand(char border, float * ans ){
  char data;                        //Variable to store the data
  String Command = "";              //variable to be returned
  while(true){
    if ( Serial.available()){       //Checks the availability of Serial port
      data = Serial.read();         // Read the data and stores it in variable. 
      if(data == border){           // Different commands are separated by # or \n characters. If you read in a # or \n it is clear that a command has just ended.
        if(border == '#'){
          UnicaTranslateCommand(ans , Command);
          return; 
        }else{
          BluetoothPadTranslateCommand(ans , Command);
          return;
        }
      }else{
        Command = Command + String(data); //Append the character to the end of the command string 
      }
    }
  }
}




void BluetoothPadTranslateCommand(float * ans , String Input){
  int X;
  int Y;
  int S;
  int C = Input.substring(0,1).toInt();
  int start = 2;
  int count = 2;
  int i = 0;
  
  while(i<2){
    count ++;

    if(Input.substring(count,count+1)==","){
      if(i==0){
        X=Input.substring(start,count).toInt();
        start = count + 1;
        i++;
      }else if(i==1){
        Y=Input.substring(start,count).toInt();
        start = count + 1;
        i++;
        S=Input.substring(start,Input.length()).toInt();
      }
    }
  }


  float angle = 180.0*atan2(Y,X)/3.14;
  if(Y<0){
    angle+=360.0;
  }
  
  float power = sqrt((X*X)+(Y*Y));
  if(power>100){
    power = 100.0;
  }
  power = power * 2.55;

  ans[0]=angle;
  ans[1]=power;
  ans[2]=S;
  for(int i = 3; i<7; i++){
    if((C+3) == i){
      ans[i]=1;
    }else{
      ans[i]=0;
    }
  }  
}




void UnicaTranslateCommand(float * ans , String Input){
  float angle = float(Input.substring(0,3).toInt());      //First get the abc numbers out of the command and then convert to integer.
  float power = float(Input.substring(3,6).toInt()*2.55); //Next get the def numbers out of the command and then convert to integer. Finally convert this 0-100 value into a 0-255 value.
  int button = float(Input.substring(6).toInt());         //Finally get the g number out of the command and then convert to integer.
  ans[0]=angle;
  ans[1]=power;
  for(int i = 2; i<7; i++){
    if(button==(i-1)){
      ans[i]=1;
    }else{
      ans[i]=0;
    }
  }

  Serial.println(ans[0]);
  Serial.println(ans[1]);
  Serial.println(ans[2]);
  Serial.println(ans[3]);
  Serial.println(ans[4]);
  Serial.println(ans[5]);
  Serial.println(ans[6]);
  Serial.println("");
  
}




void SimpleMapInput(float * ans, float * Input){
  float angle = Input[0];
  float power = Input[1];
  int L_direction;
  int R_direction;
  float L_speed;
  float R_speed;


  // define direction for L motor (corrasponds with motor A on the motor driver)
  if(angle<=180 ||angle>=330){
    L_direction = 1;
  }else{
    L_direction = 0;
  }

  // define direction for R motor (corrasponds with motor B on the motor driver)
  if(angle<=200){
    R_direction = 1;
  }else{
    R_direction = 0;
  }

  // define L speed
  if(angle>=90 && angle<=270){
    L_speed = abs(((angle-90.0)/90.0)-1.0)*power; //The angle will range from 90 -> 270 and will output a L_Speed range of 1 -> -1, multiplied by the the power variable.
  }else if(angle<90){
    L_speed = (0.5+(angle/180.0))*power; //The angle will range from 0 -> 90 and will output a L_Speed range of 0.5 -> 1, multiplied by the the power variable.
  }else{
    L_speed = abs(0.7 + (((angle-360.0)/90.0)*1.7))*power; //The angle will range from 270 -> 360 and will output a L_Speed range of -1 -> 0.5, multiplied by the the power variable.
  }

  // define R speed
  if(angle<=90){
    R_speed = (angle/90.0)*power;
  }else if(angle>=270){
    R_speed = abs((angle-360)/90)*power;
  }else if(angle>90 && angle<180){
    R_speed = (1-((angle-90)/180))*power;
  }else{
    R_speed = abs(0.7 - (((angle-180.0)/90.0)*1.7))*power;
  }

  


  ans[0] = (L_direction);
  ans[1] = (R_direction);
  ans[2] = (L_speed);
  ans[3] = (R_speed);
  ans[4] = map(Input[2], 0, 1, 90, 180);
}




void ExecuteCommand_L298N(float * Command) {
  
  if(Command[0]>0){
    digitalWrite(DIR_L_F, HIGH);
    digitalWrite(DIR_L_B, LOW);
  }else{
    digitalWrite(DIR_L_B, HIGH);
    digitalWrite(DIR_L_F, LOW);
  }

  if(Command[1]>0){
    digitalWrite(DIR_R_F, HIGH);
    digitalWrite(DIR_R_B, LOW);
  }else{
    digitalWrite(DIR_R_B, HIGH);
    digitalWrite(DIR_R_F, LOW);
  }
  
  // Next set speed:
  if (Command[2] > 100) {
    analogWrite(PWM_L, 200);
    Serial.print("HIGH ");
  } else {
    analogWrite(PWM_L, 0);
    Serial.print("LOW ");
  }

  if (Command[3] > 100) {
    analogWrite(PWM_R, 200);
    Serial.print("HIGH\n");
  } else {
    analogWrite(PWM_R, 0);
    Serial.print("LOW\n");
  }
  //analogWrite((int)PWM_L, Command[2]);
  //analogWrite((int)PWM_R, Command[3]);
}
