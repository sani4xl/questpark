#include <Bounce2.h>

#include "IRremote.h"
#include "CppList.h"

#include <SPI.h>
#include <Servo.h>
#include <Wire.h>

// CLASS BEGIN
class Flasher
{
    int ledPin; // номер пина со светодиодом
    long OnTime; // время включения в миллисекундах
    long OffTime; // время, когда светодиод выключен

    // Текущее состояние
    int ledState; // состояние ВКЛ/ВЫКЛ
    unsigned long previousMillis; // последний момент смены состояния

  public:
    Flasher(int pin, long on, long off)
    {
      ledPin = pin;
      pinMode(ledPin, OUTPUT);

      OnTime = on;
      OffTime = off;

      ledState = HIGH;
      previousMillis = 0;
      digitalWrite(ledPin, ledState);
    }

    void UpdateOFF()
    {
      digitalWrite(ledPin, LOW);
    }
    
    void UpdateON()
    {
      digitalWrite(ledPin, HIGH);
    }
    
    void Update()
    {

      digitalWrite(ledPin, LOW);
      delay(200);
      digitalWrite(ledPin, HIGH);

    }
};

// CLASS END
class Sweeper
{
    Servo servo; // сервопривод
    int pos; // текущее положение сервы
    int increment; // увеличиваем перемещение на каждом шаге
    int updateInterval; // промежуток времени между обновлениями
    unsigned long lastUpdate; // последнее обновление положения

  public:
    Sweeper(int interval)
    {
      updateInterval = interval;
      increment = 1;
    }

    void Attach(int pin)
    {
      servo.attach(pin);
    }

    void Detach()
    {
      servo.detach();
    }

    void Update(int var)
    {
      switch (var) {
        case 1:
          //         if((millis() - lastUpdate) > updateInterval) // время обновлять
          //            {
          //              lastUpdate = millis();
          //              pos += increment;
          //              servo.write(pos);
          //              Serial.print("POS:");
          //              Serial.println(pos);
          //
          //              if (pos >= 120) // конец вращения
          //                {
          //                // обратное направление
          //                //increment = -increment;
          //                //servo.detach();
          //                break;
          //                }
          //           }

          Serial.print("Hit - server 1");

          pos = 90;
          servo.write(pos);
          //delay(200);
          //Serial.print("POS:");
         // Serial.println(pos);

          break;
        case 2:
          //Serial.print("Hit - server 2");
          pos = random(95,120); 
          
//          if (pos >= 100) {
//            pos = 93;
//            //delay(50);
//          } else {
//            pos = 110;
//          } // end IF
          
          servo.write(pos);
          delay(200);
          //Serial.print("POS:");
          //Serial.println(pos);
          break;
        case 3:
          //Serial.print("Hit - server 3");
          pos = 0;
          servo.write(pos);
          delay(200);
          //Serial.print("POS:");
         // Serial.println(pos);
          break;
        case 4:
          //Serial.print("Hit - server 3");
          pos = 160;
          servo.write(pos);
          delay(200);
          //Serial.print("POS:");
          //Serial.println(pos);
          break;


      }
    }

};
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Flasher ledBot1(48, 100, 100);
Flasher ledBot2(46, 100, 100);

Sweeper weapon_1(30);
Sweeper weapon_2(30);

Sweeper keyBox(10);

//
//bool _initialized = false;
// Instantiate a Bounce object
Bounce debouncer = Bounce(); 

int rcv_count;
IRrecv **all_rcv;

long randNumber;

int firstBot = 0;
int secondBot = 1;
int health[] = {100, 100};
boolean isDied[] = {0, 0};
boolean isOFF[] = {0, 0};
boolean isWIN = false;
/* s7s */
const int ssPin = 53;
const int ssPin2 = 49;
String strOne;

//------------------------------------- Box-Step1
int ledRest = 8;
int btnRest = 7;
boolean isBoxOpen = false;
int btnBoxPress = 77;
boolean isWeaponStart = false;
boolean isIrStart = false;
boolean isRest = false;
boolean isStart = false;

int roomLightRelay = 10;
int MagRelay = 9;
int relayStartBtn = 30;  //24
boolean isMag = false;
boolean isRelayStart = false;
boolean isRelayRestart = false;
boolean isR1 = false;
boolean isR2 = false;
boolean isKeyBox = false;

int count =0;
long previousMillis = 0; //Время последнего нажатия кнопки

int relayReserveBtn = 39;                                    // ---- > RESERVE

// ------------------------------------ RELAY
int relay = 26;                  //   ???
int relay_2 = 28;                //   ???

int relay_sound = 40;                //   ???

unsigned long sc_start_relay = 0;
unsigned long sc_start_mag = 0;
unsigned long sc_start_key = 0;
unsigned long sc_start_weapon = 0;
unsigned long sc_start_ir = 0;

boolean isTRelay = false;

int x = 0;

boolean isTime = false;
boolean isBtnUnk = false;
unsigned long sc_start=0;

Bounce debouncer1 = Bounce(); 

Bounce debouncer2 = Bounce(); 

Bounce debouncer3 = Bounce(); 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void setup() {
  //if (_initialized) return;
  
  Serial.begin(9600);
 // Wire.begin(8);                // join i2c bus with address #8
 // Wire.onReceive(receiveEvent); // register event
  pinMode(MagRelay, OUTPUT);
  digitalWrite(MagRelay, HIGH);
  
  pinMode(roomLightRelay, OUTPUT);
  digitalWrite(roomLightRelay, HIGH);
  
  // Box - #1
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  pinMode(relay_2, OUTPUT);
  digitalWrite(relay_2, HIGH);

  pinMode(relay_sound, OUTPUT);
  digitalWrite(relay_sound, HIGH);

  pinMode(ledRest,OUTPUT);
  digitalWrite(ledRest, LOW);
   
  pinMode(btnRest,INPUT);
  debouncer3.attach(btnRest);
  debouncer3.interval(5); // interval in ms
  
  rcv_count = 4;
  all_rcv = (IRrecv **)malloc(rcv_count*sizeof(int));
  
  all_rcv[0] = new IRrecv(35);
  all_rcv[1] = new IRrecv(37);
  all_rcv[2] = new IRrecv(34);
  all_rcv[3] = new IRrecv(36);
  
  for (int i=0; i<rcv_count; ++i){
    all_rcv[i]->enableIRIn();
  }
  
  //_initialized = true;
  pinMode(relayStartBtn, INPUT);
  debouncer1.attach(relayStartBtn);
  debouncer1.interval(5); // interval in ms
  
  // ---
  pinMode(relayReserveBtn, INPUT_PULLUP);
  debouncer2.attach(relayReserveBtn);
  debouncer2.interval(5); // interval in ms
  
  

  // -------- SPI initialization
  pinMode(ssPin, OUTPUT);  // Set the SS pin as an output
  //digitalWrite(ssPin, HIGH);  // Set the SS pin HIGH

  pinMode(ssPin2, OUTPUT);  // Set the SS pin as an output
  //digitalWrite(ssPin2, LOW);  // Set the SS pin HIGH
  
  keyBox.Attach(45);
  keyBox.Update(1);
  keyBox.Detach();
  SPI.begin();  // Begin SPI hardware
  SPI.setClockDivider(SPI_CLOCK_DIV64);  // Slow down SPI clock
  // --------
  clearDisplaySPI();
  s7sSendStringSPI_1((String)health[firstBot] + "--");
  s7sSendStringSPI_2((String)health[secondBot] + "--");

//    weapon_1.Attach(43);
//    weapon_1.Update(1);
//   weapon_1.Attach(41);
ledBot1.UpdateOFF();
ledBot2.UpdateOFF();
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
  
}


void loop() {

//    weapon_1.Update(1);
//    weapon_1.Update(2);
  
  //****************************************
  //*   Открываем Box: срабативают вверхние замки на удержание 5 сек.
  //*   через 1 сек. срабативаю маникены на открытия
  //****************************************
  //delay(200);
  // Update the Bounce instance :
  
  debouncer1.update();  // START
  debouncer2.update();  // RESERVE
  debouncer3.update();  // RESTART

  
  int value = debouncer1.read();
  if ( value == HIGH ) {
    if ((isWIN == false ) && (isStart == false)) {
      Serial.println(" ------------------------------------------------------> START ");
       previousMillis = millis();
       isRelayStart = true;
       isStart = true;
       sc_start_mag = millis();
     }
  } 
   
  //-------------------------------------------------   BOX   ---------------------------------------------------- 
  //  isRelayStart --> TRUE isBoxOpen -- > FALSE
  
  //if ((btnBoxPress == x) && (!isBoxOpen)) {
     
    // RESERVE
    int value2 = debouncer2.read();
    
    if ((value2 == HIGH)) {   
      //if (digitalRead(relayReserveBtn) == HIGH){
      // включаем    
      Serial.println("RESERVE");
      digitalWrite(relay, LOW);
      delay(300);
    
      digitalWrite(relay_2, LOW);
      digitalWrite(relay, HIGH);  // Отключить Реле
      delay(300);
    
      digitalWrite(relay_2, HIGH); // Отключить Реле

      
      init2();
      
      isRelayStart = true;
      isStart = true;
      sc_start_mag = millis();
      
//      if (!isStart) {
//         isRelayStart = true;
//         isStart = true;
//      } // ---
      
    }

    
    // END RESERVE
    
    // ----
//  int value3 = debouncer3.read();  
//  //if ((value3 == HIGH)&&(isRest)) {   
//  
//  if ((value3 == HIGH)) {   
//  
//  //  if (digitalRead(btnRest) == HIGH){
//   Serial.print("R-Btn");
//   digitalWrite(ledRest, HIGH);
//   delay(60);
//    
//   //
//   Serial.println("----------------------------------------------------------    RESTART");
//   isRelayStart = false;
//   isBoxOpen = false;
//   // ---------------------
//   isMag = false;
//   isTRelay = false;
//   isR1 = false;
//   isR2 = false;
//   // ---------------------
//   isWeaponStart = false;
//   isIrStart = false;
//   // ---------------------   
//   init2();
//   // ---------------------   
//   isWIN = false;
//   isStart = false;    
//
//  }// END REST
//  
  
    // ####################################  RELAY  ###########################
     
    if ((isRelayStart)&& (!isBoxOpen)) { 
    
      
    //      
    if (!isMag) {
      Serial.println("roomLight -------------------------> ON-roomLight-ON");
      digitalWrite(roomLightRelay, LOW);
      
      Serial.println("MAG -------------------------> ON-MAG-ON");
      digitalWrite(MagRelay, LOW);
      if ((millis() - sc_start_mag > 1000)) {
        Serial.println("-----------------------------------------------------------> PIKACHU");
        isMag = true;
        isTRelay = true;
      }
      
    }
    
    if (isTRelay) {
      sc_start_relay = millis(); // замеряем время до начала подключения
      isTRelay = false;
      Serial.print(sc_start_relay);
      Serial.println("Relay -------------------------> ON : 1");  // Включить Реле
      digitalWrite(relay, LOW);
      isR1 = true;
    }
    
    if ((millis() - sc_start_relay > 500) && (isR1)) {
      Serial.println("Relay -------------------------> OFF : 1");    
      digitalWrite(relay, HIGH);  // Отключить Реле
      Serial.println("Relay -------------------------> ON : 2");    
      digitalWrite(relay_2, LOW);
      isR1 = false; // OFF if
      isR2 = true; // STRAT if
      
      sc_start_relay = millis();
    }
    
    if ((millis() - sc_start_relay > 500) && (isR2)) {
      Serial.println("Relay -------------------------> OFF : 2");    
      digitalWrite(relay_2, HIGH); // Отключить Реле
      Serial.println("MAG -------------------------> OFF-MAG-OFF");
      digitalWrite(MagRelay, HIGH);
      
      isR2 = false; // OFF if
      isBoxOpen = true;
      isWeaponStart = true;
      isRelayRestart = true;
      isRelayStart = false;
      sc_start_weapon = millis();
      sc_start_ir = millis();
    }
    
  } //END -IF BoxBtn
  
  // ---------------------------------------------  WEAPON ------------------------------------------------------------
  // isWeaponStart   -- > TRUE
  
  if (isWeaponStart) {
    if (millis() - sc_start_weapon > 1000) {
    
      Serial.print("isWeaponStart : ");
      Serial.println(isWeaponStart);
      
      ledBot1.UpdateON();
      ledBot2.UpdateON();

      clearDisplaySPI();
      s7sSendStringSPI_1((String)health[firstBot] + "--");
      s7sSendStringSPI_2((String)health[secondBot] + "--");
      
      weapon_1.Attach(43);
      weapon_2.Attach(41);

      weapon_1.Update(1);
      weapon_2.Update(1);
      isWeaponStart = false;
      digitalWrite(relay_sound, LOW);
      isIrStart = true;
    
      sc_start = millis();
      // CLOSE     
  
    }
  }// isWeaponStart
  
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// ---------------------------------
// ---------------------------------
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// ------------------------------------------ isIrStart ------------------------------------------------------------------------------
//   isIrStart --> TRUE
  
  if (isIrStart) {
   // Serial.print("+++++++++++ IrStart +++++++ ");
  if (millis() - sc_start_ir > 1400) {
      
    
  for (int i=0; i<rcv_count; ++i){
    decode_results results;
    if (all_rcv[i]->decode(&results)) {
      int btn = DecodeButton(results.value);
//      Serial.print("Rcv_");
//      Serial.print(i);
//      Serial.print(":");
//      Serial.println(btn);
//      Serial.println(results.value, HEX);
//      // BTN_UNKNOWN = 101
      
      
      if (btn == 101) { 
//          Serial.println(" ---  FAIL  ---");
          if ((millis()- sc_start > 1200)) {
           sc_start = millis();
           isBtnUnk = true;
          }
      
      }
      if ((isBtnUnk)||(btn == 111)||(btn == 222)||(btn == 333)||(btn == 444)) { 
      isBtnUnk = false;
      // BOT #1
      if ( ((i == 0)||(i == 1)) && (!isDied[firstBot]) ){
//          Serial.println("BOT -1- SHOOT");
         // 
          ledBot1.Update();
          
          randNumber = random(36,56);
          //clearDisplaySPI(); 
          shotBot(randNumber, firstBot);

          
          //weapon_1.Update(2);
          s7sSendStringSPI_1((String)health[firstBot] + "--");
          if (isDied[firstBot]) {
     //         Serial.println("Bot #1 Died");
              weapon_1.Update(4);
              ledBot1.UpdateOFF();
              s7sSendStringSPI_1("----");
          }

      }
      // BOT #2
      if ( ((i == 2)||(i == 3)) && (!isDied[secondBot])) {
//          Serial.println("BOT -2- SHOOT");
          
          
          ledBot2.Update();
          
          randNumber = random(45,55);
          //clearDisplaySPI();
          shotBot(randNumber, secondBot);
          
          //weapon_2.Update(2);
          s7sSendStringSPI_2((String)health[secondBot] + "--");
          if (isDied[secondBot]) {
         //     Serial.println("Bot #2 Died");
              weapon_2.Update(4);
              ledBot2.UpdateOFF();
              s7sSendStringSPI_2("----");
          }
      }
      //
      }  // ---- END btn
      //
      all_rcv[i]->resume();
    }
  }
  // ----
    if (!isDied[firstBot]){
   //   Serial.print("isD#1");
      weapon_1.Update(2);
     // weapon_1.Detach();
    }

  if (!isDied[secondBot]){
     // Serial.print("isD#2");
      weapon_2.Update(2);
      //weapon_2.Detach();
    }

  } // --- sc_start_ir
    
  } // --- isIrStart
  //
  
  if ((isDied[firstBot]) && (isDied[secondBot]) && (!isWIN)) {
    Serial.println("WIN");
    digitalWrite(relay_sound, HIGH);
    //isStartRest = false;
    isWIN = true;
    isIrStart = false;
  //  isRest = true;
    digitalWrite(ledRest, LOW);
    keyBox.Attach(45);
    keyBox.Update(3);
    sc_start_key = millis();
    weapon_1.Detach();
    weapon_2.Detach();
    
    isKeyBox = true;
    Serial.println("roomLight -------------------------> OFF-roomLight-OFF");
    digitalWrite(roomLightRelay, HIGH);
    
   }
  
  if (((millis() - sc_start_key > 7000) && (millis() - sc_start_key < 10000))&&(isKeyBox)) {
     keyBox.Attach(45);
     keyBox.Update(1);
     delay(120);
     keyBox.Detach();
     isKeyBox = false;
     init2();
  }
    
  


  //
}

//
void shotBot(int hit , int bot)
{
 // Serial.println(' --- HIT --- ');
  health[bot] = health[bot] - hit;
  
  if ((health[bot] <= 0) && (isDied[bot] == 0)) { 
    isDied[bot] = 1;
    isOFF[bot] = 1;
  }
}

//

const int BTN_0 = 111;
const int BTN_1 = 222;
const int BTN_2 = 333;
const int BTN_3 = 444;

const int BTN_REPEAT = 100;
const int BTN_UNKNOWN = 101;
const int BTN_ERROR = 102;
const int BTN_EMPTY = 0;

int DecodeButton(int param){
	int rez = BTN_UNKNOWN;
	switch (param){
	case 0x0: {
						rez = BTN_ERROR;
						break;
			  }
	case 0x00F0111:{
						rez = BTN_0;
						break;
					}
	case 0x00F0222:{
						rez = BTN_1;
						break;
					}
	case 0x00F0333:{
						rez = BTN_2;
						break;
					}
	case 0x00F0444:{
						rez = BTN_3;
						break;
					}
	case 0xFFFFFFFF:{
						rez = BTN_REPEAT;
						break;
					}
	}
	return rez;
}
/***************SPI LIB*********************/
void s7sSendStringSPI_1(String toSend)
{
  digitalWrite(ssPin, LOW);

  //Serial.println(toSend);
  for (int i = 0; i < 4; i++)
  {
    SPI.transfer((byte)toSend[i]);
  }

  digitalWrite(ssPin, HIGH);
}


void s7sSendStringSPI_2(String toSend)
{

  digitalWrite(ssPin2, LOW);

  //Serial.println(toSend);
  for (int i = 0; i < 4; i++)
  {
    SPI.transfer((byte)toSend[i]);
  }

  digitalWrite(ssPin2, HIGH);
}

void clearDisplaySPI()
{
  digitalWrite(ssPin, LOW);
  SPI.transfer(0x76);  // Clear display command
  digitalWrite(ssPin, HIGH);

  digitalWrite(ssPin2, LOW);
  SPI.transfer(0x76);  // Clear display command
  digitalWrite(ssPin2, HIGH);
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

void init2() {
  
   isRelayStart = false;
   isBoxOpen = false;
   // ---------------------
   isMag = false;
   isTRelay = false;
   isR1 = false;
   isR2 = false;
   // ---------------------
   isWeaponStart = false;
   isIrStart = false;
   // ---------------------   
   isWIN = false;
   isStart = false;    

//  ledBot1.UpdateON();
//  ledBot2.UpdateON();
   ledBot1.UpdateOFF();
   ledBot2.UpdateOFF();

  health[firstBot] = 100;
  health[secondBot] = 100;
  
  isDied[firstBot] = 0;
  isDied[secondBot] = 0;  
  
  clearDisplaySPI();
  
  weapon_1.Attach(43);
  weapon_2.Attach(41);
  weapon_1.Update(4);
  weapon_2.Update(4);
  
  
  keyBox.Attach(45);
  keyBox.Update(1);
  delay(120);
  keyBox.Detach();
  weapon_1.Detach();
  weapon_2.Detach();
 // s7sSendStringSPI_1((String)health[firstBot] + "--");
 // s7sSendStringSPI_2((String)health[secondBot] + "--");

}
