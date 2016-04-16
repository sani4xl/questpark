
#include <SPI.h> // Include the Arduino SPI library

const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin1 = 9; 
const int ledPin2 = 10; 

const int speakerPin = 3;

/**/
/* Possible start point for counting down */
const int ssPin = 8;
int countdownOptions[] = {5, 15, 60, 45}; //Possible values to count down from
int countdownSetting = 2;                //Default countdown-value is 25 minutes (Pomodoro Technique)
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
  
  pinMode(buttonPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  
  initCable();
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
  counting = true; 
  if (!isWin){
    evaluateButton();
    count();
  } else {
    digitalWrite (ledPin2, LOW);
    digitalWrite (ledPin1, LOW);
    digitalWrite(speakerPin, LOW);
    noTone(speakerPin);
  }
}
/**/
void initCable() {
    Serial.println(" --- INIT cabel --- ");
   for (int i=0; i<4; i++){
      pinMode(cablePin[i], INPUT); 
     // digitalWrite (cablePin[i], HIGH);
     Serial.println(cablePin[i]);
  }
  Serial.println(" ------ ");
}
/**/
void count() {

  if (counting){
    unsigned long now = millis();
    int secondsPassed = (now-lastStartTime)/1000;
    int secondsDisplay = ((countdownOptions[countdownSetting] * 60) - secondsPassed) % 60;
    int minutesDisplay = (((countdownOptions[countdownSetting] - newDefuseTime) * 60) - secondsPassed) / 60;
    
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
  
//
void evaluateButton() {
   
  for (int n=0; n<4; n++){
  //Проверяем все кабеля на разрыв   
  if ((digitalRead(cablePin[n]) == 0)&&(defuseOptions[1][n] != 1)&&(!isWin)) {  
        Serial.print("n:");   Serial.println(n);
        Serial.print("def:"); Serial.println(defuseOptions[0][countDef]);
        Serial.print("cab:"); Serial.println(cablePin[n]);
        
        //Узнать последовательность

        if (cablePin[n] != defuseOptions[0][countDef] ){
          newDefuseTime += 10;
          timerC(); 
        } else {
          countDef++;
          isGoodDef = true;
          Serial.println("isGoodDef!!!");
          Serial.println(countDef);
        }
        
        if((( countDef*10 ) + newDefuseTime) == 40) { isWin = true; Serial.println("isWin");}
        
        Serial.print("newTime +10");   
        Serial.println(newDefuseTime);

        //------------------------------------------
        defuseOptions[1][n] = 1;
        delay(1000);
  } // if end ((digitalRead(cablePin[n]) == 0)&&(defuseOptions[1][n] != 1))
  } // for end N
//-------------------------------------------------------------------------
//                1 +10 +10 
//  0  1  2  3
// {4, 5, 6, 7} - cablePin[]
// {4, 7, 5, 6} - defuseOptions[2][4]
//
//-------------------------------------------------------------------------

} //evaluateButton

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


