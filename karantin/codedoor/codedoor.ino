#include "Keypad.h"
 
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'#','0','*'}
};
byte rowPins[ROWS] = { 3, 4, 5, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 8, 9}; //connect to the column pinouts of the keypad
 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char* password = "384";
int password_len = 3;

int position = 0;
int guessed_digit = 0;
boolean mlocked = true;
int speakerPin = 2;

int relePin = 13;

String enteredPassword = "";

void setup()
{
  Serial.begin(9600);

  pinMode (speakerPin, OUTPUT);
  analogWrite (speakerPin, 255);
  delay(50);
  analogWrite (speakerPin, 255);
  delay(100);
  
  pinMode (relePin, OUTPUT);
  digitalWrite(relePin, LOW);
  //servo_Motor.write(0);
  
  mlocked = false;
  setLocked(true);
  
 
  enteredPassword = "";
}
 
void loop()
{
//char key = keypad.getKey();
 
 char key = keypad.getKey();
 if(key){
    makeSound();
 }

  // reseting
 if (key == '*' || key == '#'){
   position = 0;
   guessed_digit = 0;
   setLocked(true); 
 }
 else if(key){
   if(key == password[position]){
     guessed_digit++;
   }
  
  //Serial.println("check");
  //Serial.print("Position: "); Serial.println(position);
  //Serial.print("Gussed: "); Serial.println(guessed_digit);
  
    position++;
    if(position>=password_len){
    //Serial.println(11111);
      if(guessed_digit == password_len){
        setLocked(false);
      }
      else{
        soundWrongSignal();
      }
    
      position = 0;
      guessed_digit = 0;
    }
  }

  delay(100);
}



void setLocked(boolean locked){
 if(locked == mlocked){
  return;
 }
 
 
 if (locked){
   //digitalWrite(redPin, HIGH);
   //digitalWrite(greenPin, LOW);
   //servo_Motor.write(0);
   digitalWrite(relePin, LOW);
 }
 else{
  
  //Serial.println("unlocked");
  //servo_Motor.write(90);
  digitalWrite(relePin, HIGH);
  
  //digitalWrite(redPin, LOW);
  //digitalWrite(greenPin, HIGH);
  
  
  delay(100);
  
  makeSound();
  delay(100);
  makeSound();
  delay(100);
  makeSound();
  delay(100);
  makeSound();
  delay(100);
  
  
 }
 
 mlocked = locked;
}

void soundWrongSignal(){
  //Serial.println("wrong");
  analogWrite (speakerPin, 0);
  delay (600);
  analogWrite (speakerPin, 255);
}

void makeSound(){
 
  analogWrite (speakerPin, 0);
  delay (50);
  analogWrite (speakerPin, 255);
  delay(50);
}


