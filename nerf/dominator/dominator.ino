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
#define NUMPIXELS 32

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


#define CMD_NEXT 0X01
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

int currentTrack = 0;
SoftwareSerial mp3Serial(MP3_TX, MP3_RX);

#define DISPLAY_LENGTH 8
static int8_t Send_buf[DISPLAY_LENGTH] = {0} ;

/* 
 *  GENERAL SETUP
 */
#define GAME_RUNNING 1
#define GAME_STOPPED 0

#define GAME_MODE_NONE 0
#define GAME_MODE_DOMINATION 1
#define GAME_MODE_DEFUSE 2
#define GAME_MODE_ARTIFACT 3


#define SCORE_TIME_GAP 1 // time between ticks to add score
#define NON_ACTIVITY_TIME 5 // time before switch to neutral state

#define CHAR_LENGTH_PER_TEAM 3


int gameStatus = 0;
int gameMode = 0;

String gameIndicator = ""; // to display on screen

int currentSec;
int gameStartSec;
int lastScoreSec;
int lastActivitySec;


/*
 * DOMINATION SETUP
 */
 #define DOMINO_GAME_DURATION_SEC 30
 #define MAX_SCORES 10 //999
 
int teamRedScores = 0;
int teamGreenScores = 0;
int countingTeam = 0; // 1 for red, -1 for green


#define TEAM_RED 1
#define TEAM_GREEN -1
#define TEAM_NO 0

/**
 * DEFUSE SETUP
 */
 #define DEFUSE_GAME_DURATION_SEC 60
 bool bombPlanted = false;
 bool bombDefused = false;

 /**
  * ARTIFACT SETUP
  */

#define ARTIFACT_ACTIVITY_TIME 10 // time before deactivate artifact
#define ARTIFACT_GAME_DURATION_SEC 60
bool artifactActivated = false;  


/* 
 *  RFID CODES SETUP 
 */
byte *currentCode = NULL;

struct Code {int c1; int c2; int c3; int c4;};


Code redCodes[] = {
  {20, 75, 226, 43}
};

Code greenCodes[] = {
  {103, 155, 194, 95},
  {215, 3, 73, 98},
  {247, 254, 70, 99}
};

const Code dominoGameCode = {7, 216, 65, 99};
const Code defuseGameCode = {215, 76, 84, 98};
const Code ARTI_GAME_MODE_CODE = {23, 71, 62, 99};


const Code stopGameCode = {231, 105, 62, 99};
const Code startGameCode = {103, 191, 66, 99};

const Code BOMB_CODE = {135, 90, 83, 98};
const Code KIT_CODE = {7, 222, 53, 99};

const Code ARTIFACT_CODE = {7, 72, 54, 99};



/**
 * GAME LOGIC
 */

void setup() {
  serialinit();
  
  initDisplay();
  initRfid();
  initTimer();
  initLed();
  initMp3Player();
}

void loop() {
  calculateCurrentSec();
  readCode();
  selectGame();
  runGame();
}

void initMp3Player(){
  
  mp3Serial.begin(9600);
  delay(500);//Wait chip initialization is complete
  sendMp3Command(CMD_SEL_DEV, DEV_TF);//select the TF card  
  delay(200);//wait for 200ms
  //sendCommand(CMD_SET_VOLUME, 0X0F);//установить громкость в 15 (50%)
  sendMp3Command(CMD_SET_VOLUME, 0X1E);//установить громкость в x1E=30 (100%)
//  sendMp3Command(CMD_SET_VOLUME, 0X06);//установить громкость в x1E=30 (100%)
  delay(200);
  Serial.println(F("MP3 initialized..."));
  
  
  //sendCommand(CMD_SINGLE_CYCLE, SINGLE_CYCLE_OFF);//запустить зацикленное проигрывание содержимого папки "/01"

  delay(100);
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
  turnOffPixels();
}

void initRfid() {

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
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
    case GAME_MODE_DOMINATION:
      runDominatorGame();
      break;
    case GAME_MODE_DEFUSE:
      runDefuseGame();
      break;  
    case GAME_MODE_ARTIFACT:
      runArtifactGame();  
      break;  
    default:
      return;
  }

  displayGameIndicator();
}

void runArtifactGame() {
  int artifactSecLeft = ARTIFACT_GAME_DURATION_SEC - (currentSec - gameStartSec);
  checkForArtifactAction();
  checkForArtifactWinner(artifactSecLeft);
  checkForArtifactDeactivation();

  gameIndicator = formatTimer(artifactSecLeft);
}

void checkForArtifactAction() {
  if (currentCode == NULL) {
      return;
    }

    Serial.println("check for defuse action");

    if(isUuidMatch(currentCode, ARTIFACT_CODE)) {
      playArtefactActivatedTrack();
      artifactActivated = true;
      Serial.println("artifact activated");
      setPixelsYellow();
      lastActivitySec = currentSec;
    }

    if (checkRedMatch(currentCode) || checkGreenMatch(currentCode)) {
      
    } 

}

void checkForArtifactDeactivation() {
  if (!(artifactActivated && (currentSec - lastActivitySec > ARTIFACT_ACTIVITY_TIME))) {
    return;
  }

  Serial.println("deactivate artifact");
  
  artifactActivated = false;
  switchToNeutral();
}

void checkForArtifactWinner(int artifactSecLeft) {
  if (!( (artifactActivated && (countingTeam == TEAM_RED || countingTeam == TEAM_GREEN)) || artifactSecLeft <= 0)) {
    return;
  }
  
  if (artifactActivated && countingTeam == TEAM_RED) {
    redWon();
  } else if (artifactActivated && countingTeam == TEAM_GREEN) {
    greenWon();
  }
  
  stopGame();
}

void runDefuseGame() {
 int defuseSecLeft = DEFUSE_GAME_DURATION_SEC - (currentSec - gameStartSec);
 
 checkForDefuseAction();
 checkForDefuseWinner(defuseSecLeft);
 

 gameIndicator = formatTimer(defuseSecLeft);
}

void checkForDefuseWinner(int defuseSecLeft) {
  if (!(bombDefused || defuseSecLeft <= 0)) {
    return;
  }

 if (bombDefused) {
    swatWon();
  } if (bombPlanted) {
    terroristsWon();
  } else {
    swatWon();
  }

  stopGame();
}

void terroristsWon() {
  playTerroristWonTrack();
  blinkRed();
}

void swatWon() {
  playSwatWonTrack();
  blinkGreen();
}

void checkForDefuseAction() {
  if (currentCode == NULL) {
      return;
    }

    Serial.println("check for defuse action");

    if(isUuidMatch(currentCode, BOMB_CODE)) {
      playBombPlantedTrack();
      bombPlanted = true;
      setPixelsRed();
    }

    if(isUuidMatch(currentCode, KIT_CODE) && bombPlanted) {
      playBombDefusedTrack();
      bombDefused = true;
      setPixelsGreen();
    }
}

void switchToRed() {
  countingTeam = 1;
  setPixelsRed();
  playRedSwitchTrack();
  //playMusicWithTrackSwitch();
}

void switchToGreen() {
  countingTeam = -1;
  setPixelsGreen();
  playGreenSwitchTrack();
  //playMusicWithTrackSwitch();
}

void switchToNeutral() {
  countingTeam = TEAM_NO;
  setPixelsBlue();
}

void countTeams() {

  if (currentSec - lastScoreSec < SCORE_TIME_GAP) {
    return;
  }

  lastScoreSec = currentSec;
  
  if (countingTeam == TEAM_RED) { 
    teamRedScores += countingTeam;
  } else if (countingTeam == TEAM_GREEN) { 
    teamGreenScores -= countingTeam;
  }
  
  if (teamRedScores > MAX_SCORES) {
    teamRedScores = MAX_SCORES;
  }
  
  if (teamGreenScores > MAX_SCORES) {
    teamGreenScores = MAX_SCORES;
  }
}

void startGameWithCode(int newGameMode) {

  gameMode = newGameMode;
  startGame();
}

void startGame() {
  playAttentionStartTrack();
  calculateCurrentSec();
 
  gameStartSec = currentSec;
  
  //playMusic();
  
  gameStatus = GAME_RUNNING;
  setPixelsBlue();
}

void stopGame() {
  Serial.println(F("stopping game..."));
  gameStatus = GAME_STOPPED;
}

void resetGame() {
  gameMode = GAME_MODE_NONE;
  gameStatus = GAME_STOPPED;
  turnOffPixels();
  displayNothing();
}

void runDominatorGame() {
  
  countTeams();
  gameIndicator = formatScores(teamRedScores) + "  " + formatScores(teamGreenScores); // delimiter 2 spaces
  
  checkForSwitch();
  checkForNeutralMatch();
  checkForDominationWinner();
}

void readCode() {
  currentCode = NULL;
  
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

    currentCode = rfid.uid.uidByte;

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    
    delay(50);
}

void selectGame() {
  if (currentCode == NULL) {
      return;
  }

  Serial.println("selecting game...");
  if(isUuidMatch(currentCode, dominoGameCode)) {
    Serial.println("Domination mode selected");
    playDominationModeTrack();
    resetDominationMode();
    startGameWithCode(GAME_MODE_DOMINATION);
  }

  if(isUuidMatch(currentCode, defuseGameCode)) {
    Serial.println("Defuse mode selected");
    playDefuseModeTrack();
    resetDefuseMode();
    startGameWithCode(GAME_MODE_DEFUSE);
  }

  if(isUuidMatch(currentCode, ARTI_GAME_MODE_CODE)) {
    Serial.println("Artifact mode selected");
    playArtifactModeTrack();
    resetArtifactMode();
    startGameWithCode(GAME_MODE_ARTIFACT);
  }


  if(isUuidMatch(currentCode, stopGameCode)) {
    Serial.println("stop game");
    stopGame();
  }

  if(isUuidMatch(currentCode, startGameCode)) {
    Serial.println("restart game");
    resetDominationMode();
    resetDefuseMode();
    resetArtifactMode();
    startGame();
  }
  
    
}

void resetArtifactMode() {
  switchToNeutral();
}

void resetDefuseMode() {
  bombPlanted = false;
  bombDefused = false;
}

void resetDominationMode() {
  teamRedScores =  0;
  teamGreenScores = 0;
  switchToNeutral();
}

void checkForSwitch() {
    if (currentCode == NULL) {
      return;
    }

    Serial.println("check for team switch");

    countingTeam = TEAM_NO;
      
    if (checkRedMatch(currentCode) || checkGreenMatch(currentCode)) {
      lastActivitySec = currentSec;
    } 
    
}

void checkForNeutralMatch() {
  if (countingTeam == TEAM_NO || currentSec - lastActivitySec < NON_ACTIVITY_TIME) {
    return;
  }

  Serial.println("goes neutral");
  
  switchToNeutral();
}

bool checkRedMatch(byte *uidByte) {
//  Serial.println("red check");
  if (!isUuidMatchList(uidByte, redCodes, sizeof(redCodes) / sizeof(redCodes[0]))) {
    return false;
  }

  Serial.println("red match");

  switchToRed();
  return true;
}

bool checkGreenMatch(byte *uidByte) {
//  Serial.println("green check");
  if (!isUuidMatchList(uidByte, greenCodes, sizeof(greenCodes) / sizeof(greenCodes[0]) )) {
    return false;
  }

  Serial.println("green match");

  switchToGreen();
  return true;
}

bool isUuidMatchList(byte *uidByte, Code *codes, int listSize) {
//  Serial.println("list size");
//  Serial.println(listSize);
  bool match = false;
  for (int i = 0; i < listSize; i++) {
    if(isUuidMatch(uidByte, codes[i])){
        match = true;
        break;
    }
  }

  return match;
}



bool isUuidMatch(byte *uidByte, Code code) {   
  return
    int(uidByte[0]) == code.c1 &&
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
  tm.displayText("        ");
}

String formatScores(int scores) {
  String str = String(scores);
  int spaces = CHAR_LENGTH_PER_TEAM - str.length();
  for(int i = 0; i < spaces; i++) {
    str = " " + str;
  }

  return str;
}

String formatTimer(int scores) {
  String str = String(scores);
  int spaces = DISPLAY_LENGTH - str.length();
  for(int i = 0; i < spaces; i++) {
    str = " " + str;
  }

  return str;
}

void checkForDominationWinner() {
  
  if (!(teamRedScores >= MAX_SCORES || teamGreenScores >= MAX_SCORES ||  (currentSec - gameStartSec >= DOMINO_GAME_DURATION_SEC) )) {
//  if (!(teamRedScores >= MAX_SCORES || teamGreenScores >= MAX_SCORES)) {
    return;
  }

  gameStatus = GAME_STOPPED;

  if (teamRedScores == teamGreenScores) {
    // draw; // chose the last one
  } else if (teamRedScores > teamGreenScores) {
    redWon();
  } else {
    greenWon();
  }

  stopGame();
}

void redWon() {
  playRedWonTrack();
  blinkRed();
}

void greenWon() {
  playGreenWonTrack();
  blinkGreen();
}


void blinkRed() {
  for (int i = 0; i < 10; i++) {
    turnOffPixels();
    delay(500);
    setPixelsRed();
    delay(500);
  }
  setPixelsWhite();
}

void blinkGreen() {
  for (int i = 0; i < 10; i++) {
    turnOffPixels();
    delay(500);
    setPixelsGreen();
    delay(500);
  }
  setPixelsWhite();
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

void setPixelsYellow() {
  setPixelsColor(pixels.Color(255, 255, 0));
}

void setPixelsWhite() {
  setPixelsColor(pixels.Color(255, 255, 255));
}

void setPixelsColor(uint32_t color){
   pixels.clear();
   for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, color); // Moderately bright green color.
   }  
   pixels.show();
}
//
//void playMusic() {
//  sendMp3Command(CMD_PLAY_FOLDER, 0X0101); //запустить зацикленное проигрывание содержимого папки "/01"
//  
//}

//void playMusicWithTrackSwitch() {
//  playMusic();
//  currentTrack++;
//  for (int i = 0; i < currentTrack; i++) {
//    sendMp3Command(CMD_NEXT, 0x0000);
//  }
//}

void playArtefactActivatedTrack() {
  int songCode = word(0x02, 1);
  sendMp3Command(0X0F, songCode);
}

void playBombDefusedTrack() {
  int songCode = word(0x02, 2);
  sendMp3Command(0X0F, songCode);
  delay(5000);
}

void playBombPlantedTrack() {
  int songCode = word(0x02, 3);
  sendMp3Command(0X0F, songCode);
}


void playGreenSwitchTrack() {
  int songCode = word(0x02, 4);
  sendMp3Command(0X0F, songCode);
}

void playRedSwitchTrack() {
  int songCode = word(0x02, 5);
  sendMp3Command(0X0F, songCode);
}

void playNeutralityModeTrack() {
  int songCode = word(0x02, 6);
  sendMp3Command(0X0F, songCode);
  delay(5000);
}

void playGreenWonTrack() {
  int songCode = word(0x02, 7);
  sendMp3Command(0X0F, songCode);
  delay(5000);
}

void playRedWonTrack() {
  int songCode = word(0x02, 8);
  sendMp3Command(0X0F, songCode);
  delay(5000);
}


void playArtifactModeTrack() {
  int songCode = word(0x02, 9);
  sendMp3Command(0X0F, songCode);
  delay(6000);
}

void playDominationModeTrack() {
  int songCode = word(0x02, 10);
  sendMp3Command(0X0F, songCode);
  delay(5000);
}

void playDefuseModeTrack() {
  int songCode = word(0x02, 11);
  sendMp3Command(0X0F, songCode);
  delay(5000);
}

void playAttentionStartTrack() {
  int songCode = word(0x02, 12);
  sendMp3Command(0X0F, songCode);
  delay(5000);
}


void playTerroristWonTrack() {
  int songCode = word(0x02, 7);
  sendMp3Command(0X0F, songCode);
}

void playSwatWonTrack() {
  int songCode = word(0x02, 8);
  sendMp3Command(0X0F, songCode);
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
