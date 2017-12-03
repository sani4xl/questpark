
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

boolean mlocked = false;

// Init array that will store new NUID 
byte nuidPICC[4] = {0xEA, 0xD3, 0x24, 0x0A};

int relePin = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  pinMode (relePin, OUTPUT);
  digitalWrite(relePin, LOW);
  
  mlocked = false;
  setLocked(true);

}

void loop() {
  if ( rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      
    if (
      
      !(piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
         piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
         piccType != MFRC522::PICC_TYPE_MIFARE_4K)) {
      //Serial.println(F("Your tag is not of type MIFARE Classic."));
      //return;
     
   //     Serial.println(F("The NUID tag is:"));
    //Serial.print(F("In hex: "));
    //printHex(rfid.uid.uidByte, rfid.uid.size);
    /*Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
       //*/
      if(isUidMatched(rfid.uid.uidByte, nuidPICC)){
          Serial.println("hand applied");
          setLocked(false);
          //playPassword();
          
      }


      
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    }


    

    delay(50);
    
    
  }

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

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}



void setLocked(boolean locked){
 if(locked == mlocked){
  return;
 }
 
 
 if (locked){
   //digitalWrite(redPin, HIGH);
   //digitalWrite(greenPin, LOW);
   //servo_Motor.write(0);
   digitalWrite(relePin, LOW);
 }
 else{
  
  //Serial.println("unlocked");
  //servo_Motor.write(90);
  digitalWrite(relePin, HIGH);
  
  //digitalWrite(redPin, LOW);
  //digitalWrite(greenPin, HIGH);
  
  
  delay(100);
  
  makeSound();
  delay(100);
  makeSound();
  delay(100);
  makeSound();
  delay(100);
  makeSound();
  delay(100);
  
  
 }
 
 mlocked = locked;
}


void makeSound(){
}

