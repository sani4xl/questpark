 /* Author: Danny van den Brande, Arduinosensors.nl
 This is a example on how to use the KY-040 Rotary encoder. 
 Its very basic but if your new to arduino or could not find 
 any code, then you have something to start with.
 because there is little documentation about the KY sensor kit.
 */
 int CLK = 9;  // Pin 9 to clk on encoder
 int DT = 8;  // Pin 8 to DT on encoder
 
 int RotPosition = 0; 
 int rotation;  
 int value;
 boolean LeftRight;

 
 const int stepsPerRound = 30;
 const int stepsPerRoundTrash = 29;
 int prevDirection = 0;
 int rotDirection = 0;
 int directionDelta = 0;

 int curRoundPos = 0;
 int curRounds = 0;

 // left - 1
 // right +1
 const int pathSteps = 3;
 int curPathPos = 0;
 int prevCurPathPos = 0;
 int roundsPath[pathSteps] = { -2, 1, -1}; 
 
 void setup() { 
   Serial.begin (9600);
   pinMode (CLK,INPUT);
   pinMode (DT,INPUT);
   rotation = digitalRead(CLK);   
 } 

 void checkForWin(){
   if( roundsPath[curPathPos] == curRounds){
     curPathPos++;
     curRounds = 0;
     if(curPathPos >= pathSteps){
       Serial.println("WIN");
       curPathPos = 0;
       delay(2000);
     }
   }
   else if(curRounds > 0 && roundsPath[curPathPos] < 0){
      curPathPos = 0;
   }
   else if(curRounds < 0 && roundsPath[curPathPos] > 0){
      curPathPos = 0;
   }

   if(prevCurPathPos != curPathPos){
    prevCurPathPos = curPathPos;
   }
   
 }
 
 void loop() { 
     value = digitalRead(CLK);
     int dtVal = digitalRead(DT);
     if (value != rotation){ // we use the DT pin to find out which way we turning.
     if (digitalRead(DT) != value) {  // Clockwise
       RotPosition ++;
       LeftRight = true;
     } else { //Counterclockwise
       LeftRight = false;
       RotPosition--;
     }
     if (LeftRight){ // turning right will turn on red led.
       //Serial.println ("clockwise");
       prevDirection = -1;
       rotDirection = -1;
       //digitalWrite (RedLed, HIGH);
       //digitalWrite (GreenLed, LOW);
     }else{        // turning left will turn on green led.   
       //Serial.println("counterclockwise");
       prevDirection = 1;
       rotDirection = 1;
       //digitalWrite (RedLed, LOW);
       //digitalWrite (GreenLed, HIGH);
     }

     directionDelta += rotDirection;
     if(directionDelta < -2){
       directionDelta = -2;
     }
     else if(directionDelta > 2){
       directionDelta = 2;
     }
     //Serial.println(directionDelta);
      
     if(directionDelta == 0){
      Serial.println("switch direction");
      curRoundPos = rotDirection;
      curRounds = 0;
     }
     else{
      curRoundPos += rotDirection;
     }

     if ( abs(round(curRoundPos / stepsPerRoundTrash) ) >= 1){
        curRoundPos = 0 ;
        Serial.println("full round");
        curRounds += rotDirection; 
        Serial.println(curRounds);
        // += 
      
     }
     //Serial.print("Encoder RotPosition: ");
     //Serial.println(RotPosition);
     // this will print in the serial monitor.
     
   } 
   //Serial.println( dtVal );
   rotation = value;

   checkForWin();
 } 
