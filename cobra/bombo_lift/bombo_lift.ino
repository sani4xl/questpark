int IN3 = 3; // Input3 подключен к выводу 5 
int IN4 = 4;
int SPEEDIN = 5;

const int buttonUp = 7; 
const int buttonDown = 8; 

const int stopperDown = 11; 
const int stopperUp = 10; 

int buttonUpState = 0;
int buttonDownState = 0;

int movingDown = false;
int movingUp = false;

int stopperDownState = false;
int stopperUpState = false;

void setup()
{
 pinMode (SPEEDIN, OUTPUT); 
 pinMode (IN3, OUTPUT);
 pinMode (IN4, OUTPUT);
 
 pinMode(buttonUp, INPUT);
 pinMode(buttonDown, INPUT);
 pinMode(stopperDown, INPUT);
 pinMode(stopperUp, INPUT);
  Serial.begin(9600); 
}
void loop()
{
  
  stopperDownState = digitalRead(stopperDown);
  if(stopperDownState){
     
  }
  else{
    Serial.print("down reached");  
  }
 
 stopperUpState = digitalRead(stopperUp);
  if(stopperUpState){
     //Serial.print("NO FORMAT");  
     //delay(100);
  } 
  else{
    Serial.print("up reached");  
  }
  
  buttonUpState = digitalRead(buttonUp);
  buttonDownState = digitalRead(buttonDown);
  
  if(buttonDownState){
    //Serial.print("DOWN PRESSED");  
    if(movingDown){
      movingDown = false;
    }
    else{
      movingDown = true;
    }
    
    movingUp = false;
    delay(250);
  }
  else if(buttonUpState){
    //Serial.print("UP PRESSED");  
    movingDown = false;
    if(movingUp){
      movingUp = false;
    }
    else{
      movingUp = true;
    }
    delay(250);
  }
  
  if(!stopperDownState){
    movingDown = false;
  }
  
  if(!stopperUpState){
    movingUp = false;
  }
  
  // На пару выводов "IN" поданы разноименные сигналы, мотор готов к вращаению
  if(movingDown){
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
    Serial.print("moving down");  
    
  // подаем на вывод ENB управляющий ШИМ сигнал 
  }
  else if(movingUp){
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, HIGH);
    Serial.print("moving up");  
  }
  
  if(movingUp || movingDown ){
    //delay(2000);
    analogWrite(SPEEDIN, 250);
    //delay(2000);
    // Останавливаем мотор повад на вывод ENB сигнал низкого уровеня. 
    // Состояние выводов "IN" роли не играет.
  } 
  else{
    analogWrite(SPEEDIN, 0);
  }
  
  
  
  
}
