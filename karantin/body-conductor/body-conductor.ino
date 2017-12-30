// STUFF: 
//https://arduino-ua.com/prod1909-datchik-toka-i-napryajeniya-s-analogovim-vihodom-na-max471
// power supply 12V 1A
// + rele 5V

// Henry's Bench
// MAX471 Power Meter Tutorial

#define VT_PIN A0 
#define AT_PIN A1

const int RELE_PIN = 3;

void setup()
{
  Serial.begin(9600);
  pinMode (RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, LOW);
  
}

float voltageSum = 0;
void loop()
{
  int vt_read = analogRead(VT_PIN);
  int at_read = analogRead(AT_PIN);

  float voltage = vt_read * (5.0 / 1024.0) * 5.0;
  float current = at_read * (5.0 / 1024.0);
  float watts = voltage * current;

  if(voltage > 0){
    voltageSum += voltage;
    Serial.println(voltageSum);
    if(voltageSum >= 0.5){
      digitalWrite(RELE_PIN, HIGH);
    }
  }
  else{
    voltageSum = 0;
    digitalWrite(RELE_PIN, LOW);
  }
  
  //Serial.print("Volts: "); 
  //Serial.println(voltage);
  //Serial.print("\tAmps: ");
  //Serial.print(current,3);
  //Serial.print("\tWatts: ");
  //Serial.println(watts,3);
  //Serial.println();
  
  delay(500);
  
}
