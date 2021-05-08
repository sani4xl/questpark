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

const int MAX_SCORES = 999;
const int CHAR_LENGTH_PER_TEAM = 3;

int teamRedScores = 0;
int teamGreenScores = 0;
int countingTeam = 0; // 1 for red, -1 for green

int gameStatus = 1;

const int GAME_RUNNING = 1;
const int GAME_STOPPED = 0;

String gameIndicator = "";


void setup() {
  serialinit();
  
  tm.displayBegin();
  tm.brightness(8);
  
  displayNothing();
  
  switchToRed(); // todo: remove
}

void loop() {
  runGame();

}

void runGame() {

  if (gameStatus != GAME_RUNNING) {
    return;
  }
  
  switch (gameMode) {
    case DOMINATION_MODE:
      runDominatorGame();
      break;
  }

  displayGameIndicator();
  
}

void switchToRed() {
  countingTeam = 1;
}

void switchToGren() {
  countingTeam = -1;
}

void countTeams() {
  if (countingTeam > 0) { 
    teamRedScores+= 1;
  } else if (countingTeam < 0) { 
    teamGreenScores+= 1;
  }
  
  if (teamRedScores > MAX_SCORES) {
    teamRedScores = MAX_SCORES;
  }
  
  
  if (teamGreenScores > MAX_SCORES) {
    teamGreenScores = MAX_SCORES;
  }
}

void runDominatorGame() {
  
  countTeams();
  gameIndicator = formatScores(teamRedScores) + "  " + formatScores(teamGreenScores); // delimiter 2 spaces
  
  delay(50);

  checkForWinner();
}

void displayGameIndicator() {
  int str_len = gameIndicator.length() + 1;
  char char_array[str_len];
  gameIndicator.toCharArray(char_array, str_len);
  tm.displayText(char_array);
}

void displayNothing() {
  tm.displayText("________");
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
  if (!(teamRedScores >= MAX_SCORES || teamGreenScores >= MAX_SCORES)) { // || outof time
    return;
  }

  gameStatus = GAME_STOPPED;

  if (teamRedScores == teamGreenScores) {
    // draw;
  } else if (teamRedScores > teamGreenScores) {
    // red wins;
  } else {
    // green wins
  }

  blinkGameIndicator();

}

void blinkGameIndicator() {
  for (int i = 0; i < 10; i++) {
    displayNothing();
    delay(500);
    displayGameIndicator();
    delay(500);
  }
}


void serialinit()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("--starting dominator--");
}
