#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUMPIXELS      8

const int micPin = 4;
const int shakePin = 2;
const int ledPin = 6;
boolean micTriggeredTime = 0;
boolean shakeTriggeredTime = 0;

boolean micTriggeringCount = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);
boolean led1Active = false;
int led1ActiveIndex = 0;
const int ledStepTime = 1000;
int ledLastTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(micPin, INPUT);
  pinMode(shakePin, INPUT);

  //#if defined (__AVR_ATtiny85__)
  //  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  //#endif
  pixels.begin(); 
  //
  turnOffLedLine();
  //turnOneLedLine();
  //turnOneLedLine();
}

void turnOffLedLine(){
   pixels.clear();
   for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
   }  
    pixels.show();

}

void checkLedLines(){
  if(!led1Active){
    return;  
  }

  int currentMills = millis();
  if(currentMills - ledLastTime < ledStepTime){
    return;
  }

  ledLastTime = currentMills;
  
  
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if(i <= led1ActiveIndex){
      pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
    }  
    else{
      pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
    }
   }  
   pixels.show();

   led1ActiveIndex++;

   if(led1ActiveIndex >= NUMPIXELS){
     led1ActiveIndex = 0;
     led1Active = false;
   }
}


void turnOneLedLine(){
   led1Active = true;

}


void loop() {
  // put your main code here, to run repeatedly:
   int currentMills = millis();
   int soundVal = digitalRead(micPin);
   int shakeVal = digitalRead(shakePin);
   
   if(soundVal && !micTriggeredTime){
     micTriggeredTime = currentMills;
     //micTriggeringCount++;
   }
   else{
     //micTriggeringCount = 0;
     micTriggeredTime = 0;
   }

   if(shakeVal){
     shakeTriggeredTime = currentMills;
   }
   
   Serial.print(shakeVal);
   Serial.print(" ");
   Serial.println(soundVal);

   //if( shakeTriggeredTime > 0 && abs(shakeTriggeredTime - micTriggeredTime) < 500){
   if( shakeVal && micTriggeredTime > 0 && (currentMills - micTriggeredTime > 2000)  ){
     Serial.println("GOOD");
     turnOneLedLine();
   }

   checkLedLines();

   //

}
