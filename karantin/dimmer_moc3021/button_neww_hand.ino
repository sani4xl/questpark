#include <LCD_1602_RUS.h>

LCD_1602_RUS lcd(0x27, 16, 2);


int melody[] = {  
  110, 131, 147,147,147, 

};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 4, 8, 5, 5, 4, 8, 5, 5, 4, 8, 4, 4, 4, 4
};

int melody2[] = {  
  4978, 1760, 2349, 2349 
// A2  C3       D3        E3    F3      G3
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations2[] = {
  10, 7, 11, 3
};

void playOpenSound() {
  Serial.println("playOpenSound");
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 15; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(9, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(9);
  }
}

void playSound() {
  Serial.println("Sound");
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 4; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration2 = 1000 / noteDurations2[thisNote];
    tone(9, melody2[thisNote], noteDuration2);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration2 * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(9);
  }
}

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define PIN2            7

#define NUMPIXELS      3


#define LEFTCL   3
#define RIGHTCL   4
const int leftHand  = 3;
const int rightHand = 4;
const int speakerPin = 9;


int password[] = { leftHand , leftHand , leftHand , rightHand, leftHand, rightHand};

int passwordLen = 6;
int key;
int position = 0;
int guessed_digit = 0;


boolean leftBtnPressed = false;
boolean rightBtnPressed = false;


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);
//Global Variables
const byte BUTTON=2; // our button pin
const byte BUTTON2=3; // our button pin
 
unsigned long buttonPushedMillis; // when button was released
unsigned long ledTurnedOnAt; // when led was turned on
unsigned long turnOnDelay = 30; // wait to turn on LED
unsigned long turnOffDelay = 300; // turn off LED after this time
bool ledReady = false; // flag for when button is let go
bool ledState = false; // for LED is on or not.
bool flag = 0;

unsigned long buttonPushedMillis2; // when button was released
unsigned long ledTurnedOnAt2; // when led was turned on

bool ledReady2 = false; // flag for when button is let go
bool ledState2 = false; // for LED is on or not.
bool flag2 = 0;
 
void setup() {
 Serial.begin(9600); 
 pinMode(BUTTON, INPUT);
 pinMode(BUTTON2, INPUT);
 // pinMode(speakerPin, OUTPUT);
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();

  //analogWrite (speakerPin, 255);

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();

  pixels2.begin(); // This initializes the NeoPixel library.
  pixels2.show();


}
 
void loop() {
 // get the time at the start of this loop()
 unsigned long currentMillis = millis(); 
 leftBtnPressed = false;
 rightBtnPressed = false;
  
 // check the button
 if (digitalRead(BUTTON) == HIGH && flag == 0) {
 Serial.println("btn-HIGH");
 key = rightHand;
 //playSound();
    rightBtnPressed = true;
  // update the time when button was pushed
  buttonPushedMillis = currentMillis;
  ledReady = true;
  flag = 1;
 }

if (digitalRead(BUTTON) == LOW && flag == 1){
   Serial.println("FLAG OFF");
   flag =0;
  }
// make sure this code isn't checked until after button has been let go
 if (ledReady) {

   //this is typical millis code here:
   if ((unsigned long)(currentMillis - buttonPushedMillis) >= turnOnDelay) {
    Serial.println(" "); 
    Serial.println("LED ON");
    Serial.println(" ");
     // okay, enough time has passed since the button was let go.
    colorWipe(pixels.Color(0, 150, 0), 50);
     // setup our next "state"
     ledState = true;
     // save when the LED turned on
     ledTurnedOnAt = currentMillis;
     // wait for next button press
     ledReady = false;
   }
 }
  
 // see if we are watching for the time to turn off LED
 if (ledState) {
  

   // okay, led on, check for now long
   if ((unsigned long)(currentMillis - ledTurnedOnAt) >= turnOffDelay) {
    colorWipe(pixels.Color(0, 0, 0), 50);

    Serial.println(" "); 
    Serial.println("LED OFF");
    Serial.println(" ");
     ledState = false;
 
   }
 }
   
 // ---------------- 2 -------------
  if (digitalRead(BUTTON2) == HIGH && flag2 == 0) {
    Serial.println("btn-HIGH TWO");
    key = leftHand;
    leftBtnPressed = true;
    buttonPushedMillis2 = currentMillis;
    ledReady2 = true;
    flag2 = 1;
 }

if (digitalRead(BUTTON2) == LOW && flag2 == 1){
   Serial.println("FLAG OFF TWO");
   flag2 =0;
  } 

if (ledReady2) {

   //this is typical millis code here:
   if ((unsigned long)(currentMillis - buttonPushedMillis2) >= turnOnDelay) {
    Serial.println(" "); 
    Serial.println("LED ON TWO");
    Serial.println(" ");
     // okay, enough time has passed since the button was let go.
    colorWipeTwo(pixels2.Color(0, 150, 0), 50);
     // setup our next "state"
     ledState2 = true;
     // save when the LED turned on
     ledTurnedOnAt2 = currentMillis;
     // wait for next button press
     ledReady2 = false;
   }
 }
  
 // see if we are watching for the time to turn off LED
 if (ledState2) {
  

   // okay, led on, check for now long
   if ((unsigned long)(currentMillis - ledTurnedOnAt2) >= turnOffDelay) {
    colorWipeTwo(pixels2.Color(0, 0, 0), 50);

    Serial.println(" "); 
    Serial.println("LED OFF TWO");
    Serial.println(" ");
     ledState2 = false;
 
   }
 }  
// -----------------------------------------------------------------------
    if((rightBtnPressed)or(leftBtnPressed)){
    //Serial.print("Oh");
      if(key){
        if(key == password[position]){
            position++;
        }else if(key == password[0]){
                position = 1;
              } else {
                position = 0;
              }
        Serial.print(position);
        Serial.print("");
        Serial.println(guessed_digit);
        if(position >= passwordLen){
             // playOpenSound();
             lcdee();
              position =0;
              guessed_digit = 0;
              }else{}
        } //
      }  //
    
//----------------------------------------------------------------------
}
//  void playOpenSound(){
//    Serial.print("Sound");
//   analogWrite (speakerPin, 0);
//   delay(50);
//   analogWrite (speakerPin, 255);
//   delay(50);
//   analogWrite (speakerPin, 0);
//   delay(50);
//   analogWrite (speakerPin, 255);
//   delay(50);
//   analogWrite (speakerPin, 0);
//   delay(50);
//   analogWrite (speakerPin, 255);
//   
//}


/// В-З-В-Ю-С-ЮВ-СЗ-ЮВ-СВ (ищи красный череп)
void lcdee(){
  lcd.setCursor(0, 0);
  lcd.print(L"В-З-В-Ю-С-ЮВ-СЗ-");
  lcd.setCursor(0, 1);
  lcd.print(L"ЮВ-СВ");
  }

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    //delay(wait);
  }
}
void colorWipeTwo(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels2.numPixels(); i++) {
    pixels2.setPixelColor(i, c);
    pixels2.show();
    //delay(wait);
  }  
}

