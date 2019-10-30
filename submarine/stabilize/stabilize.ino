/*
  распаять ардуино нано:
  - кнопки 3 d10-d12
  - led d2-d4 (din)
  - rele d5
  - 5v 4 slots
  - gnd - 6 slots
*/

#include <Bounce2.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUMPIXELS 16

const int ledStepTime = 200; 
int ledLastTime;

int relePin = 5;

const int ledStripsCount = 2;
int ledStripPins[ledStripsCount] = {2, 3};
int buttonPins[ledStripsCount] = {11, 12};
boolean ledStripActive[ledStripsCount] = {true, true};
int ledStripIndex[ledStripsCount] = {1, 6};

int ledStripDirection[ledStripsCount] = {1, 1};

int ledStripGreenPins[ledStripsCount] = {3, 6};

Adafruit_NeoPixel ledStrips[ledStripsCount] ;
Bounce bouncers[ledStripsCount] ;

boolean isWin = false;

void setup() {
  // put yourdebouncer setup code here, to run once:
  Serial.begin(9600);
  
  for(int i = 0 ; i< ledStripsCount; i++){
    ledStrips[i] = Adafruit_NeoPixel(NUMPIXELS, ledStripPins[i], NEO_GRB + NEO_KHZ800);
    ledStrips[i].begin();
    ledStrips[i].setBrightness(10);
    ledStrips[i].show();

    pinMode(buttonPins[i], INPUT_PULLUP);
    bouncers[i] = Bounce();
    bouncers[i].attach(buttonPins[i]);
    bouncers[i].interval(5); // interval in ms
  }

  ledLastTime = millis();

  pinMode(relePin, OUTPUT);
  gameRestart();
}

void gameRestart() {
  isWin = false;
  for(int i = 0; i < ledStripsCount; i++){
    ledStripActive[i] = true;
    ledStripIndex[i] = random(1, NUMPIXELS);
    ledStripGreenPins[i] = random(1, NUMPIXELS);
    ledStripDirection[i] = random(1, 100) % 2 == 0 ? 1 : -1;
  }
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

void unlock() {
  digitalWrite(relePin, HIGH);
}

void lock() {
  digitalWrite(relePin, LOW);
}

void setPixelsColor(Adafruit_NeoPixel &pixels, uint32_t color){
   pixels.clear();
   for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, color); // Moderately bright green color.
   }  
   pixels.show();

}

void lightPixels(Adafruit_NeoPixel &firstPixel, int ledIndex, int greenIndex) {
   firstPixel.clear();
   firstPixel.setPixelColor(ledIndex, greenIndex == ledIndex ?  firstPixel.Color(0,0,255) :  firstPixel.Color(200,100,0) ); // Moderately bright green color.
   firstPixel.show(); 
}

void renderPixels(){
  
  int currentMills = millis();
  if(currentMills - ledLastTime < ledStepTime){
    return;
  }

  ledLastTime = currentMills;

  for(int i = 0 ; i< ledStripsCount; i++) {

    if(!ledStripActive[i]){
      continue;
    }

    

    int ledIndex = ledStripIndex[i];
    ledIndex += ledStripDirection[i];
    if (ledIndex >= NUMPIXELS) {
      ledIndex = NUMPIXELS - 1;
      ledStripDirection[i] = -1;
    } else if (ledIndex < 0){
      ledIndex = 0;
      ledStripDirection[i] = 1;
    }

    ledStripIndex[i] = ledIndex;
    
    int greenIndex = ledStripGreenPins[i];
    lightPixels(ledStrips[i], ledIndex, greenIndex);

    
  }

  checkForWin();
  
}

void colorifyStrips(uint8_t r, uint8_t g, uint8_t b){
  
  for(int i = 0; i < ledStripsCount; i++){
    setPixelsColor( ledStrips[i] , ledStrips[i].Color( r,g,b ) );
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
    unlock();
    isWin = true;
    for(int i = 0; i< 5; i++){
      colorifyStrips(0,255,0);
      delay(200);
      colorifyStrips(0,0,0);
      delay(200);
    }
    
    
    delay(60000);
    gameRestart();
  }
  
}

void checkButtons(){
    for(int i =0 ; i< ledStripsCount; i++){
       Bounce bouncer = bouncers[i];
       bouncers[i].update();

       if ( bouncers[i].read() ) {
         ledStripActive[i] = !ledStripActive[i];
       }
    }
  
}

void loop() {
  int currentMills = millis();
  checkButtons();
  renderPixels();
}

