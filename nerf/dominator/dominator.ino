#include <TM1638plus_Model2.h>
#include <TM1638plus_common.h>
#include <TM1638plus.h>

#include <MFRC522.h>
#include <SPI.h>


// LED
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define LED_PIN   6
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// RFID
/*
 * * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

const int RST_PIN = 9;
const int SS_PIN = 10;

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 


const int TM_STB = 4; 
const int TM_CLK = 3; 
const int TM_DIO = 2; 

bool high_freq = false; //default false,, If using a high freq CPU > ~100 MHZ set to true. 

//Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(TM_STB, TM_CLK , TM_DIO, high_freq);

int gameMode = 1;

const int DOMINATION_MODE = 1;
const int BOMB_DEFUSE_MODE = 2;

const int MAX_SCORES = 999;
const int CHAR_LENGTH_PER_TEAM = 3;

int teamRedScores = 0;
int teamGreenScores = 0;
int countingTeam = 0; // 1 for red, -1 for green

int gameStatus = 1;

const int GAME_RUNNING = 1;
const int GAME_STOPPED = 0;

String gameIndicator = "";

int currentSec;
int lastScoreSec;
const int SCORE_TIME_GAP = 1; // time between ticks to add score

struct Code {int c1; int c2; int c3; int c4;};

Code redCodes[] = {
  {20, 75, 226, 43}
};

Code greenCodes[] = {
  {103, 155, 194, 95}
};


void setup() {
  serialinit();
  
  initDisplay();
  initRfid();
  initTimer();
  initLed();
  
  //switchToRed(); // todo: remove
}

void initLed() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();
}

void initTimer() {
  currentSec = millis() / 1000;
  lastScoreSec = millis() / 1000;
}

void initDisplay() {
  tm.displayBegin();
  tm.brightness(8);
  displayNothing();
}

void initRfid() {

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  calculateCurrentSec();
  runGame();

  pixels.clear();

  for(int i=0; i<NUMPIXELS; i++) {

    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    //delay(DELAYVAL);
  }

}

void calculateCurrentSec() {
  currentSec = millis() / 1000;
}

void runGame() {

  if (gameStatus != GAME_RUNNING) {
    return;
  }
  
  switch (gameMode) {
    case DOMINATION_MODE:
      runDominatorGame();
      break;
  }

  displayGameIndicator();
  
}

void switchToRed() {
  countingTeam = 1;
  setPixelsRed();
}

void switchToGreen() {
  countingTeam = -1;
  setPixelsGreen();
}

void switchToNeutral() {
  countingTeam = 0;
}

void countTeams() {

  if (currentSec - lastScoreSec < SCORE_TIME_GAP) {
    return;
  }

  lastScoreSec = currentSec;
  
  if (countingTeam > 0) { 
    teamRedScores+= 1;
  } else if (countingTeam < 0) { 
    teamGreenScores+= 1;
  }
  
  if (teamRedScores > MAX_SCORES) {
    teamRedScores = MAX_SCORES;
  }
  
  
  if (teamGreenScores > MAX_SCORES) {
    teamGreenScores = MAX_SCORES;
  }
}

void runDominatorGame() {
  
  countTeams();
  gameIndicator = formatScores(teamRedScores) + "  " + formatScores(teamGreenScores); // delimiter 2 spaces
  
  checkForSwitch();
  checkForWinner();
}


void checkForSwitch() {

  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  //Serial.println(F("tag present..."));

  rfid.PICC_ReadCardSerial();
  

  Serial.println(F("reading tag..."));
  
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  Serial.println(rfid.PICC_GetTypeName(piccType));
      
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }
         
      //Serial.println(F("Your tag is not of type MIFARE Classic."));
      
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
       
    checkRedMatch(rfid.uid.uidByte);
    checkGreenMatch(rfid.uid.uidByte);


      
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    
    delay(50);
  
}

void checkRedMatch(byte *uidByte) {
  if (!isUuidMatchList(uidByte, redCodes)) {
    return;
  }

  Serial.println("red match");

  switchToRed();
}

void checkGreenMatch(byte *uidByte) {
  if (!isUuidMatchList(uidByte, greenCodes)) {
    return;
  }

  Serial.println("green match");

  switchToGreen();
}

bool isUuidMatchList(byte *uidByte, Code *codes) {
  bool match = false;
  for (int i = 0; i < 1; i++) {
    if(isUuidMatch(uidByte, codes[i])){
        match = true;
        break;
    }
  }

  return match;
}

bool isUuidMatch(byte *uidByte, Code code) {
  return
    int(uidByte[0]) == int(code.c1) &&
    int(uidByte[1]) == code.c2 &&
    int(uidByte[2]) == code.c3 &&
    int(uidByte[3]) == code.c4;
}

void displayGameIndicator() {
  int str_len = gameIndicator.length() + 1;
  char char_array[str_len];
  gameIndicator.toCharArray(char_array, str_len);
  tm.displayText(char_array);
}

void displayNothing() {
  tm.displayText("________");
}

String formatScores(int scores) {
  String str = String(scores);
  int spaces = CHAR_LENGTH_PER_TEAM - str.length();
  for(int i = 0; i < spaces; i++) {
    str = " " + str;
  }

  return str;
}

void checkForWinner() {
  if (!(teamRedScores >= MAX_SCORES || teamGreenScores >= MAX_SCORES)) { // || outof time
    return;
  }

  gameStatus = GAME_STOPPED;

  if (teamRedScores == teamGreenScores) {
    // draw;
  } else if (teamRedScores > teamGreenScores) {
    // red wins;
  } else {
    // green wins
  }

  blinkGameIndicator();

}

void blinkGameIndicator() {
  for (int i = 0; i < 10; i++) {
    displayNothing();
    delay(500);
    displayGameIndicator();
    delay(500);
  }
}


void serialinit()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("--starting dominator--");
}


/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}


void turnOffPixels(){
   for(int i=0; i < NUMPIXELS; i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
   }  
   pixels.show();

}

void setPixelsGreen() {
  setPixelsColor(pixels.Color(0,255,0));
}

void setPixelsRed() {
  setPixelsColor(pixels.Color(255,0,0));
}

void setPixelsColor(uint32_t color){
   pixels.clear();
   for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, color); // Moderately bright green color.
   }  
   pixels.show();
}
