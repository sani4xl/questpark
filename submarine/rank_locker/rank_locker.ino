/*
 * изделие №5
 * запаять D2-D12
 * D2 - clk screen
 * D3 - dio screen
 * D4 - rele
 * D5-D12 - пины на клаве слева на право (вид сверху)
 * 4 slota GND
 * 4 slota 5V
 */
#include <TM1637Display.h>

#include <Keypad.h>
#include <Servo.h>
String password = "7194";
int password_len = 4;

int relePin = 4;

String enteredPassword = "";

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 8, 7, 6, 5};
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 12, 11, 10, 9}; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// screen
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);
char mainScreenBuffer[4];

int buttonState = 0;

void setup()
{
  
  pinMode (relePin, OUTPUT);
  digitalWrite(relePin, LOW);

  setLocked(true);
  enteredPassword = "";
  Serial.begin(9600);
  clearDisplay();
}

void clearDisplay() {
  uint8_t data[] = { 0x00, 0x00, 0x0, 0x00 };
  display.setBrightness(0x0f);
  display.setSegments(data);
  display.clear();
}

void displayData(String pass) {
  uint8_t data[] = { 0x00, 0x00, 0x0, 0x00 };
  display.setBrightness(0x0f);
  display.clear();

  // Selectively set different digits
  int len = pass.length();
  if (len > 0) {
    data[0] = display.encodeDigit(pass[0] - '0');
  }

  if (len > 1) {
    data[1] = display.encodeDigit(pass[1] - '0');
  }

  if (len > 2) {
    data[2] = display.encodeDigit(pass[2] - '0');
  }

  if (len > 3) {
    data[3] = display.encodeDigit(pass[3] - '0');
  }

  display.setSegments(data);
}

void reset() {
  enteredPassword = "";
  setLocked(true);
  clearDisplay();
}

void loop() {
  
  char key = kpd.getKey();
  
  if (key == '*' || key == '#') {
    reset();
  }
  else if(key) {
    enteredPassword += key;

    displayData(enteredPassword);
  
    if (password == enteredPassword) {
      for (int i = 9; i >= 0; i--) {
        displayData(String(i));
        delay(200);
      }
      setLocked(false);
      delay(10000);
    } else if (enteredPassword.length() >= 4) {
      for (int i = 0; i < 6; i++) {
        delay(120);
        displayData("0000");
      }
      reset();
    }
  
}

delay(100);
}

void setLocked(boolean locked){
 
 if (locked){
   digitalWrite(relePin, LOW);
 }
 else{
  digitalWrite(relePin, HIGH);
 }
 
}
