const int micPin = 4;
const int shakePin = 3;
boolean micTriggeredTime = 0;
boolean shakeTriggeredTime = 0;

boolean micTriggeringCount = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(micPin, INPUT);
  pinMode(shakePin, INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
   int currentMills = millis();
   int soundVal = digitalRead(micPin);
   int shakeVal = digitalRead(shakePin);
   
   if(soundVal && !micTriggeredTime){
     micTriggeredTime = currentMills;
     //micTriggeringCount++;
   }
   else{
     //micTriggeringCount = 0;
     micTriggeredTime = 0;
   }

   if(shakeVal){
     shakeTriggeredTime = currentMills;
   }
   
   //Serial.print(shakeVal);
   //Serial.print(" ");
   //Serial.println(soundVal);

   //if( shakeTriggeredTime > 0 && abs(shakeTriggeredTime - micTriggeredTime) < 500){
   if( shakeVal && micTriggeredTime > 0 && (currentMills - micTriggeredTime > 3000)  ){
     Serial.println("GOOD");
   }

   //

}
