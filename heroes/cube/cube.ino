#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec, uidDecTemp;  // для храниения номера метки в десятичном формате
const int relay = 2; // PIN D2 --> RELAY

long previousMillis = 0;
long interval = 45;

const int LED_3 = 3;
const int LED_5 = 5;
const int LED_6 = 6;

int brightness_1 = 0; // яркость светодиода
int brightness_2 = 100; // яркость светодиода
int brightness_3 = 50; // яркость светодиода
int fadeAmount = 5; // на сколько увеличить яркость светодиода
int fadeAmount_2 = 5;
int fadeAmount_3 = 5;

  
void setup() {
  Serial.begin(9600);
  Serial.println("Waiting for card...");
  SPI.begin();
  mfrc522.PCD_Init();     // Init MFRC522 card.
  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH); // OFF relay  

  pinMode(LED_3, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);
}


void loop() {
  unsigned long currentMillis = millis();
  //1
  analogWrite(LED_3, brightness_1);
  //2
  analogWrite(LED_5, brightness_2);
  //3
  analogWrite(LED_6, brightness_3);
  
  //проверяем не прошел ли нужный интервал, если прошел то
  if(currentMillis - previousMillis > interval) {
    // сохраняем время последнего переключения
    previousMillis = currentMillis; 
    brightness_1 = brightness_1 + fadeAmount;
    brightness_2 = brightness_2 + fadeAmount_2;
    brightness_3 = brightness_3 + fadeAmount_3;
    
    if (brightness_1 == 0 || brightness_1 == 220) {
      fadeAmount = -fadeAmount;
    }

    if (brightness_2 == 0 || brightness_2 == 220) {
      fadeAmount_2 = -fadeAmount_2;
    }

    if (brightness_3 == 0 || brightness_3 == 220) {
      fadeAmount_3 = -fadeAmount_3;
    }
    
  } // delay $interval
 
  // Поиск новой метки
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Выбор метки
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  uidDec = 0;
  // Выдача серийного номера метки.
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }
  Serial.println("Card UID: ");
  Serial.println(uidDec); // Выводим UID метки в консоль.

  if ((uidDec == 444860938) || (uidDec == 3698081641) || (uidDec == 3603377946)) // Сравниваем Uid метки, ЕСЛИ он равен заданому ТО реле вкл.
  {
    digitalWrite(relay,LOW);
    brightness_1 = 0;
    brightness_2 = 0;
    brightness_3 = 0;
    analogWrite(LED_3, brightness_1);
    analogWrite(LED_5, brightness_2);
    analogWrite(LED_6, brightness_3);
     
    delay(10000); // пауза 3 сек и реле выкл.
    digitalWrite(relay,HIGH);
  }
}
