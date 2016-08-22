#include <Keypad.h>
//#include <Servo.h>

//Servo servo_Motor;
char* password = "8294";
int password_len = 4;

int position = 0;
int guessed_digit = 0;
boolean mlocked = true;
int speakerPin = 13;

int relePin = 12;

String enteredPassword = "";

//int buttonPin = 13;
int redPin = 11;
int greenPin = 10;

const byte ROWS = 4; // Four rows
const byte COLS = 3; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 8, 7, 6, 5 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 4, 3, 2 }; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//#define ledpin 13
int buttonState = 0;

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  
  //pinMode(buttonPin, INPUT);
  //pinMode(ledpin,OUTPUT);
  //digitalWrite(ledpin, HIGH);
  
  //servo_Motor.attach(9);
  
  
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
  //Serial.begin(9600);
}

void loop()
{
  
  
  //buttonState = digitalRead(buttonPin);
  
  /*
  if(buttonState == HIGH){
     if(mlocked){
       //makeSound();
       setLocked(false);
     }
     else{
       makeSound();
       setLocked(true);
     }
     
     delay(100);
  }
  */
  
  char key = kpd.getKey();
  if(key){
    
    //enteredPassword += key;
    //Serial.println(key);//'pwd' + enteredPassword);
    makeSound();
    
  }
  
if (key == '*' || key == '#')
{
position = 0;
guessed_digit = 0;
//enteredPassword = "";
//Serial.println("reset");

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
/*
key == password[position]
){
  Serial.println('same len');
  if(password == enteredPassword){
     setLocked(false);
  }
  else{
    soundWrongSignal();
  }
  
  enteredPassword = "";
}*/


delay(100);
}

void setLocked(boolean locked){
 if(locked == mlocked){
  return;
 }
 
 
 if (locked){
   digitalWrite(redPin, HIGH);
   digitalWrite(greenPin, LOW);
   //servo_Motor.write(0);
   digitalWrite(relePin, LOW);
 }
 else{
  
  //Serial.println("unlocked");
  //servo_Motor.write(90);
  digitalWrite(relePin, HIGH);
  
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  
  
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


