
#include <LiquidCrystal_I2C.h>
#include <LCD_1602_RUS.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif



//LiquidCrystal_I2C lcd(0x27, 16, 2);
LCD_1602_RUS lcd(0x27, 16, 2);
Adafruit_NeoPixel pwdLed = Adafruit_NeoPixel(7, 5, NEO_GRB + NEO_KHZ800);
 /* Author: Danny van den Brande, Arduinosensors.nl
 This is a example on how to use the KY-040 Rotary encoder. 
 Its very basic but if your new to arduino or could not find 
 any code, then you have something to start with.
 because there is little documentation about the KY sensor kit.
 */
 int CLK = 9;  // Pin 9 to clk on encoder
 int DT = 8;  // Pin 8 to DT on encoder
 
 int RotPosition = 0; 
 int rotation;  
 int value;
 boolean LeftRight;

 const int buzzer = 3;

 
 const int stepsPerRound = 30;
 const int stepsPerRoundTrash = 29;
 int prevDirection = 0;
 int rotDirection = 0;
 int directionDelta = 0;

 int curRoundPos = 0;
 int curRounds = 0;

 const int led = 13;
 boolean ledOn = true;

 int winResultShowTime = 30;

 // right - 1
 // left +1
 const int pathSteps = 5;
 int curPathPos = 0;
 int prevCurPathPos = 0;
 int roundsPath[pathSteps] = { 3, -1, 2, -3, 1}; 


 int prevMills = millis();
 void setup() { 
   Serial.begin (9600);
   pinMode (CLK,INPUT);
   pinMode (DT,INPUT);
   rotation = digitalRead(CLK); 

   pinMode(led, OUTPUT);  

   //lcd.begin();
   lcd.begin();                      // initialize the lcd
  // Print a message to the LCD.

  pinMode (buzzer, OUTPUT);
  analogWrite (buzzer, 0);
  delay(50);
  analogWrite (buzzer, 255);
  delay(100);
  playSong();
  //analogWrite (speakerPin, 0);
  //delay(100);
   
   //lcd.print(L"Теперь можно");
   gameStart();

   pwdLed.begin();
   pwdLed.show();
   turnOnPixels();
 } 

 void turnOnPixels(){
   pwdLed.clear();
   for(int i=0;i<7;i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pwdLed.setPixelColor(i, pwdLed.Color(255,0,0)); // Moderately bright green color.
   }  
   pwdLed.show();
}

 void gameStart(){
  lcd.backlight();
  lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print(L"Спасите");
  //lcd.setCursor(0, 1);
  //lcd.print(L"Черную Жемчужину");
 }

 void showWin(){
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(L"Ищите");
  lcd.setCursor(0, 1);
  lcd.print(L"синего попугая");
 }

 

 void checkForWin(){
   if( roundsPath[curPathPos] == curRounds){
     curPathPos++;
     curRounds = 0;
     if(curPathPos >= pathSteps){
       Serial.println("WIN");
       showWin();
       playSong();
       curPathPos = 0;
       delay(winResultShowTime * 1000);
       gameStart();
     }
   }
   else if(curRounds > 0 && roundsPath[curPathPos] < 0){
      curPathPos = 0;
   }
   else if(curRounds < 0 && roundsPath[curPathPos] > 0){
      curPathPos = 0;
   }

   if(prevCurPathPos != curPathPos){
    prevCurPathPos = curPathPos;
   }
   
 }
 
 void loop() { 
     
     value = digitalRead(CLK);
     int dtVal = digitalRead(DT);
     if (value != rotation){ // we use the DT pin to find out which way we turning.
     if (digitalRead(DT) != value) {  // Clockwise
       RotPosition ++;
       LeftRight = true;
     } else { //Counterclockwise
       LeftRight = false;
       RotPosition--;
     }
     if (LeftRight){ // turning right will turn on red led.
       //Serial.println ("clockwise");
       prevDirection = -1;
       rotDirection = -1;
       //digitalWrite (RedLed, HIGH);
       //digitalWrite (GreenLed, LOW);
     }else{        // turning left will turn on green led.   
       //Serial.println("counterclockwise");
       prevDirection = 1;
       rotDirection = 1;
       //digitalWrite (RedLed, LOW);
       //digitalWrite (GreenLed, HIGH);
     }

     directionDelta += rotDirection;
     if(directionDelta < -2){
       directionDelta = -2;
     }
     else if(directionDelta > 2){
       directionDelta = 2;
     }
     //Serial.println(directionDelta);
      
     if(directionDelta == 0){
      Serial.println("switch direction");
      curRoundPos = rotDirection;
      curRounds = 0;
     }
     else{
      curRoundPos += rotDirection;
     }

     if ( abs(round(curRoundPos / stepsPerRoundTrash) ) >= 1){
        curRoundPos = 0 ;
        Serial.println("full round");
        curRounds += rotDirection; 
        Serial.println(curRounds);
        // += 
      
     }
     //Serial.print("Encoder RotPosition: ");
     //Serial.println(RotPosition);
     // this will print in the serial monitor.
     
   } 
   //Serial.println( dtVal );
   rotation = value;

   checkForWin();
   

   
 } 

 void playSong(){
  for (int i=0;i<10;i++){              //203 is the total number of music notes in the song
  //int wait = duration[i] * songspeed;
  //tone(buzzer,notes[i],wait);          //tone(pin,frequency,duration)
  //delay(wait);
  delay(100);
  digitalWrite(buzzer, 0);
  delay(100);
  digitalWrite(buzzer, 255);
  }  
  
 
 }

