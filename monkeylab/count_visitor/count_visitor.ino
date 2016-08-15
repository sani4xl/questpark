/***********************************************
 * PROJECT:  COUNT VISITOR
 * CREATORS: MonkeyLab @..@ SANSAN and MANJULIFE
 * DATE:     31-7-2016
 * VERSION:  1.00
***********************************************/

#include <NewPing.h>
 
#define TRIGGER_PIN 7
#define ECHO_PIN 6
#define MAX_DISTANCE 200
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
 
void setup() {
   Serial.begin(9600);
}
 
void loop() {
   delay(50);
   unsigned int uS = sonar.ping_cm();
   Serial.print(uS);
   Serial.println("cm");
}
