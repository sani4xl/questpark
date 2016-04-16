#include <SPI.h> // Include the Arduino SPI library


// lift stuff
int MOTOR_A = 30; // Input3 подключен к выводу 5 
int MOTOR_B = 32;
int SPEEDIN = 28;

const int buttonUp = 22; 
const int buttonDown = 24; 

const int stopperDown = 36; 
const int stopperUp = 34; 

int buttonUpState = 0;
int buttonDownState = 0;

int movingDown = false;
int movingUp = false;
int liftedUp = false;
int notLiftedYet = true;

int stopperDownState = false;
int stopperUpState = false;


int secondsLeftForLiftingUp = 30;// 60 * 4; // 4 minute

// bomb stuff
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin1 = 9; 
const int ledPin2 = 10; 

const int speakerPin = 3;

/**/
/* Possible start point for counting down */
const int ssPin = 8;
int countdownOptions[] = {1, 5, 15, 60, 45}; //Possible values to count down from
int countdownSetting = 0; 

int cablePin[] = {4, 5, 6, 7};
int defuseOptions[2][4] = {{4, 7, 5, 6},
                           {0, 0, 0, 0}}; 

boolean isGoodDef = false;
boolean isWin = false;
int countDef = 0;

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
boolean active = true;     //Is the clock active or suspended (active can be "not yet counting, but the button is pressed")
//

unsigned int counter = 0;  // This variable will count up to 65k
char tempString[10];  // Will be used with sprintf to create strings

void setup()
{
  
  Serial.begin(9600);
  
  // init lift
   pinMode (SPEEDIN, OUTPUT); 
   pinMode (MOTOR_A, OUTPUT);
   pinMode (MOTOR_B, OUTPUT);
 
   pinMode(buttonUp, INPUT);
   pinMode(buttonDown, INPUT);
   pinMode(stopperDown, INPUT);
   pinMode(stopperUp, INPUT);
  
  // init bomb
  pinMode(buttonPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  
  //initCable();
  // -------- SPI initialization
  pinMode(ssPin, OUTPUT);  // Set the SS pin as an output
  digitalWrite(ssPin, HIGH);  // Set the SS pin HIGH
  
  SPI.begin();  // Begin SPI hardware
  SPI.setClockDivider(SPI_CLOCK_DIV64);  // Slow down SPI clock
  // --------

  clearDisplaySPI();  
}


void loop()
{
//  buttonState = digitalRead(buttonPin);
//  if (buttonState == HIGH) {   
//     counting = true; 
//  }
  lifting();
  counting = true; 
  if (!isWin){
    //evaluateButton();
    count();
  } else {
    digitalWrite (ledPin2, LOW);
    digitalWrite (ledPin1, LOW);
    digitalWrite(speakerPin, LOW);
    //noTone(speakerPin);
  }
}

void lifting(){
  stopperDownState = digitalRead(stopperDown);
  if(stopperDownState){
     
  }
  else{
    Serial.print("down reached");  
  }
 
 stopperUpState = digitalRead(stopperUp);
  if(stopperUpState){
     //Serial.print("NO FORMAT");  
     //delay(100);
     liftedUp = false;
  } 
  else{
    liftedUp = true;
    Serial.print("up reached");  
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
    Serial.println("moving down");  
    
  // подаем на вывод ENB управляющий ШИМ сигнал 
  }
  else if(movingUp){
    digitalWrite (MOTOR_A, LOW);
    digitalWrite (MOTOR_B, HIGH);
    Serial.println("moving up");  
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

  if (counting){
    unsigned long now = millis();
    int secondsPassed = (now-lastStartTime)/1000;
    
    
    
    int lastMinute = countdownOptions[countdownSetting];
    int lastSecond = lastMinute * 60;
    
    int secondsLeft = lastSecond - secondsPassed;
    if(secondsLeft < secondsLeftForLiftingUp && !liftedUp && notLiftedYet ){
      movingUp = true;
      notLiftedYet = false;
    }
    //Serial.println(secondsLeft);  
    
    
    int secondsDisplay = (lastSecond - secondsPassed) % 60;
    int minutesDisplay = (((lastMinute - newDefuseTime) * 60) - secondsPassed) / 60;
    
    String currentTimeValue = "";
    // Serial.print(minutesDisplay);
    currentTimeValue += minutesDisplay;
    // Serial.print(":");
    if(secondsDisplay < 10) {
    //  Serial.print("0");
      currentTimeValue += "0";
    }
    currentTimeValue += secondsDisplay;
    //Вывод на дисплей
    displayNumber(currentTimeValue);
    countdownTime = currentTimeValue.toInt(); //Global Time
    
    if(secondsDisplay == 0 && minutesDisplay == 0) {
      counting = false;
      active = false;
    }
  } //counting
}// END count()



void timerC(){
  
  int var = 0;
  int cnt = 0;  
  
  while(var < 1000){
      cnt = countdownTime - var;
      displayNumber((String)cnt);
  var++;
 }
}
  //
  void displayNumber(String line)
{
   counter = line.toInt();
   sprintf(tempString, "%4d", counter);
   s7sSendStringSPI((String)tempString);   
   //beep(50);
   dec(counter);
   led(counter);   
  
 }
 
void dec(int isEvenNumber){
   if (isEvenNumber % 2 == 0) { setDecimalsSPI(0b00010000);} else{ setDecimalsSPI(0b00000000);}
}

void led(int isEvenNumber){
  if (isEvenNumber % 2 == 0) { digitalWrite (ledPin1, HIGH);digitalWrite (ledPin2, LOW);  tone (speakerPin, 777);
  } else{ digitalWrite (ledPin1, LOW);digitalWrite (ledPin2, HIGH);digitalWrite(speakerPin, LOW);noTone(speakerPin); }
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

  tone (speakerPin, 500); //включаем на 500 Гц
  delay(delayms); //ждем 100 Мс
  digitalWrite(speakerPin, LOW);
 
  
}

