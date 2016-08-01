// --  redBtn10s TerroristBox
// -- modify 05/07/2016 manjulife

#include <Wire.h>
 
const int relayStartPin = A1;

const int ledPin = 13;    // статус готовности бокса
const int piezoPin = 3;   //

const int buttonPin = 2;   // START btn
int buttonState = 0;

const int restartBtnPin = 4;   // RESTART btn
int buttonRestartState = 0;
  

byte x= 77;
boolean isFinishPress = false;
boolean isPress = false;
boolean isFinishSound = false;
boolean isTime = false; 
boolean isRelayRestart = false; 
boolean isFinishLed = false; 

unsigned long sc_start=0;
unsigned long soundON=0;
unsigned long sc_start_relay = 0;
unsigned long sc_led = 0;
int countLed = 0;

int ledPanel[] = {5, 6, 7, 8, 9, 10, 11, 12};

int melody[] = {
  262, 196, 196, 220, 196, 0, 247, 262
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
//

int numTones = 10;
int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//            mid C  C#   D    D#   E    F    F#   G    G#   A
//

void setup() {
    Wire.begin();
    Serial.begin(9600);
    
    pinMode(restartBtnPin, INPUT);
    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
    
    //int i;
    for (int i = 0; i < 8; i++) {
    pinMode(ledPanel[i], OUTPUT);
    }
    
    pinMode(relayStartPin, OUTPUT);
    digitalWrite(relayStartPin, LOW);
    
    sc_led = millis();
    countLed = 0;
}

void offLights() {
  for (int i = 0; i < 8; i++) {
     digitalWrite(ledPanel[i], LOW);
   }
}

void onLights() {
  for (int i = 0; i < 8; i++) {
     digitalWrite(ledPanel[i], HIGH);
   }
}


void circleLights() {
  Serial.print("L");
  if ( (millis()- sc_led > 100) ) {
    if (countLed == 8) { digitalWrite(ledPanel[7], LOW); countLed = 0;}
    
    digitalWrite(ledPanel[countLed], HIGH);
    digitalWrite(ledPanel[countLed - 1], LOW);
    countLed ++; 
    sc_led = millis();
   }    
}

  
void loop() {
  
    
    buttonRestartState = digitalRead(restartBtnPin);
    if ((buttonRestartState == HIGH)&&(isFinishPress == true)) {
      Serial.println("--- RESTART ---");
      digitalWrite(ledPin, HIGH);
      isFinishPress = false;
      isFinishLed = false;
     // if (isRelayRestart) {
     //   digitalWrite(relayStartPin, HIGH);
     //   delay(1000);
     //   digitalWrite(relayStartPin, LOW);
     // }  
    }
  
  
    buttonState = digitalRead(buttonPin);
    
    // Кнопка нажата и не прошло 10 сек.
    if ((buttonState == HIGH)&&(isPress == false)&&(isFinishPress == false)) {
        //
        if (!isTime) { 
          sc_start = millis(); // замеряем время до начала подключения
           isTime = true; 
        }
        
        // ========================== 0.2 SEC 
        if ((millis()- sc_start > 50)&&(millis()- sc_start < 2000)) {
          Serial.print("-S0-");
          digitalWrite(ledPanel[0], HIGH);
          digitalWrite(ledPanel[6], HIGH);      
          if ((millis()- sc_start < 600)) {tone(piezoPin, 70);} else {noTone(piezoPin);}
        }
       
        // ========================== 4 SEC
        if ((millis()- sc_start > 2000)&&(millis()- sc_start  < 4000)) {
          Serial.print("-S4-");
          offLights();
          digitalWrite(ledPanel[2], HIGH);
          digitalWrite(ledPanel[6], HIGH);
          if ((millis()- sc_start < 2600)) {tone(piezoPin, 70);} else {noTone(piezoPin);}     
        }
        
        // ========================== 6 SEC
        if ((millis()- sc_start > 4000)&&(millis()- sc_start  < 6000)) { 
          Serial.print("-S6-");
          offLights();
          digitalWrite(ledPanel[3], HIGH);
          digitalWrite(ledPanel[7], HIGH);
          if ((millis()- sc_start < 4600)) {tone(piezoPin, 70);} else {noTone(piezoPin);}
        }
        
        // ========================== 8 SEC
        if ((millis()- sc_start > 6000)&&(millis()- sc_start  < 8000)) { 
          Serial.print("-S8-");
          offLights();
          digitalWrite(ledPanel[5], HIGH);
          digitalWrite(ledPanel[1], HIGH);
          if ((millis()- sc_start < 6700)) {tone(piezoPin, 70);} else {noTone(piezoPin);}
        }  
        
        // ========================== 10 SEC 
        if ((millis()- sc_start > 8000)&&(millis()- sc_start  < 10000)) {
          Serial.print("-S10-");
          offLights();
          digitalWrite(ledPanel[4], HIGH);
          digitalWrite(ledPanel[1], HIGH);     
          if ((millis()- sc_start < 8600)) {tone(piezoPin, 70);} else {noTone(piezoPin);}
        }
 
        // ==========================   > ~ SEC
        if(millis()- sc_start >10000) {
          Serial.print("START");
          //  if ((millis()- sc_start > 15000)&&(millis()- sc_start < 18000)) {
          onLights();      
          sc_start = 0;
          isPress = true;
          soundON = 0;
          isFinishSound = true;
            //  }
         // ----------------------------------------------------
            
       } // endIF
    }
    else {
        Serial.println("- Off -");
        if (!isFinishLed){circleLights();} 
        else {offLights();}
       // digitalWrite(ledPin, LOW);
        sc_start = 0;
        isPress = false;
        isTime = false;
        noTone(piezoPin);
    }
    
    // Кнопка была нажата 10 сек
    if (isFinishSound) {
        Serial.print("- Music -");
        // Music On
        for (int i = 0; i < numTones; i++)
          {
            tone(piezoPin, tones[i]);
            delay(500);
          }
        noTone(piezoPin);

        delay(1000);
        Serial.print("- SEND -");
        isFinishSound = false;
        sc_start = 0;
        isPress = false;
        isTime = false;
        isFinishPress = true;
        isFinishLed = true;
        digitalWrite(ledPin, LOW);
    
        Wire.beginTransmission(8);
        // Send the question: H=humidity, T=temperature
        // Wire.write("H");
        Wire.write(x);   
        Wire.endTransmission();
        Serial.println(x);
        
        // realy
        sc_start_relay = millis(); // замеряем время до начала подключения
        Serial.print(sc_start_relay);
        
        digitalWrite(relayStartPin, HIGH);
        Serial.println("relayStartPin - ONN ");
        //delay(100);
        //digitalWrite(relayStartPin, HIGH);
      
       // Serial.println(millis() - sc_start_relay);
        

          
    }
    
    //
    if ((millis() - sc_start_relay > 1100)) {
        digitalWrite(relayStartPin, LOW);
        isRelayRestart = true;
        Serial.println("relayStartPin - OFF ");
    }
  
    delay(10);
}//END SKETCH
