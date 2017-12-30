//gp2y0a02yk0f
//http://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
// 
const int trigPin = 9;
const int echoPin = 10;

const int relay = 6;
long duration;
int distance;
boolean isMotor = true;
unsigned long sc_start_motor = 0;

void setup() {
  pinMode(relay, OUTPUT);
  digitalWrite(relay,LOW); 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);



  if ((distance < 7)||(distance == 7)) {
      digitalWrite(relay,HIGH);
      isMotor = true;
      sc_start_motor = millis();
    } else {isMotor = false;}  
  
  if ( (isMotor == false)&&( millis() - sc_start_motor > 2000) ){
      digitalWrite(relay,LOW);   
      Serial.print("ON");
    }
    
}

