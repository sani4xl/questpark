/*
 * распаять 
 * D3 rele
 * D4 Оптический датчик препятствия 
 * D2 кнопка
 * 5в 4 слота
 * gnd 4 слота
 */

#include <Bounce2.h>

int buttonPin = 2;    //The pin location of the sensor
int relePin = 3;
int indicatorPin = 4;
Bounce bouncer;

void setup() //
{ 
  
  Serial.begin(9600);
  pinMode(relePin, OUTPUT);
  pinMode(indicatorPin, INPUT); //initializes digital pin 2 as an input

  pinMode(buttonPin, INPUT_PULLUP);
  bouncer = Bounce();
  bouncer.attach(buttonPin);
  bouncer.interval(5); // interval in ms
} 


void unlock() {
  digitalWrite(relePin, HIGH);
}

void lock() {
  digitalWrite(relePin, LOW);
}

void loop () {

  int cardPresent = digitalRead(indicatorPin);
  bouncer.update();
  
  if (cardPresent > 0 && bouncer.fell()) {
    
      unlock();
      delay(60000);
      lock();
  } 
}
