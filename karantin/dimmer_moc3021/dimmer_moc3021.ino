#include <TimerOne.h>           // Avaiable from http://www.arduino.cc/playground/Code/Timer1

volatile int i=0;               // Variable to use as a counter
volatile boolean zero_cross=0;  // Boolean to store a "switch" to tell us if we have crossed zero
int AC_pin = 3;                 // Output to Opto Triac
int buton1 = 4;                 // first button at pin 4
int buton2 = 5;                 // second button at pin 5
int dim2 = 0;                   // led control
int dim = 0;                  // Dimming level (0-128)  0 = on, 128 = 0ff
int pas = 8;                    // step for count;
// version: 4m7 (15.04.2013 - Craiova, Romania) - 16 steps, 4 button & LED blue to red (off to MAX) 
// version: 7m3 (22.01.2014 - Craiova, Romania) - 16 steps, 2 button & LCD1602

int freqStep = 90;    // This is the delay-per-brightness step in microseconds for 50Hz (change the value in 65 for 60Hz)

 
void setup() {  // Begin setup
  Serial.begin(9600);   
  pinMode(buton1, INPUT);  // set buton1 pin as input
  pinMode(buton2, INPUT);  // set buton1 pin as input
  pinMode(AC_pin, OUTPUT);                          // Set the Triac pin as output
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);      
  // Use the TimerOne Library to attach an interrupt

}

void zero_cross_detect() {    
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  i=0;
  digitalWrite(AC_pin, LOW);
}                                 

// Turn on the TRIAC at the appropriate time
void dim_check() {                   
  if(zero_cross == true) {              
    if(i>=dim) {                     
      digitalWrite(AC_pin, HIGH);  // turn on light       
      i=0;  // reset time step counter                         
      zero_cross=false;    // reset zero cross detection
    } 
    else {
      i++;  // increment time step counter                     
    }                                
  }    
}                                      

void loop() {  
  dimS( 70,  79,  89);
  dimD( 50,  88,  79);
  delay(500);
  dimS( 110,  87,  102); // night
  dimD( 70,  101,  75);
  delay(700);
  dimS( 90,  79,  89);
  dimD( 70,  88,  79);
  delay(500);
  dimS( 110,  79,  89);
  dimD( 50,  88,  79);
  delay(500);

// for (int i=105; i >= 70; i--){
//    dim=i;
//    delay(90);
//      Serial.println(dim);
//  }

 // for (int i=128; i >= 5; i--){
//    dim=106;
//    delay(10);
//   }  
}

void dimS(int del, int on, int off) {
   for (int i=on; i <= off; i++){
    dim=i;
    delay(del);
      Serial.println(dim);
  }
  }
void dimD(int del, int on, int off) {
   for (int i=on; i >= off; i--){
    dim=i;
    delay(del);
      Serial.println(dim);
  }
  }
