#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUMPIXELS      8

const int micPin = 4;
const int shakePin = 2;

const int ledMicPin = 7;
const int ledShakePin = 6;

int micTriggeredTime = 0;
int micLastTime = 0;
int shakeTriggeredTime = 0;

int micTriggeringCount = 0;

Adafruit_NeoPixel micPixels = Adafruit_NeoPixel(NUMPIXELS, ledMicPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel shakePixels = Adafruit_NeoPixel(NUMPIXELS, ledShakePin, NEO_GRB + NEO_KHZ800);

boolean ledMicActive = false;
int ledMicActiveIndex = 0;

boolean ledShakeActive = false;
int ledShakeActiveIndex = 0;

const int ledStepTime = 100;
int ledMicLastTime = 0;
int ledShakeLastTime = 0;

boolean isMicTop = false;
boolean isShakeTop = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(micPin, INPUT);
  pinMode(shakePin, INPUT);

  //#if defined (__AVR_ATtiny85__)
  //  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  //#endif
  micPixels.begin(); 
  shakePixels.begin(); 
  //
  turnOffMicLedLine();
  turnOffShakeLedLine();
  //turnOneLedLine();
  //turnOneLedLine();
}

void turnOffMicLedLine(){
   micPixels.clear();
   for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    micPixels.setPixelColor(i, micPixels.Color(0,0,0)); // Moderately bright green color.
   }  
    micPixels.show();

}

void turnOffShakeLedLine(){
   shakePixels.clear();
   for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    shakePixels.setPixelColor(i, shakePixels.Color(0,0,0)); // Moderately bright green color.
   }  
    shakePixels.show();

}

void checkMicLedLines(){
  /*if(!ledMicActive){
    return;  
  }*/
  if( !(ledMicActive || ledMicActiveIndex)){
    return;
  }

  Serial.println(ledMicActiveIndex);

  int currentMills = millis();
  if(currentMills - ledMicLastTime < ledStepTime){
    return;
  }

  ledMicLastTime = currentMills;
  
  
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if(i <= ledMicActiveIndex){
      micPixels.setPixelColor(i, micPixels.Color(0,150,0)); // Moderately bright green color.
    }  
    else{
      micPixels.setPixelColor(i, micPixels.Color(0,0,0)); // Moderately bright green color.
    }
   }  
   micPixels.show();

   if(ledMicActive){
    ledMicActiveIndex++;
   }
   else{
    ledMicActiveIndex--;
   }

   if(ledMicActiveIndex >= NUMPIXELS){
     //ledMicActiveIndex = 0;
     ledMicActiveIndex = NUMPIXELS - 1;
     //ledMicActive = false;
     isMicTop = true;
     //turnOffMicLedLine();
   }
   else {
    isMicTop = false;
   }

   if(ledMicActiveIndex <= 0){
    ledMicActiveIndex = 0;
    turnOffMicLedLine();
   }

   ledMicActive = false;
}

void checkShakeLedLines(){
  if(! (ledShakeActive || ledShakeActiveIndex)){
    return;  
  }

  int currentMills = millis();
  if(currentMills - ledShakeLastTime < ledStepTime){
    return;
  }

  ledShakeLastTime = currentMills;
  
  
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if(i <= ledShakeActiveIndex){
      shakePixels.setPixelColor(i, shakePixels.Color(0,150,0)); // Moderately bright green color.
    }  
    else{
      shakePixels.setPixelColor(i, shakePixels.Color(0,0,0)); // Moderately bright green color.
    }
   }  
   shakePixels.show();

   if(ledShakeActive){
    ledShakeActiveIndex++;
   }
   else{
    ledShakeActiveIndex--;
   }

   if(ledShakeActiveIndex >= NUMPIXELS){
     isShakeTop = true;
     //ledShakeActiveIndex = 0;
     ledShakeActive = false;
     //turnOffShakeLedLine();
   }
   else {
    isShakeTop = false;
   }

   if(ledShakeActiveIndex <= 0){
     ledShakeActiveIndex = 0;
     ledShakeActive = false;
     turnOffShakeLedLine();
   }
}


void turnOneMicLedLine(){
   ledMicActive = true;
}

void turnOneShakeLedLine(){
   ledShakeActive = true;
}

int soundTriggerIndex = 0;
void loop() {
  // put your main code here, to run repeatedly:
   int currentMills = millis();
   int soundVal = digitalRead(micPin);
   int shakeVal = digitalRead(shakePin);

   //if(soundVal){
   // soundTriggerIndex
   //}
   
   if(soundVal){
     micLastTime = currentMills;
     if(!micTriggeredTime){
       micTriggeredTime = currentMills;
     }
     //micTriggeringCount++;
     //Serial.println("sound");
   }
   
   if(!soundVal && (currentMills - micLastTime) > 300 ){
     //micTriggeringCount = 0;
     micTriggeredTime = 0;
     //Serial.println("jopa");
     
   }


   if(micTriggeredTime > 0){
    //Serial.println(currentMills - micTriggeredTime); 
   }

   //if(soundVal && 
   if(micTriggeredTime > 0 &&  ((currentMills - micTriggeredTime) >= 500) ){
    //soundTriggerIndex++;
    Serial.println("sound");
    //if(soundTriggerIndex > 10){
      turnOneMicLedLine();
    //}
    //Serial.print(micTriggeredTime);
    //Serial.print( " ");
    //Serial.println(currentMills - micTriggeredTime );
   }
   /*
   else if((currentMills - micTriggeredTime) > 100){
    soundTriggerIndex--;
    if(soundTriggerIndex < 0){
      soundTriggerIndex = 0;
    }
   }*/

   if(shakeVal){
     shakeTriggeredTime = currentMills;
     turnOneShakeLedLine();
   }
   
   //Serial.print(shakeVal);
   //Serial.print(" ");
   //Serial.println(soundVal);

   //if( shakeTriggeredTime > 0 && abs(shakeTriggeredTime - micTriggeredTime) < 500){
   if( shakeVal && micTriggeredTime > 0 && (currentMills - micTriggeredTime > 2000)  ){
     Serial.println("GOOD");
     //turnOneLedLine();
   }

   checkShakeLedLines();
   checkMicLedLines();

   //

}
