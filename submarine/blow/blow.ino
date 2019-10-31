// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
// Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
// http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com
// https://forum.seeedstudio.com/viewtopic.php?f=4&t=989&p=3632
/*
 * Изделие №4
 * распаять 
 * D3 rele
 * D2 желтый провод крутилки через резистер + питание
 * D4 светодиод RGB out
 * 5в 4 слота
 * gnd 4 слота
 * добавить светодиод RGB
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUMPIXELS 16

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               
int hallsensorPin = 2;    //The pin location of the sensor
int relePin = 3;
int ledPin = 4;
unsigned long calculateLastTime;
boolean calculationActive = false;
unsigned long powerBlowTime = 0;
Adafruit_NeoPixel led;
int threshold = 10000;

void rpm ()     //This is the function that the interupt calls 
{ 
  NbTopsFan++;  //This function measures the rising and falling edge of the 
} 
// The setup() method runs once, when the sketch starts
void setup() //
{ 
  pinMode(hallsensorPin, INPUT); //initializes digital pin 2 as an input
  Serial.begin(9600);
  attachInterrupt(0, rpm, RISING); //and the interrupt is attached
  calculateLastTime = millis();
  calculationActive = false;
  pinMode(relePin, OUTPUT);

  led = Adafruit_NeoPixel(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);
  led.begin();
  led.show();
} 


void unlock() {
  digitalWrite(relePin, HIGH);
}

void lock() {
  digitalWrite(relePin, LOW);
}

void turnOffPixels(){
   led.clear();
   led.show();
}

void turnOnPixels(float brightness) {

   Serial.println(round(255.0 * (1.0 - brightness)));
  
   led.clear();
   led.setPixelColor(0, led.Color( round(255.0 * (1.0 - brightness)), round(255.0 * brightness), 0)); // Moderately bright green color.
   //led.setPixelColor(0, led.Color( 100, 100, 0)); // Moderately bright green color.
   led.show();
}

void loop () {
  unsigned long currentTime = millis();
  if (!calculationActive) {
    NbTopsFan = 0;  //Set NbTops to 0 ready for calculations
    sei();    //Enables interrupts
    calculationActive = true;
  }
  if (currentTime - calculateLastTime > 1000) {
    
    //delay (1000); //Wait 1 second
    cli();    //Disable interrupts
    Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour 
    calculationActive = false;
    calculateLastTime = currentTime;
  }

  if (Calc > 500) {
    if (powerBlowTime == 0) {
      powerBlowTime = currentTime;
    }
  } else {
    powerBlowTime = 0;
    turnOffPixels();
  }
  
  //Serial.print (Calc, DEC); //Prints the number calculated above
  //Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a  new line
  unsigned long deltaTime = currentTime - powerBlowTime;
  
  if (powerBlowTime > 0) {
    //Serial.println ( (float)deltaTime / (float)threshold);
    turnOnPixels( (float)deltaTime / (float)threshold);
    if (deltaTime > threshold) {
      unlock();
      delay(60000);
      lock();
      powerBlowTime = 0;
    }
  } 
}
