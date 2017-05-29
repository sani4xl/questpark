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
int mainLedIndex = 0;
/*Adafruit_NeoPixel pixel1;
Adafruit_NeoPixel pixledIndexel2;
Adafruit_NeoPixel pixel3;
Adafruit_NeoPixel pixel4;
*/
boolean isActive = false;

Bounce debouncer = Bounce(); 
const int buttonPin = 13; 

byte mcI[8] = { 0b01111, 0b10001, 0b10001, 0b01111, 0b00101, 0b01001, 0b10001, 0b00000 };
byte mcN[8] = { 0b10001, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b10001, 0b00000 };
byte mcL[8]   = {B00011,B00111,B00101,B00101,B01101,B01001,B11001,B00000,}; // Буква "Л"
byte mcP[8]   = {B11111,B10001,B10001,B10001,B10001,B10001,B10001,B00000,};
byte mcU[8]   = {B10001,B10001,B10001,B01010,B00100,B01000,B10000,B00000,}; // Буква "У"

const int ruI = 1;
const int ruL = 2;
const int ruU = 3;
const int ruP = 4;

boolean isWin = false;

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
  lcd.createChar(ruI, mcN);
  lcd.createChar(ruL, mcL);
  lcd.createChar(ruP, mcP);
  lcd.createChar(ruU, mcU);
  gameRestart();
  
}

void gameRestart(){
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("OCTAHOB");
  lcd.print(char(ruI));
  lcd.print("TE");
  lcd.setCursor(0,1);
  lcd.print("B");
  lcd.print(char(ruU));
  lcd.print(char(ruL));
  lcd.print("KAH");

  reinitStrips();
}

void reinitStrips(){
  isWin = false;
  for(int i = 0; i < ledStripsCount; i++){
    ledStripActive[i] = true;
    ledStripIndex[i] = 0;
    ledStripGreenPins[i] = random(1, NUMPIXELS);
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

void setPixelsColor(Adafruit_NeoPixel &pixels, uint32_t color){
   pixels.clear();
   for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, color); // Moderately bright green color.
   }  
   pixels.show();

}

void lightPixelsOne(Adafruit_NeoPixel &firstPixel, int ledIndex, int greenIndex){

}

void lightPixels(Adafruit_NeoPixel &firstPixel, int ledIndex, int greenIndex){

   //Serial.println(ledIndex);
   for(int j=0;j<NUMPIXELS;j++){
    //if(j <= ledIndex ){
    if(j == ledIndex ){
      
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
  
  int currentMills = millis();
  if(currentMills - ledLastTime < ledStepTime){
    return;
  }

  ledLastTime = currentMills;

  

  for(int i =0 ; i< ledStripsCount; i++){

    if(!ledStripActive[i]){
      continue;
    }

    

    int ledIndex = ledStripIndex[i];
    ledIndex++;
    if(ledIndex >= NUMPIXELS){
      ledIndex = 0;
    }

    ledStripIndex[i] = ledIndex;
    
    int greenIndex = ledStripGreenPins[i];
    lightPixels(ledStrips[i], ledIndex, greenIndex);

    
  }

  checkForWin();

  mainLedIndex++;
  if(mainLedIndex >= NUMPIXELS){
    mainLedIndex = 0;
    if(!isWin){
      /*colorifyStrips(255,0,0);
      delay(200);
      colorifyStrips(0,0,0);
      delay(200);
      colorifyStrips(255,0,0);
      delay(200);
      colorifyStrips(0,0,0);
      delay(200);
      colorifyStrips(255,0,0);
      delay(200);
      colorifyStrips(0,0,0);
      */
    }
    reinitStrips();
  }
  
  
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

  if(sum >= 1){//ledStripsCount){
    Serial.println("win");  
    renderWinMsg();
    isWin = true;
    for(int i =0; i< 5; i++){
      colorifyStrips(0,255,0);
      delay(200);
      colorifyStrips(0,0,0);
      delay(200);
    }
    
    
    delay(10000);
    gameRestart();
  }
  
}

void renderWinMsg(){
  lcd.backlight();
  lcd.clear();
  lcd.print(char(ruL));
  lcd.print("-");
  lcd.print(char(ruP));
  lcd.print("-");
  lcd.print(char(ruL));
  lcd.print("-");
  lcd.print(char(ruL));
  lcd.print("-");
  lcd.print(char(ruP));
  lcd.print("-");
  lcd.print(char(ruP));
  lcd.print("-");
  lcd.print(char(ruP));
  lcd.print("-");
  lcd.setCursor(0,1);
  lcd.print(char(ruL));
  lcd.print("-");
  lcd.print(char(ruP));
  lcd.print("-");
  lcd.print(char(ruP));
  lcd.print("-");
  lcd.print(char(ruL));
  lcd.print("-");
  lcd.print(char(ruL));
}


void checkButtons(){
    for(int i =0 ; i< ledStripsCount; i++){
       Bounce bouncer = bouncers[i];
       bouncers[i].update();

       if ( bouncers[i].rose() ) {
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

  
  
  
  //debouncer.rose()
   

}


/*
byte bukva_B[8]   = {B11110,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква "Б"
byte bukva_G[8]   = {B11111,B10001,B10000,B10000,B10000,B10000,B10000,B00000,}; // Буква "Г"
byte bukva_D[8]   = {B01111,B00101,B00101,B01001,B10001,B11111,B10001,B00000,}; // Буква "Д"
byte bukva_ZH[8]  = {B10101,B10101,B10101,B11111,B10101,B10101,B10101,B00000,}; // Буква "Ж"
byte bukva_Z[8]   = {B01110,B10001,B00001,B00010,B00001,B10001,B01110,B00000,}; // Буква "З"
byte bukva_I[8]   = {B10001,B10011,B10011,B10101,B11001,B11001,B10001,B00000,}; // Буква "И"
byte bukva_IY[8]  = {B01110,B00000,B10001,B10011,B10101,B11001,B10001,B00000,}; // Буква "Й"
byte bukva_L[8]   = {B00011,B00111,B00101,B00101,B01101,B01001,B11001,B00000,}; // Буква "Л"
byte bukva_P[8]   = {B11111,B10001,B10001,B10001,B10001,B10001,B10001,B00000,}; // Буква "П"
byte bukva_Y[8]   = {B10001,B10001,B10001,B01010,B00100,B01000,B10000,B00000,}; // Буква "У"
byte bukva_F[8]   = {B00100,B11111,B10101,B10101,B11111,B00100,B00100,B00000,}; // Буква "Ф"
byte bukva_TS[8]  = {B10010,B10010,B10010,B10010,B10010,B10010,B11111,B00001,}; // Буква "Ц"
byte bukva_CH[8]  = {B10001,B10001,B10001,B01111,B00001,B00001,B00001,B00000,}; // Буква "Ч"
byte bukva_Sh[8]  = {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00000,}; // Буква "Ш"
byte bukva_Shch[8]= {B10101,B10101,B10101,B10101,B10101,B10101,B11111,B00001,}; // Буква "Щ"
byte bukva_Mz[8]  = {B10000,B10000,B10000,B11110,B10001,B10001,B11110,B00000,}; // Буква "Ь"
byte bukva_IYI[8] = {B10001,B10001,B10001,B11001,B10101,B10101,B11001,B00000,}; // Буква "Ы"
byte bukva_Yu[8]  = {B10010,B10101,B10101,B11101,B10101,B10101,B10010,B00000,}; // Буква "Ю"
byte bukva_Ya[8]  = {B01111,B10001,B10001,B01111,B00101,B01001,B10001,B00000,}; // Буква "Я"
 
 

 */
