
/* - -- --- ---- ----- Quest|PARK ----- ---- --- -- -
 * room: Marvel
 * name: Twister
 * v:    2.2.15
 *       @manjulife
 * - -- --- ---- ----- Quest|PARK ----- ---- --- -- -
*/

// --- led start
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define NUMPIXELS      20
#define INTERVAL       1000

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//int delayval = 60; // delay for half a second
// --- led end

// GAME SET
#define ARRAY_SIZE_X 4
#define ARRAY_SIZE_Y 5
#define WIN_POINTS 3

#define OFF   0
#define BLUE  1
#define GREEN 2
#define RED   3


//    1   2   3   4   5   6   7   8   9   10  11   12   13   14   15   16   17   18   19   20 
int keyPin_ledPoint[2][ARRAY_SIZE_X*ARRAY_SIZE_Y] = {
     {30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50,  52,  45,  47,  49,  51,  53,  33,  35,  37},
     {1,  2,  3,  4,  5,  10, 9,  8,  7,  6,  11,  12,  13,  14,  15,  20,  19,  18,  17,  16}
};

boolean fieldArray[ARRAY_SIZE_X][ARRAY_SIZE_Y];
int pointArray[WIN_POINTS];

// STAT
boolean isGame = false;
boolean isInit = false;
boolean isGameOver = false;
boolean isWIN = false;

unsigned long sc_start=0;

//long randNumber;
int iPosition = 0;
int count = 0;
int example = 0;

//

void testALL()
{
//  int i = 0;
//  while(i<20){
//     Serial.print(i+1);
//     Serial.print(":");
//     Serial.println(keyPin[i]);
//     if (digitalRead(keyPin[i]) == HIGH  ){i++;}
//    }
  }

void set_pointArray() {
    //Serial.println("--- SET pointArray ---");
    Serial.print("point >> ");
    example = random(1, (ARRAY_SIZE_X*ARRAY_SIZE_Y) + 1);
    
    for(int i=0; i<WIN_POINTS; i++){
      if (i==0){
         pointArray[i] = example;
      }else{
         pointArray[i] = Unic();
      }
      Serial.print(" ");
      Serial.print(pointArray[i]);
      Serial.print(" ");
      }
    
    Serial.println(" ");
    Serial.println("----------------------------------------");
  }
// Умная процедурка
int Unic(){
  int k=0;
  boolean res=true;
  int range = random(1, (ARRAY_SIZE_X*ARRAY_SIZE_Y) + 1);
//       Serial.print("  -");
//       Serial.print(range);
//       Serial.print("-  ");
  
  while(k<WIN_POINTS){
    if (pointArray[k] == range){      
        res=false;
//        Serial.print(" F ");
      }
    k++;
  }//
  if (!res){Unic();}else{return range;}

} 
  

void set_fieldArray() {
    count = 0;
    Serial.println("fieldArray >> ");
    Serial.println(" ");
    for(int i=0; i<ARRAY_SIZE_X; i++){
      for(int j=0; j<ARRAY_SIZE_Y; j++){
        count = count + 1;
        fieldArray[i][j] = 0;
        for(int n=0; n<WIN_POINTS; n++){
            if (pointArray[n] == count) {
             // Serial.print("<->");
              fieldArray[i][j] = count;
            }// if end;
        }

        
      // Serial.print(count);
         Serial.print(fieldArray[i][j]);
      }
      //Serial.println(" *|");
      Serial.println(" ");
    }
    Serial.println("----------------------------------------");
    Serial.println(" ");
  }
  
//---------------------------

void ledPoint(int item,int color) {
 

  // OFF ALL PIXEL
  if (color == 0){
      for (uint16_t i=0; i < pixels.numPixels(); i++) {
        pixels.setPixelColor(i, 0);        //turn every third pixel off
      }          
  }

  // BLUE           
  if (color == 1){
      pixels.setPixelColor(item-1, 0, 0, 255); 
      pixels.show();
  }

  // GREEN
  if (color == 2){
      pixels.setPixelColor(item-1, 0, 255, 0); 
      pixels.show();
  }
  
  // RED
  if (color == 3){
      for (int j=0; j<item; j++){
        pixels.setPixelColor(j, 255, 0, 0); 
        pixels.show();
      }
  }
  
  }

// =========================  SETUP
void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  Serial.begin(9600);

  // put your setup code here, to run once:
  sc_start = millis();

  
  for(int i=0; i<ARRAY_SIZE_X*ARRAY_SIZE_Y; i++){
    pinMode(keyPin_ledPoint[0][i], INPUT);
  }
}

// =========================  LOOP
void loop() {

// STEP 1: INIT

  if (!isInit){
    Serial.println(" "); 
    Serial.println("STEP 1:INIT");
    Serial.println(" ");
    set_pointArray();
    set_fieldArray();

    
    isInit = true;
    isGame = true;
    isGameOver = false;
    isWIN =false;
    iPosition =0;


    //
    ledPoint(0,OFF);
    ledPoint(keyPin_ledPoint[1][pointArray[iPosition]-1],BLUE);
    
  } // END INIT

// STEP 2: GAME

  while (isGame){
    //inspectionPoint();
    //foundNextPoint();

// -------------------------------
  // Слушаем все кнопки
  for(int i=0; i<ARRAY_SIZE_X*ARRAY_SIZE_Y; i++){
    int buttonState = digitalRead(keyPin_ledPoint[0][i]);
       
       // ЕСЛИ нажата ЛЮБАЯ кнопка 
       if (buttonState == HIGH) {
        Serial.print("BPin: ");
        Serial.print(keyPin_ledPoint[0][i]);
        Serial.print("  Num:");
        Serial.println(i+1);

          // ЕСЛИ нажата не правильная кнопка ТОГДА конец игры
          if (isPressWrongBtn(keyPin_ledPoint[0][i])) {
            Serial.println("GO# false");
            isGameOver = true;
     
            }
        
          // ЕСЛИ нажата кнопка выграшная и все придыдущие выграшные кнопки нажаты ТОГДА выбираем следущую  
          if ( (pointArray[iPosition] == i+1)&&(isPressAllRightBtn()) ) {
            Serial.print("                   iPoint: ");
            Serial.print(pointArray[iPosition]);
            Serial.print("                 iP: ");
            Serial.print(iPosition);
            Serial.print("                 LED: ");
            Serial.println(keyPin_ledPoint[0][pointArray[iPosition]-1]);
            
            ledPoint(keyPin_ledPoint[1][pointArray[iPosition]-1],GREEN);

            if (iPosition < WIN_POINTS) { 
              iPosition++;
              ledPoint(keyPin_ledPoint[1][pointArray[iPosition]-1],BLUE);
            }
             
          } //END --IF

          // ЕСЛИ количество нажатых кнопок равно выграшным ТОГДА game win и выходи с цыкла 
          if (iPosition == WIN_POINTS) {
            Serial.println("WIN");
            isGame = false;
            isWIN  = true;
            }
            

      // ЕСЛИ не нажата кнопка  END buttonState IF--
      } else { 
          // ЕСЛИ отпущена кнопка из правильной комбинации ТОГДА конец игры   
          if (!isPressAllRightBtn()) {
            Serial.print("GO# Btn:"); 
            Serial.print(iPosition);
            Serial.print("N:");
            isGameOver = true;
     
          } //END --IF

      } // END buttonState IF--
        
  }// END FOR--
  
// -------------------------------
    
    if (isGameOver) {
      Serial.println("GameOver"); 
      ledPoint(ARRAY_SIZE_X*ARRAY_SIZE_Y,RED);
      isGame = false; 
      isInit = false;
      
      }
  } // END GAME
  

// -------------------------------

   
  
}

//------------------------------------------------------------

boolean isPressWrongBtn(int KeyPin){
  boolean result = true;
  //Serial.println(KeyPin);
  //Serial.println(keyPin_ledPoint[0][pointArray[1]-1]);
  
  for(int i = 0; i < WIN_POINTS; i ++){
    if (KeyPin == keyPin_ledPoint[0][pointArray[i]-1]) { 
    // Serial.print("TRY: ");
    // Serial.println(keyPin_ledPoint[0][pointArray[i]-1]);
     result = false;
     break;
    }// END IF--
  }// END FOR--
 
   
 return result; 
}// END --isPressWrongBtn() 


boolean isPressAllRightBtn(){
  boolean result = true;
  //Serial.println("isPressAllPoint");
  
  for(int i = 0; i < iPosition; i ++){
    int  buttonState = digitalRead(keyPin_ledPoint[0][pointArray[i]-1]);
    if (buttonState == LOW) { 
     Serial.print("LOW: ");
     Serial.println(keyPin_ledPoint[0][pointArray[i]-1]);
     
     result = false;
     break;
    }// END IF--
  }// END FOR--
 
   
 return result; 
}// END --isPressAllRightBtn() 
