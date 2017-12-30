

const int labRelePin = 22;
int prevLabReleMillis;
int nextLabReleMillisSwitch = 0;
boolean labLightOn = true;

void setup()
{
  Serial.begin(9600);

  
  pinMode (labRelePin, OUTPUT);
 
  
  
 
}


void loop()
{

  prevLabReleMillis = millis();
  if(prevLabReleMillis > nextLabReleMillisSwitch){
    nextLabReleMillisSwitch = prevLabReleMillis + random(100, 2000);
    doLabSwitch();
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
   digitalWrite(labRelePin, LOW);
   labLightOn = true;
  
}

void switchLightOff(){
   digitalWrite(labRelePin, HIGH);
   labLightOn = false;
  
}
