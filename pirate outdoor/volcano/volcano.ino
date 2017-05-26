#include <LiquidCrystal_I2C.h>

#include <Bounce2.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUMPIXELS      8

const int ledStepTime = 500; // 1 sec
int ledLastTime;


LiquidCrystal_I2C lcd(0x27, 16, 2);


const int ledStripsCount = 4;
int ledStripPins[ledStripsCount] = {3, 5, 6, 9};
int buttonPins[ledStripsCount] = {13, 8, 7, 12};
boolean ledStripActive[ledStripsCount] = {true, true, true, true};
int ledStripIndex[ledStripsCount] = {0, 0, 0, 0};

int ledStripGreenPins[ledStripsCount] = {3, 2, 4, 5};

Adafruit_NeoPixel ledStrips[ledStripsCount] ;
Bounce bouncers[ledStripsCount] ;
int ledIndex = 0;
/*Adafruit_NeoPixel pixel1;
Adafruit_NeoPixel pixel2;
Adafruit_NeoPixel pixel3;
Adafruit_NeoPixel pixel4;
*/
boolean isActive = false;

Bounce debouncer = Bounce(); 
const int buttonPin = 13; 

void setup() {
  // put yourdebouncer setup code here, to run once:
  Serial.begin(9600);
  //*
  for(int i =0 ; i< ledStripsCount; i++){
    ledStrips[i] = Adafruit_NeoPixel(NUMPIXELS, ledStripPins[i], NEO_GRB + NEO_KHZ800);
    ledStrips[i].begin();
    ledStrips[i].show();

    pinMode(buttonPins[i], INPUT);
    bouncers[i] = Bounce();
    bouncers[i].attach(buttonPins[i]);
    bouncers[i].interval(5); // interval in ms
  }
  //*/
  //pinMode(buttonPin, INPUT);
  //debouncer.attach(buttonPin);
  //debouncer.interval(5);

  ledLastTime = millis();

  //pinMode(buttonPin, INPUT);
  

  //firstPixel = Adafruit_NeoPixel(NUMPIXELS, 5, NEO_GRB + NEO_KHZ800);
  //firstPixel.begin();
  lcd.begin();
  
}


void turnOffPixels(Adafruit_NeoPixel pixels){
   //pixels.clear();
   for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
   }  
   pixels.show();

}

void turnOnPixels(Adafruit_NeoPixel pixels){
   pixels.clear();
   for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(255,0,0)); // Moderately bright green color.
   }  
   pixels.show();

}


void lightPixels(Adafruit_NeoPixel &firstPixel, int ledIndex, int greenIndex){

   //Serial.println(ledIndex);
   for(int j=0;j<NUMPIXELS;j++){
    if(j <= ledIndex ){
      
      firstPixel.setPixelColor(j, greenIndex == j ?  firstPixel.Color(0,255,0) :  firstPixel.Color(255,0,0) ); // Moderately bright green color.
    }
    else{
      firstPixel.setPixelColor(j, firstPixel.Color(0,0,0)); // Moderately bright green color.
    }
   }
    
   firstPixel.show(); 
   //delay(100);
}

void renderPixels(){
  //if(!isActive){
  //  return;  
  //}
  
  int currentMills = millis();
  if(currentMills - ledLastTime < ledStepTime){
    return;
  }

  ledLastTime = currentMills;

  for(int i =0 ; i< ledStripsCount; i++){

    if(!ledStripActive[i]){
      continue;
    }

    //Serial.print(i);
    //Serial.print(' ' );
    
    //Serial.print(' ' );
    

    int ledIndex = ledStripIndex[i];
    ledIndex++;
    if(ledIndex >= NUMPIXELS){
      ledIndex = 0;
    }

    ledStripIndex[i] = ledIndex;
    
    //Serial.println(ledIndex);

    
    int greenIndex = ledStripGreenPins[i];
    lightPixels(ledStrips[i], ledIndex, greenIndex);

    
  }
  
  

  
  
}

void checkForWin(){
  int sum = 0;
  for(int i = 0; i < ledStripsCount; i++){
    if(ledStripActive[i] ){
      continue;
    }

    if(ledStripIndex[i] == ledStripGreenPins[i]){
      sum ++;
    }
    
  }

  if(sum >= ledStripsCount){
    Serial.println("win");  
    renderWinMsg();
  }
  
}

void renderWinMsg(){
  lcd.backlight();
  lcd.print("WIN!");
}


void checkButtons(){
    for(int i =0 ; i< ledStripsCount; i++){
       Bounce bouncer = bouncers[i];
       bouncers[i].update();

       if ( bouncers[i].rose() ) {
        Serial.println("JOP");
        ledStripActive[i] = !ledStripActive[i];
       }
    }
  
}

void loop() {
  //debouncer.update();
  // put your main code here, to run repeatedly:
   int currentMills = millis();
   //updatePixelStrips();
  checkButtons();
  
  renderPixels();

   //int value = debouncer.read();

  // Turn on or off the LED as determined by the state :
  /*
  bouncers[0].update();
  if ( bouncers[0].rose() ) {
    //ledStripActive[0] = !ledStripActive[0];
    Serial.println("koko");
  }
  */
  

  //Serial.println(ledStripActive[0]);

  checkForWin();
  
  
  //debouncer.rose()
   

}
