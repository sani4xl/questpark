#define LIGHT_1  3
#define LIGHT_2  4
#define LIGHT_3  5
#define LIGHT_4  6

boolean light1Closed = false;
boolean light2Closed = false;
boolean light3Closed = false;
boolean light4Closed = false;
boolean isDoorOpened = false;
#define RELAY 8

#include <SoftwareSerial.h>

// audio stuff
static int8_t Send_buf[8] = {0} ;
#define CMD_PLAY_W_INDEX 0X03
#define CMD_SET_VOLUME 0X06
#define CMD_SEL_DEV 0X09
  #define DEV_TF 0X02
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_STOP 0X16
#define CMD_SINGLE_CYCLE 0X19
  #define SINGLE_CYCLE_ON 0X00
  #define SINGLE_CYCLE_OFF 0X01
#define CMD_PLAY_W_VOL 0X22
SoftwareSerial mp3Serial(10, 9); // RX, TX

int openedMills = 0;

int currentTrack = 0;
//  001 track 1
//  002 track 2
//  003 track 3
//  004 track 4

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(LIGHT_1, INPUT);
  pinMode(LIGHT_2, INPUT);
  pinMode(LIGHT_3, INPUT);
  pinMode(LIGHT_4, INPUT);

  pinMode(RELAY, OUTPUT);

  initMp3Player();
}


void sendMp3Command(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for(uint8_t i=0; i<8; i++)//
  {
    mp3Serial.write(Send_buf[i]) ;
  }
}

void stopMp3(){
  sendMp3Command(CMD_STOP, 0x00);//select the TF card  
}

void initMp3Player(){
  //lastTimePlay = millis() / 1000;
  //currentSec = millis() / 1000;
  //pinMode(songs, INPUT_PULLUP);
  //pinMode(alarm, INPUT);
  //Serial1.begin(9600);
  mp3Serial.begin(9600);
  delay(500);//Wait chip initialization is complete
  sendMp3Command(CMD_SEL_DEV, DEV_TF);//select the TF card  
  delay(200);//wait for 200ms
  //sendCommand(CMD_SET_VOLUME, 0X0F);//установить громкость в 15 (50%)
  sendMp3Command(CMD_SET_VOLUME, 0X1E);//установить громкость в x1E=30 (100%)

  
  //sendCommand(0X17, 0X0102);//запустить зацикленное проигрывание содержимого папки "/01"
  //sendCommand(CMD_SINGLE_CYCLE, SINGLE_CYCLE_OFF);//запустить зацикленное проигрывание содержимого папки "/01"
  play4();
}

void play4(){
  // int songCode = word(0x01,1);
  if(currentTrack == 1) return;
  //sendMp3Command(0X0F, songCode);// играем трек 001 из папки 01
  sendMp3Command(0X17, 0X0102);//запустить зацикленное проигрывание содержимого папки "/01"
  currentTrack = 1;
}

void play3(){
  if(currentTrack == 2) return;
  //int songCode = word(0x02,1);
  //sendMp3Command(0X0F, songCode);// играем трек 002 из папки 01
  sendMp3Command(0X17, 0X0202);//запустить зацикленное проигрывание содержимого папки "/01"
  currentTrack = 2;
}

void play2(){
  //int songCode = word(0x03,1);
  if(currentTrack == 3) return;
  //sendMp3Command(0X0F, songCode);// играем трек 003 из папки 01
  sendMp3Command(0X17, 0X0302);//запустить зацикленное проигрывание содержимого папки "/01"
  currentTrack = 3;
}

void play_voice_3(){
  //int songCode = word(0x04,1);
  if(currentTrack == 4) return;
  //sendMp3Command(0X0F, songCode);// играем трек 004 из папки 01
  sendMp3Command(0X17, 0X0402);//запустить зацикленное проигрывание содержимого папки "/01"
  currentTrack = 4;
}

void play_voice_2(){
  //int songCode = word(0x04,1);
  if(currentTrack == 4) return;
  //sendMp3Command(0X0F, songCode);// играем трек 004 из папки 01
  sendMp3Command(0X17, 0X0502);//запустить зацикленное проигрывание содержимого папки "/01"
  currentTrack = 4;
}

void play_voice_4(){
  //int songCode = word(0x04,1);
  if(currentTrack == 4) return;
  //sendMp3Command(0X0F, songCode);// играем трек 004 из папки 01
  sendMp3Command(0X17, 0X0602);//запустить зацикленное проигрывание содержимого папки "/01"
  currentTrack = 4;
}

void play_voice_1(){
  //int songCode = word(0x04,1);
  if(currentTrack == 4) return;
  //sendMp3Command(0X0F, songCode);// играем трек 004 из папки 01
  sendMp3Command(0X17, 0X0702);//запустить зацикленное проигрывание содержимого папки "/01"
  currentTrack = 4;
}

void loop() {
  int delta = millis() - openedMills;
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(openedMills);
  Serial.print(" ");
  Serial.print(delta);
  if(isDoorOpened && ( delta > 1000 * 60 * 30 ) ){
    
    //stopMp3();
    //return;
  }
  // put your main code here, to run repeatedly:
  int lightState1 = digitalRead(LIGHT_1);
  int lightState2 = digitalRead(LIGHT_2);
  int lightState3 = digitalRead(LIGHT_3);
  int lightState4 = digitalRead(LIGHT_4);

  int closeCount = 0;
  
  if(lightState1){
    Serial.println("s1");
    light1Closed = true;
    closeCount++;
  }
  else{
    light1Closed = false;
  }

  if(lightState2){
    Serial.println("s2");
    light2Closed = true;
    closeCount++;
  }
  else{
    light2Closed = false;
  }

  if(lightState3){
    Serial.println("s3");
    closeCount++;
    light3Closed = true;
  }
  else{
    light3Closed = false;
  }

  if(lightState4){
    Serial.println("s4");
    closeCount++;
    light4Closed = true;
  }
  else{
    light4Closed = false;
  }

  //Serial.println(closeCount);
  if(closeCount == 1){
    play3();
  }
  else if(closeCount == 2){
    play2();
  }
  else if(closeCount == 3){
    
    if(!light1Closed){
      play_voice_1();
    }
    else if(!light2Closed){
      play_voice_2();
    }
    else if(!light4Closed){
      play_voice_4();
    }
    else if(!light3Closed){
      play_voice_3();
      digitalWrite(RELAY, HIGH);
      if(!isDoorOpened){
        openedMills = millis();
      }
      isDoorOpened = true;
      delay(12000);
      
      //stopMp3();
      sendMp3Command(CMD_SET_VOLUME, 0X17);
      
    }
  }
  else if(closeCount == 4){
   stopMp3();
   delay(100);
  }
  else{
    play4();
    digitalWrite(RELAY, LOW);
  }
  

}
