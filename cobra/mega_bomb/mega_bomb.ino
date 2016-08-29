#include <Bounce2.h>

#include <SPI.h> // Include the Arduino SPI library
//#include <Servo.h>
#include "TM1637.h"

int explodePin = 43;
int doorPin = 41;
int activationPin = 39;
int lightPin = 37;
int winSoundPin = 35;
int activationButtonState = 0;
boolean fewCutPerSession = false;
int explosionTime = 7000;

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
int onUpPreActivated = false;
int onUpActivated = false;
int preUpActivatedSeconds = 0 ;

int stopperDownState = false;
int stopperUpState = false;


int secondsLeftForLiftingUp = 60 * 5;//30;

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
int countdownOptions[] = {1, 5, 15, 60, 45, 100}; //Possible values to count down from
int countdownSetting = 3; 

/*int cablePin[] = {4, 5, 6, 7};
int defuseOptions[2][4] = {{4, 7, 5, 6},
                           {0, 0, 0, 0}}; 
*/

const int wire1Pin = 4;
const int wire2Pin = 5;
const int wire3Pin = 6;
const int wire4Pin = 7;

Bounce wireBounce1 = Bounce(); 
Bounce wireBounce2 = Bounce(); 
Bounce wireBounce3 = Bounce(); 
Bounce wireBounce4 = Bounce(); 

Bounce startButtonBounce = Bounce(); 
//Bounce downButtonBounce = Bounce(); 
//Bounce upButtonBounce = Bounce(); 

// WIRE 1 - BLUE   - 4
// WIRE 2 - WHITE  - 5
// WIRE 3 - GREEN  - 6
// WIRE 4 - ORANGE - 7
// WHITE -> ORANGE -> BLUE -> GREEN

/*int defuseOptions[2][4] = {{wire2Pin, wire4Pin, wire1Pin, wire3Pin},
                           {0, 0, 0, 0}};
                           */
int defuseOptions[2][4] = {{2, 4, 1, 3},
                           {0, 0, 0, 0}};
                                                      
long subDefuseTime = 0;
int isBombDefused = 0;
int wireCutIndex = 0 ;                           

boolean isGoodDef = false;
boolean canStart = true;
boolean isWin = false;
boolean isLost = false;
boolean exploded = false;
boolean winDone = false;
int countDef = 0;
int wrongWireExtraTime = 15;// 20 ; // in minutes

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
   digitalWrite(explodePin, HIGH);
   
   pinMode(doorPin, OUTPUT); 
   digitalWrite(doorPin, HIGH);

   pinMode(winSoundPin, OUTPUT);
   digitalWrite(winSoundPin, HIGH);
   
   
   pinMode(activationPin, INPUT);
   
   pinMode(lightPin, OUTPUT);
   digitalWrite(lightPin, HIGH);
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

   startButtonBounce.attach(buttonStrartPin);
   startButtonBounce.interval(10); //

   /*
   downButtonBounce.attach(buttonDown);
   downButtonBounce.interval(10); //

   upButtonBounce.attach(buttonUp);
   upButtonBounce.interval(10); //
   */

   
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
  analogWrite(cdMotorEn, 70);
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
  
  Serial.println(canStart);
  
  if(!canStart){
    return;
  }
  
  canStart = false;
  defuseOptions[1][0] = 0;
  defuseOptions[1][1] = 0;
  defuseOptions[1][2] = 0;
  defuseOptions[1][3] = 0;
  liftedUp = false;
  
  notLiftedYet = false;
  bombActivated = false;
  onUpActivated = false;
  preUpActivatedSeconds = 0;
  onUpPreActivated = false;
  
  subDefuseTime = 0;
  newDefuseTime = 0;
  
  isBombDefused = false;
  counting = true;
  gameAcitvated = true;
  
  shutDownAnturage();
  
  wireCutIndex = 0 ;
  isLost = false;
  
  isWin = false;
  exploded = false;
  winDone = false;
  //count();
  
  //delay(500);
  
  int beepsBeforeStart = 5;
  for(int i = 0 ; i < beepsBeforeStart; i++){
    beep(500);
  }
  lastStartTime = millis();
  digitalWrite(doorPin, LOW); 
  digitalWrite(winSoundPin, HIGH);
  
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

  wireBounce1.attach(wire1Pin);
  wireBounce1.interval(5); // interval in ms
  
  wireBounce2.attach(wire2Pin);
  wireBounce2.interval(5); // interval in ms
  
  wireBounce3.attach(wire3Pin);
  wireBounce3.interval(5); // interval in ms
  
  wireBounce4.attach(wire4Pin);
  wireBounce4.interval(5); // interval in ms
}

Bounce getBounceByIndex(int index){

    switch(index){
      case 1:
      return wireBounce1;
      break;

      case 2:
      return wireBounce2;
      break;

      case 3:
      return wireBounce3;
      break;

      case 4:
      return wireBounce4;
      break;
    }
  

}

void updateBounces(){
  wireBounce1.update();
  wireBounce2.update();
  wireBounce3.update();
  wireBounce4.update();
}

void updateButtonsBounces(){
  startButtonBounce.update();
  //upButtonBounce.update();
  //downButtonBounce.update();
}

void checkWires(){
  
  
  if(!onUpActivated){
    return;
  }

  updateBounces();
  
  
  int cuts_per_session = 0;
  boolean wrongCut = false;
  boolean wireHasBeenCut = false;
  for(int i  = 0; i < 4; i++){
    Bounce tmpBounce = getBounceByIndex(defuseOptions[0][i]);
    //Serial.println(tmpBounce);
    //tmpBounce.update();
    int isWireOn = tmpBounce.read();
    //Serial.print(isWireOn);
    //Serial.println(defuseOptions[0][i]);
    //int isWireOn = digitalRead(defuseOptions[0][i]);
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
          //subDefuseTime += wrongWireExtraTime;
          //timerRunDown();
          wrongCut = true;
        }
        cuts_per_session++;
        delay(100);
        wireHasBeenCut = true;
        //break;
      }
      else{
        if(i == wireCutIndex){
          wireCutIndex ++;
        }
      }
    }
  }
  
  if(cuts_per_session >= 2){
    Serial.print(cuts_per_session);
    Serial.println (" cuts per session!!!");
    for(int k = 0; k < cuts_per_session; k++){
      subDefuseTime += wrongWireExtraTime;
    }
    timerRunDown();
  }
  
  if(wrongCut){
     subDefuseTime += wrongWireExtraTime;
     timerRunDown();
     //return;
  }  
  
  if(wireHasBeenCut){
    return;
  }
  
  if(wireCutIndex >= 4){
    isBombDefused = true; 
    shutDownAnturage();

  }
}

void doWin(){
  if(winDone){
    return;
  }
  
  winDone = true;
  shutDownAnturage();
  digitalWrite(winSoundPin, LOW);
  delay(15000); // playing 15 sec 
  digitalWrite(winSoundPin, HIGH);
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
  
  // sound
  int beepDelay = 150;
  int beepsCount = 20;
  for(int i = 0 ; i < beepsCount; i++){
    
    beep(beepDelay - i * 7);
  }
    
  digitalWrite(explodePin, LOW); 
  
  digitalWrite(doorPin, HIGH); 
  //explodeServo.attach(explodeServoPin);
  //explodeServo.write(0);
  // let smoke go out for 3 sec
  movingUp = false;
  delay(explosionTime);
  digitalWrite(explodePin, HIGH); 
  
  //playDartWader();
}

void shutDownAnturage(){
  stopLiqMotor();
  stopCdMotor();  
  digitalWrite (ledPin2, LOW);
  digitalWrite (ledPin1, LOW); 
  digitalWrite(doorPin, HIGH);
}

void stopGame(){
  gameAcitvated = false;
  isLost = false;
  isWin = false;
  preUpActivatedSeconds = 0;
  digitalWrite(doorPin, HIGH); 
  digitalWrite(winSoundPin, HIGH);
  shutDownAnturage();
}

void loop()
{
  
  updateButtonsBounces(); 
//  buttonState = digitalRead(buttonPin);
//  if (buttonState == HIGH) {   
//     counting = true; 
//  }
  
  lifting();
  
  if(!gameAcitvated){
     
     return;
   }
   
   //Serial.println(onUpActivated);
   
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
    //canStart = true;
  }
 
 stopperUpState = digitalRead(stopperUp);
  if(stopperUpState){
     //Serial.print("NO FORMAT");  
     //delay(100);
     liftedUp = false;
     digitalWrite(lightPin, HIGH);
  } 
  else{
    if(!liftedUp){
      //clearDisplaySPI();
      tm1637.clearDisplay();
      tm1637.point(false);
    }
    liftedUp = true;
    if(!onUpPreActivated){
      onUpPreActivated = true;
    }
    //Serial.println("up reached");  
    digitalWrite(lightPin, LOW);
    
  }
  
  //if(digitalRead(buttonStrartPin)){
  if(startButtonBounce.read()){
    Serial.println("start button pressed");  
    delay(250);
    startGame();
    
  }
  
  buttonUpState =   digitalRead(buttonUp);
  buttonDownState = digitalRead(buttonDown);
  
  if(buttonDownState){
    Serial.print("DOWN PRESSED");  
    if(movingDown){
      movingDown = false;
    }
    else{
      movingDown = true;
    }
    
    stopGame();
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
    if(onUpPreActivated && preUpActivatedSeconds <= 0){
       preUpActivatedSeconds = secondsPassed;
       //onUpPreActivated = true;
       
       
    }
    
    if(!onUpActivated && preUpActivatedSeconds > 0 && (secondsPassed - preUpActivatedSeconds) > 10 ){
      
      onUpActivated = true;
    }
    
    secondsPassed += subDefuseTime * 60;
    
    
    
    int lastMinute = countdownOptions[countdownSetting];
    int lastSecond = lastMinute * 60; // ???
    
    int secondsLeft = lastSecond - secondsPassed;
    
    if(secondsLeft % 60 == 0){
      clearDisplaySPI();
    }
    
    activationButtonState = digitalRead(activationPin);
  
    
    if( ( secondsLeft < secondsLeftForLiftingUp || activationButtonState) && !bombActivated){// !liftedUp && notLiftedYet ){
      movingUp = true;
      //onUpActivated = false;
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
    
    //Serial.print(secondsLeft);
    //Serial.print( " ");
    //Serial.println(currentTimeValue);
    //Вывод на дисплей
    displayNumber(currentTimeValue);
    countdownTime = currentTimeValue.toInt(); //Global Time
    
    
    //Serial.print(" ");
    //Serial.println(secondsDisplay);
    
   if(isBombDefused){
     isWin = true; 
     doWin();
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
  
  analogWrite (speakerPin, 0);
  //delay(delayms); //ждем 100 Мс
  
  
  while(var < 1000){
    cnt = countdownTime - var;
    displayNumber((String)cnt);
    if(cnt <= 0){
      displayNumber("0");
      break;
    }
    var++;
  }
  
  analogWrite (speakerPin, 255);
}
  //
void displayNumber(String line){
   

   counter = line.toInt();
   sprintf(tempString, "%4d", counter);
   String timeStringToDispay = (String)tempString;
   
   // outputting to bomb screen
   dec(counter);
   
   if(liftedUp){
     s7sSendStringSPI(timeStringToDispay);   
   }
   else{
   //beep(50);
   // outputing to main screen
   //*
     timeStringToDispay.toCharArray(mainScreenBuffer, 5);
     for(int i = 0; i<4; i++){
       mainTimeDisp[i]= mainScreenBuffer[i] - 48;
     }
  
     tm1637.display(mainTimeDisp); 
   }
  
  //*/
   
  // leds 
   if(bombActivated && onUpActivated){
     led(counter);
     anturageMotorsAction();
   }   
  
 }
 
void dec(int isEvenNumber){
   if (isEvenNumber % 2 == 0) { 
     
     if(liftedUp){
       setDecimalsSPI(0b00010000);
     }
     else{
       tm1637.point(true);
     }
 
   } else{ 
     if(liftedUp){
       setDecimalsSPI(0b00000000);
     }
     else{
       tm1637.point(false);
     }
   }
}

void led(int isEvenNumber){
  if (isEvenNumber % 2 == 0) { 
     digitalWrite (ledPin1, HIGH);
     digitalWrite (ledPin2, LOW);  
  //tone (speakerPin, 777);
  } else{ 
    digitalWrite (ledPin1, LOW);
    digitalWrite (ledPin2, HIGH);
  //digitalWrite(speakerPin, LOW);
   //noTone(speakerPin); 
 }
}
/***************SPI LIB*********************/
void s7sSendStringSPI(String toSend)
{
  digitalWrite(ssPin, LOW);
 

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
  unsigned long now = millis() / 200;
  
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

#define  c3    7634
#define  d3    6803
#define  e3    6061
#define  f3    5714
#define  g3    5102
#define  a3    4545
#define  b3    4049
#define  c4    3816    // 261 Hz 
#define  d4    3401    // 294 Hz 
#define  e4    3030    // 329 Hz 
#define  f4    2865    // 349 Hz 
#define  g4    2551    // 392 Hz 
#define  a4    2272    // 440 Hz 
#define  a4s   2146
#define  b4    2028    // 493 Hz 
#define  c5    1912    // 523 Hz
#define  d5    1706
#define  d5s   1608
#define  e5    1517    // 659 Hz
#define  f5    1433    // 698 Hz
#define  g5    1276
#define  a5    1136
#define  a5s   1073
#define  b5    1012
#define  c6    955
 
#define  R     0 

int melody1[] = {  a4, R,  a4, R,  a4, R,  f4, R, c5, R,  a4, R,  f4, R, c5, R, a4, R,  e5, R,  e5, R,  e5, R,  f5, R, c5, R,  g5, R,  f5, R,  c5, R, a4, R};
int beats1[]  = {  50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 5, 20, 5, 60, 80, 50, 20, 50, 20, 50, 20, 40, 5, 20, 5,  60, 10, 40, 5,  20, 5, 60, 40};
 
// Melody 2: Star Wars Theme
int melody2[] = {  f4,  f4, f4,  a4s,   f5,  d5s,  d5,  c5, a5s, f5, d5s,  d5,  c5, a5s, f5, d5s, d5, d5s,   c5};
int beats2[]  = {  21,  21, 21,  128,  128,   21,  21,  21, 128, 64,  21,  21,  21, 128, 64,  21, 21,  21, 128 }; 
 
int MAX_COUNT = sizeof(melody2) / 2; // Melody length, for looping.
 
long tempo = 10000; // Set overall tempo
 
int pause = 1000; // Set length of pause between notes
 
int rest_count = 50; // Loop variable to increase Rest length (BLETCHEROUS HACK; See NOTES)
 
// Initialize core variables
int toneM = 0;
int beat = 0;
long melodDuration  = 0;

 
// PLAY TONE  //
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;
  if (toneM > 0) { // if this isn't a Rest beat, while the tone has 
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < melodDuration) {
 
      //digitalWrite(speakerOut,HIGH);
      analogWrite (speakerPin, 0);
      delayMicroseconds(toneM / 2);
 
      // DOWN
      //digitalWrite(speakerOut, LOW);
      analogWrite (speakerPin, 255);
      delayMicroseconds(toneM / 2);
 
      // Keep track of how long we pulsed
      elapsed_time += (toneM);
    } 
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(melodDuration);  
    }                                
  }                                 
}
 
void playDartWader(){
   int MAX_COUNT = sizeof(melody1) / 2;
   for(int j = 0 ; j < 2; j++){ 
     for (int i=0; i<MAX_COUNT; i++) {
       toneM = melody1[i];
       beat = beats1[i];
 
       melodDuration = beat * tempo; // Set up timing
 
      playTone(); // A pause between notes
      delayMicroseconds(pause);
    }
   }
    
    analogWrite (speakerPin, 255);
}

void playStarWars(){
  
   int MAX_COUNT = sizeof(melody2) / 2;
   for(int j = 0 ; j < 2; j++){ 
     for (int i=0; i<MAX_COUNT; i++) {
      toneM = melody2[i];
      beat = beats2[i];
 
      melodDuration = beat * tempo; // Set up timing
 
      playTone(); // A pause between notes
      delayMicroseconds(pause);
     }
   }  
   
   analogWrite (speakerPin, 255);
    

}
