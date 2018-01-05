#include <Bounce2.h>

const int labRelePin = 3;
const int buttonPin = 4;
int prevLabReleMillis;
int nextLabReleMillisSwitch = 0;
boolean labLightOn = true;

boolean labLightSwitchingActive = true;

int switchMode = 0;

Bounce buttonBounce = Bounce();

void setup()
{
  Serial.begin(9600);

  buttonBounce.attach(buttonPin);
  buttonBounce.interval(10); 

  labLightSwitchingActive = false;

  
  pinMode (labRelePin, OUTPUT);
  switchLightOn();
 
  
  
 
}


void loop()
{
  buttonBounce.update();

  if(buttonBounce.rose()){

    switchMode++;
    if(switchMode>=3){
      switchMode = 0;
    }

    if(switchMode == 0){
      labLightSwitchingActive = false;
      switchLightOn();
    }
    else if(switchMode == 2){
      labLightSwitchingActive = false;
      switchLightOff();
    }
    else{
      labLightSwitchingActive = true;
    }
    
  }

  if(labLightSwitchingActive){
    prevLabReleMillis = millis();
    if(prevLabReleMillis > nextLabReleMillisSwitch){
      nextLabReleMillisSwitch = prevLabReleMillis + random(100, 2000);
      doLabSwitch();
    }
  }
  
 
}

void doLabSwitch(){
   if(labLightOn){
    switchLightOff();
  }
  else{
    switchLightOn();
  }
}

void switchLightOn(){
   digitalWrite(labRelePin, HIGH);
   labLightOn = true;
  
}

void switchLightOff(){
   digitalWrite(labRelePin, LOW);
   labLightOn = false;
  
}
