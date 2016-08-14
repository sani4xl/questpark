#include <IRremote.h>

#define JVC1 0xFB01
//#define JVC2 0xFB02

const int ledIRPin = 3; // ledIR на 9 порт в библиотеке закоменченно
const int buttonPin = 4;
const int soundPin = 7;
const int laserPin = 8;
const int ledPin =  13;

boolean isTime = false; 
unsigned long sc_start=0;
  
// переменные
int buttonState = 0;


IRsend irsend; 
//IRrecv irrecv(tIRPin);
//decode_results results;

void setup() {

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT);
  pinMode(soundPin, OUTPUT); 
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH); 
  irsend.enableIROut(38);
  //irrecv.enableIRIn(); // Start the receiver

}
 
void loop(){
//
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {   
    if (!isTime) { 
      sc_start = millis(); // замеряем время до начала подключения
      isTime = true; 
    }

    // =============================
    if ((millis()- sc_start > 400)) {
          Serial.println("-- Puf puf:avto_1");
          digitalWrite(soundPin, LOW); 
          digitalWrite(ledPin, LOW);
          digitalWrite(laserPin, LOW);
          delay(500);
         
          digitalWrite(soundPin, HIGH); 
          digitalWrite(ledPin, HIGH);
          digitalWrite(laserPin, HIGH);
          irsend.sendNEC(0x00F0111, 16);
          delay(300);

          isTime = false;

        } else {
          Serial.println("-- Piu piu");
          digitalWrite(soundPin, HIGH); 
          digitalWrite(ledPin, HIGH);
          digitalWrite(laserPin, HIGH);
          irsend.sendNEC(0x00F0111, 16);
        }
    // =============================   

  }
  else {    
    digitalWrite(soundPin, LOW); 
    digitalWrite(ledPin, LOW);
    digitalWrite(laserPin, LOW);
    isTime = false;
  
  }
  delay(99);
}

