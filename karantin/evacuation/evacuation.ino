//http://www.instructables.com/id/Interfacing-RFID-RC522-With-Arduino-MEGA-a-Simple-/
#include <Bounce2.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

// audio stuff
static int8_t Send_buf[8] = {0} ;
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
SoftwareSerial mp3Serial(3, 4); // RX, TX

const int buttonPin = 2;
Bounce buttonBounce = Bounce(); 

const int lightRelePin = 31;
const int lockRelePin = 33;

constexpr uint8_t RST_PIN = 8;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 9;     // Configurable, see typical pin layout above
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

boolean mlocked = false;

boolean accessGranted = false;

const int delayBeforeFinalMonsters = 500;
const int delayBeforeRoofUnlock = 5000;

// Init array that will store new NUID 
byte nuidPICC[4] = {0xF0, 0xD3, 0xEC, 0xD5};
byte nuidPICC2[4] = {0x7C, 0x75, 0x2A, 0x2B};


void setup() {
   Serial.begin(9600);
  // put your setup code here, to run once:
   pinMode(buttonPin, INPUT);
   buttonBounce.attach(buttonPin);
   buttonBounce.interval(10); 
   Serial.println("start program");

   pinMode(lightRelePin, OUTPUT);
   pinMode(lockRelePin, OUTPUT);

   turnLightOn();
   lockRoof();
   
   initMp3Player();

   SPI.begin(); // Init SPI bus
   rfid.PCD_Init(); // Init MFRC522 
  
   for (byte i = 0; i < 6; i++) {
     key.keyByte[i] = 0xFF;
   }
   
}

void turnLightOn(){
  digitalWrite(lightRelePin, LOW);
}

void turnLightOff(){
  digitalWrite(lightRelePin, HIGH);
}

void lockRoof(){
  digitalWrite(lockRelePin, LOW);  
}

void unlockRoof(){
  digitalWrite(lockRelePin, HIGH);  
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonBounce.update();
  if(buttonBounce.rose()){
    Serial.println("start evacuation button pressed");
    if(!accessGranted){
      playAccessRequest();
      
    }
    else{
      playEvacuation();
      delay(delayBeforeRoofUnlock);
      unlockRoof();
      delay(delayBeforeFinalMonsters);
      turnLightOff();
      
    }
  }

  readRfid();

}

void readRfid(){

   if ( rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      
    if (
      
      !(piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
         piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
         piccType != MFRC522::PICC_TYPE_MIFARE_4K)) {
      //Serial.println(F("Your tag is not of type MIFARE Classic."));
      //return;
     
     //   Serial.println(F("The NUID tag is:"));
    //Serial.print(F("In hex: "));
    //printHex(rfid.uid.uidByte, rfid.uid.size);
    /*Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
       //*/
      if(
        isUidMatched(rfid.uid.uidByte, nuidPICC)
        ||
        isUidMatched(rfid.uid.uidByte, nuidPICC2)
        
        ){
          Serial.println("access applied");
          //setLocked(false);
          //playPassword();
          accessGranted = true;
          playAccessGranted();
          
          
      }


      
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }


    

    delay(50);
    
    
  }
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

void stopMp3(){
  sendMp3Command(CMD_STOP, 0x00);//select the TF card  
}

void initMp3Player(){
  //lastTimePlay = millis() / 1000;
  //currentSec = millis() / 1000;
  //pinMode(songs, INPUT_PULLUP);
  //pinMode(alarm, INPUT);
  //Serial1.begin(9600);
  mp3Serial.begin(9600);
  delay(500);//Wait chip initialization is complete
  sendMp3Command(CMD_SEL_DEV, DEV_TF);//select the TF card  
  delay(200);//wait for 200ms
  //sendCommand(CMD_SET_VOLUME, 0X0F);//установить громкость в 15 (50%)
  sendMp3Command(CMD_SET_VOLUME, 0X1E);//установить громкость в x1E=30 (100%)

}


void playAccessRequest(){
  
  sendMp3Command(0X0F, 0X0101);
  delay(1500);
}

void playAccessGranted(){
  
  sendMp3Command(0X0F, 0X0103 );// играем трек 001 из папки 01
  delay(2000);
  
}

void playEvacuation(){
 
  sendMp3Command(0X0F, 0X0102);// играем трек 001 из папки 01
 
}




boolean isUidMatched(byte *uidByte, byte *nuidPICC){
  if (uidByte[0] != nuidPICC[0] || 
    uidByte[1] != nuidPICC[1] || 
    uidByte[2] != nuidPICC[2] || 
    uidByte[3] != nuidPICC[3] ) {
       return false;
    }

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
