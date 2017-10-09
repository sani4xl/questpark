#include <Bounce2.h>

#include <SPI.h> // Include the Arduino SPI library
#include <Servo.h>
#include "TM1637.h"
#include <Keypad.h>
#include <SoftwareSerial.h>

#define buzzer 48
const int speakerPin = 48;
// SH_CP; 11
const int clockPin = 49;
// DS; 15
const int dataPin = 45;
// ST_CP; 12
const int latchPin = 47;

const int keyBeepDelay = 50;
bool coverOpened = false;
const int openCoverPin = 8;
Servo openServo;

const int keyPin = 2;
Servo keyServo;

//char password[4] = "7775";
char* password = "7775";
int password_len = 4;
int position = 0;
int guessed_digit = 0;

int relayBox = 13;  

const byte rows = 4;
const byte cols = 4;
const char keys [rows][cols] =
{
  {'1','2','3','F'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[rows] = {31, 29, 27, 25 }; //connect to the row pinouts of the keypad
byte colPins[cols] = {39, 37, 35, 33 }; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

int explodePin = 43;
int doorPin = 41;
int activationPin = 39;
int lightPin = 37;
int winSoundPin = 35;
int activationButtonState = 0;
boolean fewCutPerSession = false;
int explosionTime = 7000;



// screen
#define MAIN_SCREEN_CLK 49//pins definitions for TM1637 and can be changed to other ports       
#define MAIN_SCREEN_DIO 47
TM1637 tm1637(MAIN_SCREEN_CLK, MAIN_SCREEN_DIO);
int8_t mainTimeDisp[] = {0x00,0x00,0x00,0x00};
char mainScreenBuffer[4];

// lift stuff
int MOTOR_A = 30; // Input3 подключен к выводу 5 
int MOTOR_B = 32;
int SPEEDIN = 28;

const int buttonUp = 22; 
const int buttonDown = 24; 

const int buttonStrartPin = 3; // кнопка START


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

//const int speakerPin = 2;

/**/
/* Possible start point for counting down */
const int ssPin = 38;

// MEGA SDI pin = 51
// mege sck pin = 52
int countdownOptions[] = {1, 10, 15, 60, 45, 100}; //Possible values to count down from
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

Bounce startBtnBounce = Bounce(); 
Bounce downButtonBounce = Bounce(); 
Bounce upButtonBounce = Bounce(); 
Bounce activationBounce = Bounce(); 


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

int defuseCounterThrashold = 10;                                                      
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
int wrongWireExtraTime = 7;// 20 ; // in minutes

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
SoftwareSerial mp3Serial(10, 9); // RX, TX

//  001 start
//  002 defused
//  003 explosion
// 004 loser music
// 005 win music



void setup()
{
  Serial.begin(9600);

  initMp3Player();
  
  pinMode(explodePin, OUTPUT);
  digitalWrite(explodePin, HIGH);

  pinMode(doorPin, OUTPUT); 
  digitalWrite(doorPin, HIGH);

  pinMode(winSoundPin, OUTPUT);
  digitalWrite(winSoundPin, HIGH);


  pinMode(activationPin, INPUT);

  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, HIGH);
  closeCover();
  closeKey();


  counting = true;
  // init lift
  
  pinMode (SPEEDIN, OUTPUT); 
  pinMode (MOTOR_A, OUTPUT);
  pinMode (MOTOR_B, OUTPUT);

  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonStrartPin, INPUT);

  startBtnBounce.attach(buttonStrartPin);
   startBtnBounce.interval(10); //

   
   downButtonBounce.attach(buttonDown);
   downButtonBounce.interval(10); //
   
   upButtonBounce.attach(buttonUp);
   upButtonBounce.interval(10); //

   activationBounce.attach(activationPin);
   activationBounce.interval(10); //

   

   
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
   keypad.setDebounceTime(20);
   pinMode(buzzer, OUTPUT);
   analogWrite (buzzer, 255);
   delay(50);

   pinMode(relayBox, OUTPUT);     
   digitalWrite(relayBox, LOW);
   pinMode(clockPin, OUTPUT);
   pinMode(dataPin, OUTPUT);
   pinMode(latchPin, OUTPUT);
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
  sendMp3Command(CMD_SET_VOLUME, 0X16);//установить громкость в x1E=30 (100%)

  
  //sendCommand(0X17, 0X0102);//запустить зацикленное проигрывание содержимого папки "/01"
  //sendCommand(CMD_SINGLE_CYCLE, SINGLE_CYCLE_OFF);//запустить зацикленное проигрывание содержимого папки "/01"
  int songCode = word(0x01,1);
  sendMp3Command(0X0F, songCode);// играем трек 001 из папки 01
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

void playGameStartSound(){
 playMp3Sound(word(0x01, 0x01));
 delay(5000);
 sendMp3Command(CMD_STOP, 0x00);
}

void playDefusedSound(){
 playMp3Sound(word(0x01, 0x02));
}

void playExplosionSound(){
 playMp3Sound(word(0x01, 0x03));
}

void playLoserMusic(){
 playMp3Sound(word(0x01, 0x04));
}

void playWinMusic(){
 playMp3Sound(word(0x01, 0x05));
}

void playMp3Sound(int songCode){
  sendMp3Command(CMD_STOP, 0x00);//select the TF card  
  sendMp3Command(0X0F, songCode);// играем трек 001 из папки 01 
}

/*
void playGameStartSound(){
 
  sendMp3Command(CMD_STOP, 0x00);//select the TF card  

  int songCode = word(0x01, 0x01);
  sendMp3Command(0X0F, songCode);// играем трек 001 из папки 01 
}
*/

void closeCover(){
 coverOpened = false;
 openServo.attach(openCoverPin);
 openServo.write(0);
 delay(1000);
 openServo.detach();  
}

void openCover(){
 coverOpened = true;
 openServo.attach(openCoverPin);
 openServo.write(180);
 delay(1000);
 openServo.detach();  
}


void closeKey(){
 coverOpened = false;
 openServo.attach(keyPin);
 openServo.write(0);
 delay(1000);
 openServo.detach();  
}

void openKey(){
 coverOpened = true;
 openServo.attach(keyPin);
 openServo.write(180);
 delay(1000);
 openServo.detach();  
}

void initLCDKeys()
{
  for (int i = 0; i < sizeof(rowPins); i++)
    pinMode(rowPins[i],OUTPUT);
  for (int i = 0; i < sizeof(colPins); i++)
  {
    pinMode(colPins[i],INPUT);
    digitalWrite(colPins[i],LOW);
  }
}


void startGame(){

  Serial.print("canStart:");
  Serial.println(canStart);
  
  if(!canStart && !isLost){
    return;
  }


  
  Serial.println("GO");
  canStart = false;
  defuseOptions[1][0] = 0;
  defuseOptions[1][1] = 0;
  defuseOptions[1][2] = 0;
  defuseOptions[1][3] = 0;
  
  bombActivated = false;
  onUpActivated = false;
  preUpActivatedSeconds = 0;
  onUpPreActivated = false;
  
  subDefuseTime = 0;
  newDefuseTime = 0;
  
  isBombDefused = false;
  counting = true;
  gameAcitvated = true;

  
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

  playGameStartSound();
  
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
  startBtnBounce.update();
  upButtonBounce.update();
  downButtonBounce.update();
  activationBounce.update();
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
    int wireIndex = defuseOptions[0][i];
    Bounce tmpBounce = getBounceByIndex(defuseOptions[0][i]);
    //Serial.println(tmpBounce);
    //tmpBounce.update();
    int isWireOn = tmpBounce.read();
   
    // wire already cut
    if(defuseOptions[1][i]){
      isWireOn = false;
    }
    
    if(!isWireOn){
      // wire was not cut before
      if(defuseOptions[1][i] < defuseCounterThrashold ){
        defuseOptions[1][i] = defuseOptions[1][i] + 1;
        
        if(defuseOptions[1][i] >= defuseCounterThrashold){
          Serial.print(wireIndex);
          Serial.print(" ");
          if(i == 1){
            Serial.print("BLUE WIRE");
          }
          else if(i == 2){
            Serial.print("WHITE WIRE");
          }
          else if(i == 3){
            Serial.print("GREEN WIRE");
          }
          else if(i == 4){
            Serial.print("ORANGE WIRE");
          }
          

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

          // preventing
          cuts_per_session++;
          delay(100);
          wireHasBeenCut = true;
        }  
        //break;
      }
      else{
        if(i == wireCutIndex){
          wireCutIndex ++;
        }
      }
    }
  } // END FOR
  
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
}
}

void doWin(){
  Serial.print("doWIN");
  if(winDone){
    return;
  }
  
  winDone = true;
  /*
  digitalWrite(winSoundPin, LOW);
  delay(15000); // playing 15 sec 
  digitalWrite(winSoundPin, HIGH);
  */


  playDefusedSound();
  openKey();
  delay(5000);
  playWinMusic();
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


  //digitalWrite(ssPin, LOW);  
  isLost = true;
  
  // sound
  int beepDelay = 150;
  int beepsCount = 20;
  for(int i = 0 ; i < beepsCount; i++){

    Serial.println("beep");
    beep(beepDelay - i * 7);
  }

  //digitalWrite(explodePin, LOW); 
  
  //digitalWrite(doorPin, HIGH); 
  //explodeServo.attach(explodeServoPin);
  //explodeServo.write(0);
  // let smoke go out for 3 sec
  playExplosionSound();
  movingUp = false;
  openKey();
  delay(explosionTime);
  playLoserMusic();
  //digitalWrite(explodePin, HIGH); 

}


void stopGame(){
  stopMp3();
  gameAcitvated = false;
  isLost = false;
  isWin = false;
  preUpActivatedSeconds = 0;
  digitalWrite(doorPin, HIGH); 
  digitalWrite(winSoundPin, HIGH);
}
/* --------------------------------------- BEGIN ------------------------------------*/
void loop()
{
  updateButtonsBounces(); 
  keyPass();
  //anturag();
  // ЕСЛИ кнопка START нажата тогда активировать airsoft bomb
  if(startBtnBounce.read()){
    Serial.print("button presed START:");
    Serial.println(startBtnBounce.read());  
    delay(250);
    startGame();  
  }
  
  if(!gameAcitvated){

   return;
 }

   //Serial.println(onUpActivated);

 if(isLost){
  explode();
  Serial.println("LOST");

}
else if (!isWin){
    // Проверка проводов
   //  Serial.println("GAME");
  checkWires();
  count();
} 

if(isLost || isWin) {
    //Serial.println("lost or win");
    //digitalWrite(speakerPin, LOW);

    //noTone(speakerPin);
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
    
    //activationButtonState = digitalRead(activationPin);
    activationButtonState = activationBounce.rose();

    
    if( ( secondsLeft < secondsLeftForLiftingUp || activationButtonState) && !bombActivated){// !liftedUp && notLiftedYet ){
      movingUp = true;
      Serial.println("activation");
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

void keyPass(){
  char key = keypad.getKey();
  initLCDKeys();
  //noTone(buzzer);
  delay(50);
  if (key != NO_KEY){
    Serial.println(key);
    if (key == 'F')
    {
    } else
    Serial.print(key);
      //playTone();
    beep(keyBeepDelay);

    if (key == password[position]){
      Serial.print("Pass:");
      Serial.print(password[position]);
      guessed_digit++;
    } 

    position++;
    Serial.print(" position");
    Serial.println(position);

    if (position >= password_len){
     Serial.print("== 5 ==");
     if(guessed_digit == password_len){
      setLocked();
    } else {
      soundWrongSignal();
    }
    
    position = 0;
    guessed_digit = 0;
        //
  }
}

}// END

void setLocked(){
 Serial.print("UnLocked!");
 digitalWrite(relayBox, HIGH);

 delay(100);
 beep(keyBeepDelay);
 delay(100);
 beep(keyBeepDelay);
 delay(100);
 beep(keyBeepDelay);
 delay(100);
 beep(keyBeepDelay);
 delay(100);

 if(!coverOpened){
   openCover();
   if(gameAcitvated){
     onUpActivated = true;
   }
 }
 else{
   closeCover();
   closeKey();

   if(isWin || isLost){
    stopGame();  
  }
}
delay (5000);
  //digitalWrite(relayBox, LOW);

}


void soundWrongSignal(){
  //Serial.println("wrong");
  analogWrite (buzzer, 0);
  delay (400);
  analogWrite (buzzer, 255);
  delay (400);
}



//Beeps on key presses
void playTone(){
  tone(buzzer, 150, 10);
}

void anturag(){
  for (int number = 0; number < 256; number++) {
    Serial.println(number);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, number);
    digitalWrite(latchPin, HIGH);
    delay(500);
  }
}
