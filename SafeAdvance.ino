#include "Display.h"

const int input_button = 9;
const int confirm_button = 8;
const int LEDyellow = 7;
const int LEDblue = 6;
const int LEDgreen= 5;
const int LEDred = 4;
const int buzzer = 3;
const int LDR = A2;
const int thresshold = 300;  // Change it according to your light condition
const int normal = 1;
const int setPassword = 2;

int digitNumber = 0;
int loginAttempt = 0;
int mode;
char password[] = {'1','2','3','4'}; //write your password here
char dataStr[] = {'0','0','0','0'};
unsigned long timerPreviousValue = 0;


void setup() {
  Serial.begin(9600);
  pinMode(input_button,INPUT_PULLUP);
  pinMode(confirm_button,INPUT_PULLUP);
  pinMode(LEDyellow,OUTPUT);
  pinMode(LEDblue,OUTPUT);
  pinMode(LEDgreen,OUTPUT);
  pinMode(LEDred,OUTPUT);
  pinMode(buzzer,OUTPUT);
  Display.on();
}

void loop() {
  
  int sensorReading = analogRead(LDR); // set the thresshold according to your sensor reading
                                      
  if(sensorReading > thresshold){  // it means there is light
    digitalWrite(LEDblue,HIGH);
    Display.show("____");
    while(digitalRead(input_button));
    unsigned long timerPreviousValue = millis();
    while(!digitalRead(input_button));
    if ( (millis() - timerPreviousValue) >= 3000) {
      mode = setPassword;
      digitalWrite(LEDyellow,HIGH);
      //timerPreviousValue = currentValue;
      for ( int p=0; p<4; p++)password[p] = '0';
    }
    else{
      mode = normal;
      digitalWrite(LEDyellow,LOW);
    }
    
    int i=1;
    digitNumber = 0;
    while(digitNumber<4){
        
        int j = i%5;      // Modulo operator to generate cyclic number as mentioned in the assignment
        if(j == 0)i++;
        else {
          numberToCharConversion(j);
          if( mode == normal ){
            
            Display.showCharAt(0, dataStr[0]);
            if(dataStr[1] == '0')Display.showCharAt(1, ' ');
            else Display.showCharAt(1, dataStr[1]);
            if(dataStr[2] == '0')Display.showCharAt(2, ' ');
            else Display.showCharAt(2, dataStr[2]);
            if(dataStr[3] == '0')Display.showCharAt(3, ' ');
            else Display.showCharAt(3, dataStr[3]);
          }
          else{
            Display.showCharAt(0, password[0]);
            if(password[1] == '0')Display.showCharAt(1, ' ');
            else Display.showCharAt(1, password[1]);
            if(password[2] == '0')Display.showCharAt(2, ' ');
            else Display.showCharAt(2, password[2]);
            if(password[3] == '0')Display.showCharAt(3, ' ');
            else Display.showCharAt(3, password[3]);
          }
          
          
          i++;
          
          while(digitalRead(input_button) &&  digitalRead(confirm_button) && analogRead(LDR) > thresshold){
            };
          if( analogRead(LDR) < thresshold ) goto forcedOpen;
          while(!digitalRead(input_button));
          if(digitalRead(confirm_button)== 0){
            numberToCharConversion(j);
            while(!digitalRead(confirm_button));
            if(digitNumber==3){
              break;
              }
          
            
            digitNumber++;
            }
          }
        }
    
     
      //Display.show(dataString);

      //Check if password is correct or not
      if(mode == normal){
        if(matchPassword()){
          //Display.clear();
          for(int l=0; l<4; l++)dataStr[l] = '0';
          Display.show("OPEN");
          delay(1000);
          Serial.println("Congratulations!! Unlocked!");
          digitalWrite(LEDgreen,HIGH);
          delay(5000);
          digitalWrite(LEDgreen,LOW);
          //Display.off();     // Turmimh the display
          // Clearing the string
          loginAttempt = 0;
          Display.clear();
          
        }
        else{
          Serial.println("Wrong Password. Try Again");
          for(int l=0; l<4; l++)dataStr[l] = '0';
          Display.show("----");
          
          for(int m=0; m<5; m++){
          digitalWrite(LEDred,HIGH);
          delay(500);
          digitalWrite(LEDred,LOW);
          delay(500);
          }
          Display.clear();
          loginAttempt++;
          if(loginAttempt == 3) goto forcedOpen;
        }
      }
      else{
        mode = normal;
        Serial.println("Successfully Password Changer");
        Display.show("1111");
        for(int x=0; x<3; x++){
          digitalWrite(LEDyellow,HIGH);
          delay(300);
          digitalWrite(LEDyellow,LOW);
          delay(300);
          }
        digitalWrite(LEDyellow,HIGH);
        Display.clear();
      }
      
    }

  else if(sensorReading < thresshold){
    forcedOpen:
      Display.clear();
      digitalWrite(LEDblue, LOW);
  
      Serial.println("The the door is already open");
      Serial.println("Please close it first");
      digitalWrite(buzzer, HIGH);
      for(int m=0; m<5; m++){
      digitalWrite(LEDred,HIGH);
      delay(500);
      digitalWrite(LEDred,LOW);
      delay(500);
      }
      digitalWrite(buzzer, LOW);
      
      if ( loginAttempt == 3){
        Serial.println("ALARM SAFE 101");
        Serial.println("Wait for 5 sec!");
        delay(5000);
        loginAttempt == 0;
      }
      
      while(analogRead(LDR) < thresshold); // waits untill you remove your finger from the LDR sensor
   }
}

bool matchPassword(){
  for(int k=0; k<4; k++){
    if(dataStr[k] != password[k])return false;
    }
    return true;
  }

char numberToCharConversion(int j){
  if(j==1) {
    if( mode == setPassword )password[digitNumber] = '1';
    else dataStr[digitNumber] = '1';
  }
  else if( j==2 ){
    if(mode == setPassword )password[digitNumber] = '2';
    else dataStr[digitNumber] = '2';
  }
  else if(j==3){
    if(mode == setPassword )password[digitNumber] = '3';
    else dataStr[digitNumber] = '3';
  }
  else if(j==4){
    if(mode == setPassword )password[digitNumber] = '4';
    else dataStr[digitNumber] = '4';
  }
  
}
