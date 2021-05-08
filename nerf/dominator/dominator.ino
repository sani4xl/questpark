#include <TM1638plus_Model2.h>
#include <TM1638plus_common.h>
#include <TM1638plus.h>


const int TM_STB = 4; 
const int TM_CLK = 3; 
const int TM_DIO = 2; 

bool high_freq = false; //default false,, If using a high freq CPU > ~100 MHZ set to true. 

//Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(TM_STB, TM_CLK , TM_DIO, high_freq);

int gameMode = 1;

const int DOMINATION_MODE = 1;
const int BOMB_DEFUSE_MODE = 2;

const int MAX_SCORES = 9999;
const int CHAR_LENGTH_PER_TEAM = 4;

int teamRedScores = 0;
int teamGreenScores = 0;


void setup() {
  tm.displayBegin();
  serialinit();

  tm.brightness(8);
  //tm.displayText("12345678");

}

void loop() {
  gameSelector();

}

void gameSelector() {
  switch (gameMode) {
    case DOMINATION_MODE:
      runDominatorGame();
      break;
  }
  
}

void runDominatorGame() {
  teamRedScores+= 1;
  teamGreenScores+= 2;

  String text = formatScores(teamRedScores) + formatScores(teamGreenScores);
  int str_len = text.length() + 1;
  char char_array[str_len];
  text.toCharArray(char_array, str_len);
  tm.displayText(char_array);
  delay(500);

  checkForWinner();
}

String formatScores(int scores) {
  String str = String(scores);
  int spaces = CHAR_LENGTH_PER_TEAM - str.length();
  for(int i = 0; i < spaces; i++) {
    str = " " + str;
  }

  return str;
}

void checkForWinner() {
  if (!(teamRedScores > MAX_SCORES || teamGreenScores > MAX_SCORES)) { // || outof time
    return;
  }

  if (teamRedScores == teamGreenScores) {
    // draw;
  } else if (teamRedScores > teamGreenScores) {
    // red wins;
  } else {
    // green wins
  }
  
}


void serialinit()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("--starting dominator--");
}
