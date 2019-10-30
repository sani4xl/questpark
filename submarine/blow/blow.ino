// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
// Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
// http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com
// https://forum.seeedstudio.com/viewtopic.php?f=4&t=989&p=3632
/*
 * распаять 
 * D3 rele
 * D2 желтый провод крутилки через резистер + питание
 * D4 светодиод RGB out
 * 5в 4 слота
 * gnd 4 слота
 * добавить светодиод RGB
 */

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               
int hallsensorPin = 2;    //The pin location of the sensor
int relePin = 3;
unsigned long calculateLastTime;
boolean calculationActive = false;
unsigned long powerBlowTime = 0;

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
} 


void unlock() {
  digitalWrite(relePin, HIGH);
}

void lock() {
  digitalWrite(relePin, LOW);
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
  }
  
  //Serial.print (Calc, DEC); //Prints the number calculated above
  //Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a  new line
  if (powerBlowTime > 0 && currentTime - powerBlowTime > 10000) {
    unlock();
    delay(60000);
    lock();
    powerBlowTime = 0;
  } 
}
