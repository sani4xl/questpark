/*
 * Изделие №1
распаять плату под ардуино нано:
4 выхода D9-D12
2 выхода D2-D3 на два гнезда каждый
5v на 4 гнезда
GND на 8 гнезд

LED ленты выход DIN -> D2-D3, на выходе две красных две зеленых
radio-rele все B -> GND, A -> D9-D12

*/


#include <Bounce2.h>

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define NUMPIXELS 16

const int ledStepTime = 500; // 1 sec
int ledLastTime;

int teamRedIndex = -1;
int teamRedStripPin = 3;

int teamGreenIndex = -1;
int teamGreenStripPin = 2;

Adafruit_NeoPixel *teamRedStrip;
Adafruit_NeoPixel *teamGreenStrip;

Bounce teamRedUpButton;
int teamRedUpButtonPin = 12;

Bounce teamRedDownButton;
int teamRedDownButtonPin = 11;

Bounce teamGreenUpButton;
int teamGreenUpButtonPin = 10;

Bounce teamGreenDownButton;
int teamGreenDownButtonPin = 9;

struct ButtonUpdateResult {
    int index;
    boolean updated;
};


void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  // put yourdebouncer setup code here, to run once:
  Serial.begin(9600);

  teamRedStrip = new Adafruit_NeoPixel(NUMPIXELS, teamRedStripPin, NEO_GRB + NEO_KHZ800);
  teamRedStrip->begin();
  teamRedStrip->setBrightness(10);
  teamRedStrip->show();

  teamGreenStrip = new Adafruit_NeoPixel(NUMPIXELS, teamGreenStripPin, NEO_GRB + NEO_KHZ800);
  teamGreenStrip->begin();
  teamGreenStrip->setBrightness(10);
  teamGreenStrip->show();

  initButton(teamRedUpButton, teamRedUpButtonPin);
  initButton(teamRedDownButton, teamRedDownButtonPin);
  initButton(teamGreenUpButton, teamGreenUpButtonPin);
  initButton(teamGreenDownButton, teamGreenDownButtonPin);
  
  ledLastTime = millis();

  
  Serial.println("initilized...");
  
}

void initButton(Bounce &button, int pin) {
  pinMode(pin, INPUT_PULLUP);
  button = Bounce();
  button.attach(pin);
  button.interval(5); // interval in ms
}


void lightPixels(Adafruit_NeoPixel *firstPixel, int ledIndex, bool isGreen) {
   firstPixel->clear();
   if (ledIndex >= 0 ) {
    firstPixel->fill(isGreen ?  firstPixel->Color(0,255,0) :  firstPixel->Color(255,0,0) , 0, ledIndex + 1);
   }
   firstPixel->show(); 
   delay(5);
}


void updateLights() {
  lightPixels(teamRedStrip, teamRedIndex, false);
  lightPixels(teamGreenStrip, teamGreenIndex, true);
}

ButtonUpdateResult monitorButton(Bounce &button, int index, int delta) {
  boolean updated = false;
  button.update();
  if(button.fell()){
      index += delta;
      if (index >= NUMPIXELS) {
        index = NUMPIXELS - 1;
      } 
      else if (index < -1) {
        index = -1;
      }
      else {
        updated = true;
      }
  }

  ButtonUpdateResult result = {index, updated};
  return result;
}

void checkButtons(){
  
  ButtonUpdateResult redUpResult = monitorButton(teamRedUpButton, teamRedIndex, 1); 
  teamRedIndex = redUpResult.index;

  ButtonUpdateResult redDownResult = monitorButton(teamRedDownButton, teamRedIndex, -1); 
  teamRedIndex = redDownResult.index;

  ButtonUpdateResult greenUpResult = monitorButton(teamGreenUpButton, teamGreenIndex, 1); 
  teamGreenIndex = greenUpResult.index;

  ButtonUpdateResult greenDownResult = monitorButton(teamGreenDownButton, teamGreenIndex, -1); 
  teamGreenIndex = greenDownResult.index;
  
    
  if (redUpResult.updated || redDownResult.updated || greenUpResult.updated || greenDownResult.updated) {
      updateLights();
  }
}

void loop() {
   int currentMills = millis();
   checkButtons();
}
