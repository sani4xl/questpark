#include <TM1638plus_Model2.h>
#include <TM1638plus_common.h>
#include <TM1638plus.h>

#include <MFRC522.h>
#include <SPI.h>

#include <SoftwareSerial.h> // mp3

// LED
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define LED_PIN   8 // LED strip DI
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

#define RST_PIN 9
#define SS_PIN 10

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 


#define TM_STB 4 
#define TM_CLK 3 
#define TM_DIO 2 

bool high_freq = false; //default false,, If using a high freq CPU > ~100 MHZ set to true. 

//Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(TM_STB, TM_CLK , TM_DIO, high_freq);


/** 
 * MP3  
 * 
 * Make sure your micro sd card is formatted as FAT16 or FAT32
 * 
 * All music tracks in folder /01
 * /01/001xxx.mp3
 * /01/002xxx.mp3
 */


#define CMD_PLAY_W_INDEX 0X03
#define CMD_SET_VOLUME 0X06
#define CMD_SEL_DEV 0X09
  #define DEV_TF 0X02
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_STOP 0X16
#define CMD_PLAY_FOLDER 0X17
#define CMD_SINGLE_CYCLE 0X19
  #define SINGLE_CYCLE_ON 0X00
  #define SINGLE_CYCLE_OFF 0X01
#define CMD_PLAY_W_VOL 0X22

#define MP3_RX 5
#define MP3_TX 6


SoftwareSerial mp3Serial(MP3_TX, MP3_RX);

static int8_t Send_buf[8] = {0} ;

// game setup

int gameMode = 1;

#define DOMINATION_MODE 1
#define BOMB_DEFUSE_MODE 2

#define MAX_SCORES 999
#define CHAR_LENGTH_PER_TEAM 3

int teamRedScores = 0;
int teamGreenScores = 0;
int countingTeam = 0; // 1 for red, -1 for green

int gameStatus = 1;

#define GAME_RUNNING 1
#define GAME_STOPPED 0

String gameIndicator = "";

int currentSec;
int lastScoreSec;
int lastActivitySec;
#define SCORE_TIME_GAP 1 // time between ticks to add score
#define NON_ACTIVITY_TIME 5 // time before switch to neutral state

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
  initMp3Player();

  switchToNeutral(); // for domination
}

void initMp3Player(){
  
  mp3Serial.begin(9600);
  delay(500);//Wait chip initialization is complete
  sendMp3Command(CMD_SEL_DEV, DEV_TF);//select the TF card  
  delay(200);//wait for 200ms
  //sendCommand(CMD_SET_VOLUME, 0X0F);//установить громкость в 15 (50%)
//  sendMp3Command(CMD_SET_VOLUME, 0X16);//установить громкость в x1E=30 (100%)
  sendMp3Command(CMD_SET_VOLUME, 0X06);//установить громкость в x1E=30 (100%)
  delay(200);
  Serial.println(F("MP3 initialized..."));
  
  
  //sendCommand(CMD_SINGLE_CYCLE, SINGLE_CYCLE_OFF);//запустить зацикленное проигрывание содержимого папки "/01"
  //int songCode = word(0x01, 1);
  //sendCommand(0X0F, songCode);// играем трек 001 из папки 01
  playMusic();
//  int songCode = word(0x02, 1);
//  sendMp3Command(0X0F, songCode);
  delay(100);
}

void playMusic() {
  sendMp3Command(CMD_PLAY_FOLDER, 0X0102); //запустить зацикленное проигрывание содержимого папки "/01"
}


void initLed() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();

  turnOffPixels();
}

void initTimer() {
  currentSec = millis() / 1000;
  lastScoreSec = millis() / 1000;
}

void initDisplay() {
  tm.displayBegin();
  tm.brightness(8);
  setPixelsBlue();
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

//  pixels.clear();

//  for(int i=0; i<NUMPIXELS; i++) {
//
//    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
//    pixels.show();
//    //delay(DELAYVAL);
//  }

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

void playRedSwitchTrack() {
  int songCode = word(0x02, 1);
  sendMp3Command(0X0F, songCode);
}

void playGreenSwitchTrack() {
  int songCode = word(0x02, 1);
  sendMp3Command(0X0F, songCode);
}

void switchToRed() {
  countingTeam = 1;
  setPixelsRed();
  playRedSwitchTrack();
}

void switchToGreen() {
  countingTeam = -1;
  setPixelsGreen();
  playGreenSwitchTrack();
}

void switchToNeutral() {
  countingTeam = 0;
  setPixelsBlue();
}

void countTeams() {

  if (currentSec - lastScoreSec < SCORE_TIME_GAP) {
    return;
  }

  lastScoreSec = currentSec;
  
  if (countingTeam > 0) { 
    teamRedScores += countingTeam;
  } else if (countingTeam < 0) { 
    teamGreenScores -= countingTeam;
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
  checkForNeutralMatch();
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
       
    if (checkRedMatch(rfid.uid.uidByte) || checkGreenMatch(rfid.uid.uidByte)) {
      lastActivitySec = currentSec;
    } 
      
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    
    delay(50);
}

void checkForNeutralMatch() {
  if (countingTeam == 0 || currentSec - lastActivitySec < NON_ACTIVITY_TIME) {
    return;
  }

  Serial.println("goes neutral");
  
  switchToNeutral();
}

bool checkRedMatch(byte *uidByte) {
  if (!isUuidMatchList(uidByte, redCodes)) {
    return false;
  }

  Serial.println("red match");

  switchToRed();
  return true;
}

bool checkGreenMatch(byte *uidByte) {
  if (!isUuidMatchList(uidByte, greenCodes)) {
    return false;
  }

  Serial.println("green match");

  switchToGreen();
  return true;
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
  pixels.clear();
   for(int i=0; i < NUMPIXELS; i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
   }  
   pixels.show();

}

void setPixelsGreen() {
  setPixelsColor(pixels.Color(0, 255, 0));
}

void setPixelsBlue() {
  setPixelsColor(pixels.Color(0, 0, 255));
}

void setPixelsRed() {
  setPixelsColor(pixels.Color(255, 0, 0));
}

void setPixelsColor(uint32_t color){
   pixels.clear();
   for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, color); // Moderately bright green color.
   }  
   pixels.show();
}

void sendMp3Command(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for(uint8_t i=0; i<8; i++)//
  {
    mp3Serial.write(Send_buf[i]) ;
  }
}
