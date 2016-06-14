//#include <VarSpeedServo.h>

const int downPin  = 3;  // 1
const int topPin   = 5;  // 2
const int leftPin  = 2;  // 3
const int rightPin = 4;  // 4

const int downDir  = 1;
const int topDir   = 2;
const int leftDir  = 3;
const int rightDir = 4;

const int servoPin = 10;

//const int lockPin = 8;
const int speakerPin = 9;

#include <Servo.h>
Servo myservo;
//VarSpeedServo myservo;
int servoPos = 0;

int password[] = { leftDir , downDir , leftDir , topDir, rightDir,
 downDir, rightDir, topDir, leftDir };
int passwordLen = 9;
int key;
int position = 0;
int guessed_digit = 0;


// variables will change:
int downState = 0;
int topState = 0;
int leftState = 0;
int rightState = 0;
boolean buttonPressed = false;

boolean isOpened = false;

void setup() {
  // initialize the LED pin as an output:
  
  // initialize the pushbutton pin as an input:
  pinMode(topPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  
  //pinMode(lockPin, OUTPUT);
  
  pinMode(speakerPin, OUTPUT);
  
  analogWrite (speakerPin, 255);
  
  resetServo();  
  //openServo();  
  
  Serial.begin(9600);
}

void loop() {
  // read the state of the pushbutton value:
  topState = digitalRead(topPin);
  downState = digitalRead(downPin);
  leftState = digitalRead(leftPin);
  rightState = digitalRead(rightPin);
  buttonPressed = false;
  if(topState){
    Serial.println("top"); 
    //delay(500);
    key = topDir;
    buttonPressed = true;
  }
  else if(downState){
    Serial.println("down"); 
    key = downDir;
    buttonPressed = true;
    //delay(500);
  }
  else if(leftState){
    Serial.println("left"); 
    key = leftDir;
    buttonPressed = true;
    //delay(500);
  }
  else if(rightState){
    Serial.println("right"); 
    key = rightDir;
    buttonPressed = true;
    //delay(500);
  }
  
  if(buttonPressed){
    
    
   
    
  if(key){
    
    if(key == password[position]){
      position ++;
      //guessed_digit++;
    }
    
    //position ++;
    
    else if(key == password[0]){
      position = 1;
    }
    else{
      position = 0;
    }
    
    Serial.print(position);
    Serial.print(" " );
    Serial.println(guessed_digit);
    
    
    if(position >= passwordLen){
      /*
      if(guessed_digit == passwordLen){ // valid password
        playOpenSound();
        openServo();
        
      }
      else{
        playInvalidSound();
      }
      */
      playOpenSound();
      openServo();
      
      position = 0;
      guessed_digit = 0;
    }
    else{
      if(isOpened){
        resetServo();
      }  
    }
    
    
    
  } 
 
    analogWrite (speakerPin, 0);
    delay(50);
    analogWrite (speakerPin, 255); 
    delay(200);
  } 
  
  //Serial.println(position); 
  /*
  if (position == 4){
    
    playOpenSound();
    openServo();
  }
  else if(buttonPressed && isOpened){
    resetServo();
  }
  
  delay(400);
  
  }
  */

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  /*if (buttonState == HIGH) {
    // turn LED on:
    Serial.println("on"); 
    digitalWrite(lockPin, HIGH);
    delay(1000);
    digitalWrite(lockPin, LOW);
    
  } else {
    // turn LED off:
    
  }*/
}

void openServo(){
   if(!isOpened){
      myservo.attach(servoPin);
      delay(100); 
      myservo.write(180);
      delay(1000);  
      myservo.detach();
      isOpened = true;
    }
}

void resetServo(){
    myservo.attach(servoPin);
    delay(100); 
    myservo.write(40);
    delay(1000);  
    myservo.detach();
    isOpened = false;
}

void playInvalidSound(){
  analogWrite (speakerPin, 0);
   delay(500);
   analogWrite (speakerPin, 255);
}

void playOpenSound(){
   analogWrite (speakerPin, 0);
   delay(50);
   analogWrite (speakerPin, 255);
   delay(50);
   analogWrite (speakerPin, 0);
   delay(50);
   analogWrite (speakerPin, 255);
   delay(50);
   analogWrite (speakerPin, 0);
   delay(50);
   analogWrite (speakerPin, 255);
   
}
