#include <SPI.h> // Include the Arduino SPI library
//#include <Servo.h>
#include "TM1637.h"

int explodePin = 43;

// anturage motors
int liqMotorA = 9;
int liqMotorB = 10;
int liqMotorEn = 8;

int cdMotorA = 11;
int cdMotorB = 12;
int cdMotorEn = 13;

int liqMotorLastMills = 0;
int liqMotorStage = 0;

int cdMotorLastMills = 0;
int cdMotorStage = 0;

// screen
#define MAIN_SCREEN_CLK 47//pins definitions for TM1637 and can be changed to other ports       
#define MAIN_SCREEN_DIO 45
TM1637 tm1637(MAIN_SCREEN_CLK, MAIN_SCREEN_DIO);
int8_t mainTimeDisp[] = {0x00,0x00,0x00,0x00};
char mainScreenBuffer[4];

// lift stuff
int MOTOR_A = 30; // Input3 подключен к выводу 5 
int MOTOR_B = 32;
int SPEEDIN = 28;

const int buttonUp = 22; 
const int buttonDown = 24; 

const int buttonStrartPin = 49; 


const int stopperDown = 36; 
const int stopperUp = 34; 

int buttonUpState = 0;
int buttonDownState = 0;

int movingDown = false;
int movingUp = false;
int liftedUp = false;
int notLiftedYet = true;
int bombActivated = false;

int stopperDownState = false;
int stopperUpState = false;


int secondsLeftForLiftingUp = 30;// 60 * 4; // 4 minute

// bomb stuff
//const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin1 = 40; 
const int ledPin2 = 42; 

const int speakerPin = 2;

/**/
/* Possible start point for counting down */
const int ssPin = 38;

// MEGA SDI pin = 51
// mege sck pin = 52
int countdownOptions[] = {1, 5, 15, 60, 45}; //Possible values to count down from
int countdownSetting = 0; 

/*int cablePin[] = {4, 5, 6, 7};
int defuseOptions[2][4] = {{4, 7, 5, 6},
                           {0, 0, 0, 0}}; 
*/

const int wire1Pin = 4;
const int wire2Pin = 5;
const int wire3Pin = 6;
const int wire4Pin = 7;

int defuseOptions[2][4] = {{wire1Pin, wire2Pin, wire3Pin, wire4Pin},
                           {0, 0, 0, 0}};
long subDefuseTime = 0;
int isBombDefused = 0;
int wireCutIndex = 0 ;                           

boolean isGoodDef = false;
boolean canStart = false;
boolean isWin = false;
boolean isLost = false;
boolean exploded = false;
int countDef = 0;
int wrongWireExtraTime = 20;

/* Values to prevent the button from bouncing */
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

/* Values for storing a count down session */
long lastStartTime = 0;    //When did we start
long newDefuseTime = 0;
long countdownTime = 0;
boolean counting = false;  //Are we counting at this moment?
boolean gameAcitvated = false;     //Is the clock active or suspended (active can be "not yet counting, but the button is pressed")
//

unsigned int counter = 0;  // This variable will count up to 65k
char tempString[10];  // Will be used with sprintf to create strings

void setup()
{
  
  Serial.begin(9600);
  
   pinMode(explodePin, OUTPUT); 
   //explodeServo.attach(explodeServoPin);
   //explodeServo.write(90);
   //delay(1000);
   //explodeServo.detach();
   
   counting = true;
  // init lift
  
   pinMode (SPEEDIN, OUTPUT); 
   pinMode (MOTOR_A, OUTPUT);
   pinMode (MOTOR_B, OUTPUT);
 
   pinMode(buttonUp, INPUT);
   pinMode(buttonDown, INPUT);
   pinMode(buttonStrartPin, INPUT);
   pinMode(stopperDown, INPUT);
   pinMode(stopperUp, INPUT);
  
  // init bomb
  //pinMode(buttonPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  
  analogWrite (speakerPin, 255);
  
  initCable();
  // -------- SPI initialization
  
  
  initScreens();
  initAnturageMotors();
}

void initAnturageMotors(){
  pinMode(liqMotorEn, OUTPUT);
  pinMode(liqMotorA, OUTPUT);
  pinMode(liqMotorB, OUTPUT);
  
  pinMode(cdMotorEn, OUTPUT);
  pinMode(cdMotorA, OUTPUT);
  pinMode(cdMotorB, OUTPUT);
  
  //pinMode(in3, OUTPUT);
  //pinMode(in4, OUTPUT);
  //analogWrite(enA, 30);
  analogWrite(liqMotorEn, 130);
  analogWrite(cdMotorEn, 45);
}

void startGame(){
   
  //clearDisplaySPI(); 
  /*
  counting = true;
  
  movingDown = false;
  movingUp = false;
  liftedUp = false;
  notLiftedYet = true;
  bombActivated = false;

  stopperDownState = false;
  stopperUpState = false;
  counter = 0;
  
  lastStartTime = 0;    //When did we start
  newDefuseTime = 0;
  countdownTime = 0;
  counting = false;  //Are we counting at this moment?
  active = true; 
  exploded = false;
  
  defuseOptions[1][0] = 0;
  defuseOptions[1][1] = 0;
  defuseOptions[1][2] = 0;
  defuseOptions[1][3] = 0;
  
  subDefuseTime = 0;
  isBombDefused = 0;
  wireCutIndex = 0 ;                           

  isGoodDef = false;
  
  countDef = 0;
  */
  
  if(!canStart){
    return;
  }
  
  canStart = false;
  defuseOptions[1][0] = 0;
  defuseOptions[1][1] = 0;
  defuseOptions[1][2] = 0;
  defuseOptions[1][3] = 0;
  
  notLiftedYet = false;
  bombActivated = false;
  
  subDefuseTime = 0;
  newDefuseTime = 0;
  
  isBombDefused = false;
  counting = true;
  gameAcitvated = true;
  
  
  
  wireCutIndex = 0 ;
  isLost = false;
  
  isWin = false;
  exploded = false;
  //count();
  
  //delay(500);
  
  for(int i = 0 ; i < 1; i++){
    beep(500);
  }
  lastStartTime = millis();
  
  
}

void initScreens(){
  tm1637.set();//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637.init();  
  pinMode(ssPin, OUTPUT);  // Set the SS pin as an output
  digitalWrite(ssPin, HIGH);  // Set the SS pin HIGH
  
  SPI.begin();  // Begin SPI hardware
  SPI.setClockDivider(SPI_CLOCK_DIV64);  // Slow down SPI clock
  // --------

  clearDisplaySPI(); 
}

void initCable(){
  pinMode(wire1Pin, INPUT);
  pinMode(wire2Pin, INPUT);
  pinMode(wire3Pin, INPUT);
  pinMode(wire4Pin, INPUT);
}


void checkWires(){
  for(int i  = 0; i < 4; i++){
    int isWireOn = digitalRead(defuseOptions[0][i]);
    if(defuseOptions[1][i]){
      isWireOn = false;
    }
    
    if(!isWireOn){
      if(!defuseOptions[1][i]){
        defuseOptions[1][i] = defuseOptions[1][i] + 1;
        Serial.print(i);
        Serial.println (" has been cut");
        if(i == wireCutIndex){
          wireCutIndex ++;  
        }
        else{
          Serial.println (" WRONG!!!");
          subDefuseTime += wrongWireExtraTime;
          timerRunDown();
        }
        delay(100);
        return;
      }
      else{
        if(i == wireCutIndex){
          wireCutIndex ++;
        }
      }
    }
  }
  
  if(wireCutIndex >= 4){
    isBombDefused = true; 
    //Serial.println (subDefuseTime);
  }
}

void explode(){
  if(exploded){
    return;
  }
  
  exploded = true;
  //SPI.end();
  //digitalWrite (ledPin2, LOW);
   // digitalWrite (ledPin1, LOW);
    //digitalWrite(speakerPin, LOW);
 //stopLiqMotor();
 //stopCdMotor();   
  shutDownAnturage();
    
  //digitalWrite(ssPin, LOW);  
  isLost = true;
    
  digitalWrite(explodePin, HIGH); 
  //explodeServo.attach(explodeServoPin);
  //explodeServo.write(0);
  // let smoke go out for 3 sec
  movingUp = false;
  delay(3000);
  digitalWrite(explodePin, LOW); 
}

void shutDownAnturage(){
  stopLiqMotor();
  stopCdMotor();  
  digitalWrite (ledPin2, LOW);
  digitalWrite (ledPin1, LOW); 
}

void loop()
{
  
   
//  buttonState = digitalRead(buttonPin);
//  if (buttonState == HIGH) {   
//     counting = true; 
//  }
  
  lifting();
  
  if(!gameAcitvated){
     
     return;
   }
   
  if(isLost){
    explode();
    Serial.println("LOST");
    
  }
  else if (!isWin){
    //evaluateButton();
    checkWires();
    count();
  } 
  
  if(isLost || isWin) {
    //Serial.println("lost or win");
    shutDownAnturage();
    //digitalWrite(speakerPin, LOW);
    
    //noTone(speakerPin);
  }
}

void lifting(){
  stopperDownState = digitalRead(stopperDown);
  if(stopperDownState){
     
  }
  else{
    //Serial.println("down reached");  
    canStart = true;
  }
 
 stopperUpState = digitalRead(stopperUp);
  if(stopperUpState){
     //Serial.print("NO FORMAT");  
     //delay(100);
     liftedUp = false;
  } 
  else{
    liftedUp = true;
    //Serial.println("up reached");  
  }
  
  if(digitalRead(buttonStrartPin)){
    //Serial.println("start button pressed");  
    startGame();
    
  }
  
  buttonUpState = digitalRead(buttonUp);
  buttonDownState = digitalRead(buttonDown);
  
  if(buttonDownState){
    Serial.print("DOWN PRESSED");  
    if(movingDown){
      movingDown = false;
    }
    else{
      movingDown = true;
    }
    
    canStart = true;
    movingUp = false;
    delay(250);
  }
  else if(buttonUpState){
    Serial.print("UP PRESSED");  
    movingDown = false;
    if(movingUp){
      movingUp = false;
    }
    else{
      movingUp = true;
    }
    delay(250);
  }
  
  if(!stopperDownState){
    movingDown = false;
  }
  
  if(!stopperUpState){
    movingUp = false;
  }
  
  // На пару выводов "IN" поданы разноименные сигналы, мотор готов к вращаению
  if(movingDown){
    digitalWrite (MOTOR_A, HIGH);
    digitalWrite (MOTOR_B, LOW);
    //Serial.println("moving down");  
    
  // подаем на вывод ENB управляющий ШИМ сигнал 
  }
  else if(movingUp){
    digitalWrite (MOTOR_A, LOW);
    digitalWrite (MOTOR_B, HIGH);
    //Serial.println("moving up");  
  }
  
  if(movingUp || movingDown ){
    //delay(2000);
    analogWrite(SPEEDIN, 140);
    //delay(2000);
    // Останавливаем мотор повад на вывод ENB сигнал низкого уровеня. 
    // Состояние выводов "IN" роли не играет.
  } 
  else{
    digitalWrite (MOTOR_A, LOW);
    digitalWrite (MOTOR_B, LOW);
    analogWrite(SPEEDIN, 0);
  } 
}

void count() {
  
  //Serial.print(counting);
  
  if (counting){
    unsigned long now = millis();
    int secondsPassed = (now-lastStartTime)/1000;
    secondsPassed += subDefuseTime * 60;
    
    
    
    int lastMinute = countdownOptions[countdownSetting];
    int lastSecond = lastMinute * 60; // ???
    
    int secondsLeft = lastSecond - secondsPassed;
    if(secondsLeft < secondsLeftForLiftingUp && !bombActivated){// !liftedUp && notLiftedYet ){
      movingUp = true;
      notLiftedYet = false;
      bombActivated = true;
    }
    //Serial.println(secondsLeft);  
    
    
    //int secondsDisplay = (lastSecond - secondsPassed) % 60;
    int minutesDisplay = floor( secondsLeft / 60);
    
    //(((lastMinute - newDefuseTime) * 60) - secondsPassed) / 60;
    
    int secondsDisplay = secondsLeft - minutesDisplay * 60;
    //(lastSecond - secondsPassed) % 60;
    
    //Serial.println(" counting ");
    
    
    String currentTimeValue = "";
    
    currentTimeValue += (String)minutesDisplay;
    // Serial.print(":");
    if(secondsDisplay < 10) {
    //  Serial.print("0");
      currentTimeValue += "0";
    }
    currentTimeValue += (String)secondsDisplay;
    
    Serial.print(secondsLeft);
    //Serial.print( " ");
    //Serial.println(currentTimeValue);
    //Вывод на дисплей
    displayNumber(currentTimeValue);
    countdownTime = currentTimeValue.toInt(); //Global Time
    
    
    //Serial.print(" ");
    //Serial.println(secondsDisplay);
    
   if(isBombDefused){
     isWin = true; 
    }
    else if(secondsLeft <= 0 ){
      isLost = true;
      Serial.println("LOST");
      displayNumber("0");
      
      counting = false;
      gameAcitvated = false;
      
      explode();
    }
    
    /*
    if(secondsDisplay == 0 && minutesDisplay == 0) {
      counting = false;
      gameAcitvated = false;
    }*/
    
  } //counting
}// END count()



void timerRunDown(){
  
  int var = 0;
  int cnt = 0;  
  
  while(var < 1000){
    cnt = countdownTime - var;
    displayNumber((String)cnt);
    if(cnt <= 0){
      displayNumber("0");
      break;
    }
    var++;
  }
}
  //
void displayNumber(String line){
   
   Serial.println(line); 
   counter = line.toInt();
   sprintf(tempString, "%4d", counter);
   String timeStringToDispay = (String)tempString;
   
   // outputting to bomb screen
   s7sSendStringSPI(timeStringToDispay);   
   //beep(50);
   dec(counter);
   
   
   // outputing to main screen
   //*
   timeStringToDispay.toCharArray(mainScreenBuffer, 5);
   for(int i = 0; i<4; i++){
    mainTimeDisp[i]= mainScreenBuffer[i] - 48;
  }
  
  tm1637.display(mainTimeDisp); 
  
  //*/
   
  // leds 
   if(bombActivated){
     led(counter);
     anturageMotorsAction();
   }   
  
 }
 
void dec(int isEvenNumber){
   if (isEvenNumber % 2 == 0) { 
     
     setDecimalsSPI(0b00010000);
     tm1637.point(true);
 
   } else{ 
     setDecimalsSPI(0b00000000);
     tm1637.point(false);
   }
}

void led(int isEvenNumber){
  if (isEvenNumber % 2 == 0) { digitalWrite (ledPin1, HIGH);digitalWrite (ledPin2, LOW);  
  //tone (speakerPin, 777);
  } else{ digitalWrite (ledPin1, LOW);digitalWrite (ledPin2, HIGH);
  //digitalWrite(speakerPin, LOW);
   //noTone(speakerPin); 
 }
}
/***************SPI LIB*********************/
void s7sSendStringSPI(String toSend)
{
  digitalWrite(ssPin, LOW);
 
  //Serial.println(toSend);
  for (int i=0; i<4; i++)
  {
    SPI.transfer((byte)toSend[i]);
  }
  
  digitalWrite(ssPin, HIGH);
}

void clearDisplaySPI()
{
  digitalWrite(ssPin, LOW);
  SPI.transfer(0x76);  // Clear display command
  digitalWrite(ssPin, HIGH);
}

void setBrightnessSPI(byte value)
{
  digitalWrite(ssPin, LOW);
  SPI.transfer(0x7A);  // Set brightness command byte
  SPI.transfer(value);  // brightness data byte
  digitalWrite(ssPin, HIGH);
}

void setDecimalsSPI(byte decimals)
{
  digitalWrite(ssPin, LOW);
  SPI.transfer(0x77);
  SPI.transfer(decimals);
  digitalWrite(ssPin, HIGH);
}

void beep(unsigned char delayms){

  //tone (speakerPin, 2000); //включаем на 500 Гц
  //digitalWrite(speakerPin, HIGH);
  analogWrite (speakerPin, 0);
  delay(delayms); //ждем 100 Мс
  analogWrite (speakerPin, 255);
  delay(delayms);
  //digitalWrite(speakerPin, HIGH);
 
  
}



void anturageMotorsAction()
{
  if( isLost || isWin ){
    return;
  }
  
  //liqMotorA
  unsigned long now = millis() / 1000;
  long diff = now - liqMotorLastMills;

  
  if(  diff > 6){
    liqMotorLastMills = now;
    liqMotorStage++;
    if(liqMotorStage > 3){
      liqMotorStage = 0;
    }
  }
  
  if(liqMotorStage == 0 ){
    digitalWrite(liqMotorA, HIGH);
    digitalWrite(liqMotorB, LOW);
    stopCdMotor();
  }
  else if(liqMotorStage == 2 ){
    digitalWrite(liqMotorA, LOW);
    digitalWrite(liqMotorB, HIGH);
    stopCdMotor();
  }
  else {
    digitalWrite(liqMotorA, LOW);
    digitalWrite(liqMotorB, LOW);
  
    activateCdMotor();
  }
  
} 

void activateCdMotor(){
  unsigned long now = millis() / 500;
  
  long cdDiff = now - cdMotorLastMills;
  if(  cdDiff > 1){
    cdMotorLastMills = now;
    cdMotorStage++;
    if(cdMotorStage > 1){
      cdMotorStage = 0;
    }
  }
  
   if(cdMotorStage == 0 ){
    digitalWrite(cdMotorA, HIGH);
    digitalWrite(cdMotorB, LOW);
  }
  else if(cdMotorStage == 1 ){
    digitalWrite(cdMotorA, LOW);
    digitalWrite(cdMotorB, HIGH);
  }
  
}

void stopCdMotor(){
  digitalWrite(cdMotorA, LOW);
  digitalWrite(cdMotorB, LOW);
}

void stopLiqMotor(){
  digitalWrite(liqMotorA, LOW);
  digitalWrite(liqMotorB, LOW);
}
