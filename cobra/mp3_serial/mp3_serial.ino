static int8_t Send_buf[8] = {0} ;


const int alarm = 7;     // вывод контроллера для аварии
int songs_old = 1;    // предидущее значение входа для отлавливания момента нажатия
int alarm_old = 1;
const int timeBetweenTracks = 15; // in secs

const int totalTracks = 31;
int currentTrack = 1;

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

#include <SoftwareSerial.h>
SoftwareSerial mp3Serial(8, 9); // RX, TX
int lastTimePlay; // in sec
int currentSec;

void setup() 
{
  lastTimePlay = millis() / 1000;
  currentSec = millis() / 1000;
  //pinMode(songs, INPUT_PULLUP);
  pinMode(alarm, INPUT);
  //Serial1.begin(9600);
  mp3Serial.begin(9600);
  Serial.begin(9600);
  delay(500);//Wait chip initialization is complete
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
  delay(200);//wait for 200ms
  
  sendCommand(CMD_SET_VOLUME, 0X0F);//установить громкость в 15 (50%)
  
  //sendCommand(0X17, 0X0102);//запустить зацикленное проигрывание содержимого папки "/01"
  //sendCommand(CMD_SINGLE_CYCLE, SINGLE_CYCLE_OFF);//запустить зацикленное проигрывание содержимого папки "/01"

  int songCode = word(0x01,1);
  sendCommand(0X0F, songCode);// играем трек 001 из папки 01

  //sendCommand(CMD_PLAY, DEV_TF);//select the TF card  

  //sendCommand(CMD_SET_VOLUME, 0X0F);//установить громкость в 15 (50%)
  
  //sendCommand(0X17, 0X0102);//запустить зацикленное проигрывание содержимого папки "/01"
}

void checkForNextTrack(){
  
  if( currentSec - lastTimePlay > timeBetweenTracks){
    lastTimePlay = currentSec;
    currentTrack++;
    if(currentTrack > 4){//totalTracks){
      currentTrack = 1;
    }

    int songCode = word(0x01, currentTrack);
    sendCommand(0X0F, songCode);// играем трек 001 из папки 01
    
  }
}

void playPassword(){
  // stop current track
  sendCommand(CMD_STOP, 0x00);//select the TF card  
  // playing password track
  int songCode = word(0x02, 0x01);
  sendCommand(0X0F, songCode);// играем трек 001 из папки 02
  lastTimePlay = currentSec + 10;
}

void loop() 
{
  int alarmState = digitalRead(alarm);
  //Serial.println(alarmState);
  //return;
  currentSec = millis() / 1000;
  checkForNextTrack();
  if(digitalRead(alarm)){
     playPassword();
  }
  delay(100);
  /*return;
  
if ((digitalRead(alarm)==LOW)&&(alarm_old==1))
  { 
    sendCommand(CMD_SET_VOLUME, 0X1E);//установить громкость в 30 (100%)
    sendCommand(0X17, 0X0202);//запустить зацикленное проигрывание содержимого папки "/02"
  }
alarm_old = digitalRead(alarm); 


if ((digitalRead(songs)==LOW)&&(songs_old==1))
  { 
    sendCommand(CMD_SET_VOLUME, 0X0F);//установить громкость в 15 (50%)
    sendCommand(0X17, 0X0102);//запустить зацикленное проигрывание содержимого папки "/01"
  }
songs_old = digitalRead(songs); 


delay(100);//wait for 100ms
*/
}

void sendCommand(int8_t command, int16_t dat)
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
