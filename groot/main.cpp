#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 8
#define DIO 9

int blobPin = 7;
int countBlob = 0;
bool isBlobStatus = false;
int numberKey = 1960;
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
TM1637Display display(CLK, DIO);

// пороговое значение настраивается тут:
unsigned long sc_start_blob = 0;


void setup() {
  Serial.begin(9600);
  display.setBrightness(0x0a, false);
  pinMode(blobPin, INPUT);

}

void loop() {
  int blobState = digitalRead(blobPin);

  if (( blobState == 0 )&&( isBlobStatus == false )) {
    countBlob =  countBlob + 1;
    Serial.print("blob detection: ");
    Serial.println(countBlob);
    isBlobStatus = true;
    // ---

    display.clear();
    display.setBrightness(7, true);
    display.showNumberDec(numberKey);
    sc_start_blob = millis();
  }

  if ( ( millis() - sc_start_blob > 3300 )&&( isBlobStatus == true )) {
    isBlobStatus = false;
    display.setBrightness(7, false);
    display.showNumberDec(data);
  }

  Serial.println(blobState);

  delay(200);
}
