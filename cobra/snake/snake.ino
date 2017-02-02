/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int headPin = 9;
int tailPin = 5;
int pos = 95;    // variable to store the servo position

unsigned long lastHeadTime = 0;
unsigned long lastTailTime = 0;

void setup() {
  Serial.begin(9600);
  //myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(tailPin, OUTPUT); 
  //analogWrite(tailPin, 167);
  analogWrite(tailPin, 0);
}

void loop() {
  //return;
  unsigned long now = millis();
  int headSecondsPassed = (now-lastHeadTime)/1000;

  if(headSecondsPassed > 5){
    myservo.attach(9);
    
    if(pos <= 95){ 
      for (pos = 95; pos <= 115; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(50);                       // waits 15ms for the servo to reach the position
      }
    }
    else{
      for (pos = 115; pos >= 95; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(50);                       // waits 15ms for the servo to reach the position
      }
    }
    myservo.detach();  
    delay(500);
    lastHeadTime = now;
  }  


  int tailSecondsPassed = (now-lastTailTime)/1000;

  if(tailSecondsPassed > 8){
    Serial.println("gogog");
    for(int i = 0; i < 3; i++){
      analogWrite(tailPin, 130);
      delay(800);
      analogWrite(tailPin, 0);
      delay(300);
    }
    
    lastTailTime = now;
  }  
  
}

