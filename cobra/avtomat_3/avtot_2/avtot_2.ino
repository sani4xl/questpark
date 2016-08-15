#include <IRremote.h>

#define JVC1 0xFB01
//#define JVC2 0xFB02

const int ledIRPin = 3;
const int buttonPin = 2;
const int soundPin = 4;
const int laserPin = 6;
const int ledPin =  13;
const int motorPin = 7;  
// переменные
int buttonState = 0;

int numTones = 10;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//            mid C  C#   D    D#   E    F    F#   G    G#   A


IRsend irsend; 
//IRrecv irrecv(tIRPin);
//decode_results results;

void setup() {

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);     
  pinMode(buttonPin, INPUT);
  pinMode(soundPin, OUTPUT); 
  pinMode(laserPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
  digitalWrite(laserPin, HIGH); 
  //irsend.enableIROut(38);
  //irrecv.enableIRIn(); // Start the receiver

}
 
void loop(){
//
//    Serial.println("F");
  
//
  buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {   
    Serial.println("Shoot");
    digitalWrite(soundPin, HIGH); 
    digitalWrite(ledPin, HIGH);
    digitalWrite(laserPin, HIGH);
    digitalWrite(motorPin, HIGH);
    irsend.sendNEC(0x00F0222, 16);
    delay(90); 
    //    irsend.sendSony(JVC2, 16);
    //    delay(50);
    
    Serial.print("- Music -");
        // Music On
 //       for (int i = 0; i < numTones; i++)
 //         {
 //           tone(soundPin, tones[i]);
//            delay(500);
//          }
//        noTone(soundPin);
        // Music Off
        noise();noTone(soundPin);
  }
  else {    
    digitalWrite(soundPin, LOW); 
    digitalWrite(ledPin, LOW);
    digitalWrite(laserPin, LOW);
    digitalWrite(motorPin, LOW);
  
  }
  //delay(1500);
}

void noise()  {

  unsigned long time = millis();

  while(millis() - time <= 500)  {  // change "500" for different durations in ms.
    tone(soundPin, random(100, 1000));   // change the parameters of random() for different sound
  }
}
