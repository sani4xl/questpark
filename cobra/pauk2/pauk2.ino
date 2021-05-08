/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read new NUID from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
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

#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4] = {0xFA, 0xF5, 0x10, 0x0A};

// MP3
static int8_t Send_buf[8] = {0} ;

const int timeBetweenTracks = 90; // in secs

const int totalTracks = 31;
int currentTrack = 1;

#define CMD_PLAY_W_INDEX 0X03
#define CMD_SET_VOLUME 0X06
#define CMD_SEL_DEV 0X09
  #define DEV_TF 0X02
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_STOP 0X16
#define CMD_SINGLE_CYCLE 0X19
  #define SINGLE_CYCLE_ON 0X00
  #define SINGLE_CYCLE_OFF 0X01
#define CMD_PLAY_W_VOL 0X22

#include <SoftwareSerial.h>
SoftwareSerial mp3Serial(3, 4); // RX, TX
int lastTimePlay; // in sec
int currentSec;

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  initMp3Player();

  /*

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  */
  Serial.println("start terrorist");
}
 
void loop() {
  currentSec = millis() / 1000;
  checkForNextTrack();


  // Verify if the NUID has been readed
  if ( rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

    Serial.println("reading rfid");
//    Serial.println("reading rfid");
      
    if (
      
      !(piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
         piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
         piccType != MFRC522::PICC_TYPE_MIFARE_4K)) {
      //Serial.println(F("Your tag is not of type MIFARE Classic."));
      //return;
      //*
        Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
       //*/
      if(isUidMatched(rfid.uid.uidByte, nuidPICC)){
          Serial.println("spider applied");
          playPassword();
          
      }


      
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }


    

    delay(50);
    
    
  }


}

boolean isUidMatched(byte *uidByte, byte *nuidPICC){
  Serial.println(uidByte[0] != nuidPICC[0]);
  Serial.println(uidByte[1] != nuidPICC[1]);
  Serial.println(uidByte[2] != nuidPICC[2]); 
  Serial.println(uidByte[3] != nuidPICC[3]);

  return true;
    
  if (uidByte[0] != nuidPICC[0] || 
    uidByte[1] != nuidPICC[1]
    
    //uidByte[2] != nuidPICC[2] || 
    //uidByte[3] != nuidPICC[3] 
    ) {
       Serial.println("no match");
       return false;
    }

    Serial.println("match");

    return true;
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


void initMp3Player(){
  lastTimePlay = millis() / 1000;
  currentSec = millis() / 1000;
  //pinMode(songs, INPUT_PULLUP);
  //pinMode(alarm, INPUT);
  //Serial1.begin(9600);
  mp3Serial.begin(9600);
  delay(500);//Wait chip initialization is complete
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
  delay(200);//wait for 200ms
  //sendCommand(CMD_SET_VOLUME, 0X0F);//установить громкость в 15 (50%)
  sendCommand(CMD_SET_VOLUME, 0X16);//установить громкость в x1E=30 (100%)

  
  //sendCommand(0X17, 0X0102);//запустить зацикленное проигрывание содержимого папки "/01"
  //sendCommand(CMD_SINGLE_CYCLE, SINGLE_CYCLE_OFF);//запустить зацикленное проигрывание содержимого папки "/01"
  int songCode = word(0x01,1);
  sendCommand(0X0F, songCode);// играем трек 001 из папки 01
}

void checkForNextTrack(){
  
  if( currentSec - lastTimePlay > timeBetweenTracks){
    lastTimePlay = currentSec;
    currentTrack++;
    if(currentTrack > totalTracks){//totalTracks){
      currentTrack = 1;
    }

    int songCode = word(0x01, currentTrack);
    sendCommand(0X0F, songCode);// играем трек 001 из папки 01
    
  }
}

int passwordPlayingTime = 0;
void playPassword(){
  //if(passwordIsPlaying)
  if(currentSec - passwordPlayingTime < 0){
    return;
  }
  Serial.println("play password");
  // stop current track
  sendCommand(CMD_STOP, 0x00);//select the TF card  
  // playing password track
  int songCode = word(0x02, 0x01);
  sendCommand(0X0F, songCode);// играем трек 001 из папки 02
  passwordPlayingTime = currentSec + 20;
  lastTimePlay = currentSec + 30;
  
}

void sendCommand(int8_t command, int16_t dat)
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
