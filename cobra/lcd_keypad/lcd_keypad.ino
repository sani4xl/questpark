#include <LiquidCrystal.h>

#include <Keypad.h>

#define buzzer 12

//char password[4] = "9146";
char* password = "9146";
int password_len = 4;
int position = 0;
int guessed_digit = 0;

LiquidCrystal  lcd (11,10,6,7,8,9);

int relay = 13;  

const byte rows = 4;
const byte cols = 4;
const char keys [rows][cols] =
{
  {'1','2','3','F'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[rows] = {6,7,8,9}; //connect to the row pinouts of the keypad
byte colPins[cols] = {2,3,4,5}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.noAutoscroll();
  lcd.print("QUESTPARK COBRA");
  lcd.setCursor(0,1);
  lcd.print("Enter>");
  delay(2000);
  keypad.setDebounceTime(20);
  pinMode(buzzer, OUTPUT);
  analogWrite (buzzer, 255);
  delay(50);
  
  pinMode(relay, OUTPUT);     
  digitalWrite(relay, LOW);
 
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

void loop(){

  char key = keypad.getKey();
  initLCDKeys();
  //noTone(buzzer);
  delay(50);
  if (key != NO_KEY){
    Serial.println(key);
    if (key == 'F')
    {
      clrDisplay();
    } else
      lcd.print(key);
      //playTone();
      beep();
      
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
          clrDisplay();
        } else {
          soundWrongSignal();
          clrDisplay();
        }
    
        position = 0;
        guessed_digit = 0;
        //
      }
  }

  
}

void clrDisplay(){
  lcd.setCursor(6,1);
  lcd.write("          ");
  lcd.setCursor(6,1);
}

void setLocked(){
 Serial.print("UnLocked!");
 clrDisplay();
 lcd.print("UnLocked!");
  digitalWrite(relay, HIGH);
  
  delay(100);
  beep();
  delay(100);
  beep();
  delay(100);
  beep();
  delay(100);
  beep();
  delay(100);
  delay (5000);
  digitalWrite(relay, LOW);
  
}


void soundWrongSignal(){
  //Serial.println("wrong");
  analogWrite (buzzer, 0);
  delay (400);
  analogWrite (buzzer, 255);
  delay (400);
}

void beep()
{
  analogWrite (buzzer, 0);
  delay (30);
  analogWrite (buzzer, 255);
  delay(30);
}

//Beeps on key presses
void playTone(){
  tone(buzzer, 150, 10);
  }
