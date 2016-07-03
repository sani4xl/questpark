const int buttonPin = 2; 

const int wire1Pin = 4;
const int wire2Pin = 5;
const int wire3Pin = 6;
const int wire4Pin = 7;

int defuseOptions[2][4] = {{wire1Pin, wire2Pin, wire3Pin, wire4Pin},
                           {0, 0, 0, 0}}; 
// the number of the pushbutton pin
//const int ledPin =  13;      // the number of the LED pin

long subDefuseTime = 0;
int isBombDefused = 0;
int wireCutIndex = 0 ;

// variables will change:
int buttonState = 0; 
int wire1State = 0 ;// variable for reading the pushbutton status
int wire2State = 0 ;// variable for reading the pushbutton status
int wire3State = 0 ;// variable for reading the pushbutton status
int wire4State = 0 ;// variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  //pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  pinMode(wire1Pin, INPUT);
  pinMode(wire2Pin, INPUT);
  pinMode(wire3Pin, INPUT);
  pinMode(wire4Pin, INPUT);
  Serial.begin(9600); 
}

void checkWires(){
  for(int i  = 0; i < 4; i++){
    int isWireOn = digitalRead(defuseOptions[0][i]);
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
          subDefuseTime += 10;
        }
        delay(100);
      }
      else{
        if(i == wireCutIndex){
          wireCutIndex ++;
        }
      }
    }
  }
  
  if(wireCutIndex >= 4){
    isBombDefused = true; 
    Serial.println (subDefuseTime);
  }
}

void loop() {
  //checkWires();
  // read the state of the pushbutton value:
  wire1State = digitalRead(wire1Pin);//  digitalRead(buttonPin);
  wire2State = digitalRead(wire2Pin);//  digitalRead(buttonPin);
  wire3State = digitalRead(wire3Pin);//  digitalRead(buttonPin);
  wire4State = digitalRead(wire4Pin);//  digitalRead(buttonPin);
  
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  //Serial.println(wire1State);
  //*  
  if (wire1State != HIGH) {
    Serial.println("WIRE 1 OFF");  
  }
  
  if (wire2State != HIGH) {
    Serial.println("WIRE 2 OFF");  
  }
  
  if (wire3State != HIGH) {
    Serial.println("WIRE 3 OFF");  
  }
  
  if (wire4State != HIGH) {
    Serial.println("WIRE 4 OFF");  
  }
  Serial.println("========");
  //*/
  delay(200);
}
