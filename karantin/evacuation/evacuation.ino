#include <Bounce2.h>

const int buttonPin = 4;
Bounce buttonBounce = Bounce(); 

void setup() {
   Serial.begin(9600);
  // put your setup code here, to run once:
   pinMode(buttonPin, INPUT);
   buttonBounce.attach(buttonPin);
   buttonBounce.interval(10); 

}

void loop() {
  // put your main code here, to run repeatedly:
  if(buttonBounce.rose()){
    Serial.println("start");
  }

}
