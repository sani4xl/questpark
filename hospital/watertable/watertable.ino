// scheme https://www.seeed.cc/topic_detail.html?id=575
#define RELAY_PIN 5
volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc; 
int hallsensor = 2; //The pin location of the sensor
int totalLiquid = 0;

#define LED_PIN 13

#define LIQUID_REQUIRED 50

void rpm () //This is the function that the interupt calls 
{ 
NbTopsFan++; //This function measures the rising and falling edge of the 

//hall effect sensors signal
} 
// The setup() method runs once, when the sketch starts
void setup() //
{ 
pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
Serial.begin(9600); //This is the setup function where the serial port is 
pinMode(RELAY_PIN, OUTPUT);
pinMode(LED_PIN, OUTPUT);
closeLock();
//initialised,
attachInterrupt(0, rpm, RISING); //and the interrupt is attached
} 
// the loop() method runs over and over again,
// as long as the Arduino has power
boolean isDoorOpened = false;
int starOpenTimeRange = 0;
#define OPENING_TIME_RANGE  10
void loop () 
{
NbTopsFan = 0;  //Set NbTops to 0 ready for calculations
sei();  //Enables interrupts
delay (1000); //Wait 1 second
cli();  //Disable interrupts
//Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate 
Calc = (NbTopsFan * 60 / 2); //(Pulse frequency x 60) / 7.5Q, = flow rate 
if(Calc > 0){
  totalLiquid += Calc;
}


Serial.print(" total : ");
Serial.println(totalLiquid);

if(totalLiquid >= LIQUID_REQUIRED && !isDoorOpened){
  starOpenTimeRange = millis() / 1000;
  openLock();
  isDoorOpened = true;
  Serial.println("open door");
  
}

if(isDoorOpened && (millis()/1000 - starOpenTimeRange > OPENING_TIME_RANGE )){
  closeLock();
  totalLiquid = 0;
  isDoorOpened = false;
  Serial.println("reseting");
}


Serial.print (Calc, DEC); //Prints the number calculated above
Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a new line
}

void closeLock(){
  Serial.println("close lock");
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

void openLock(){
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
}

