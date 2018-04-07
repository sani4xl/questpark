lib/ SevenSegmentTM1637 - lib for TM1637
timeBack.cpp - main.cpp for platformio

Folder structure:
 
 01/ 001xxxxx.mp3  // Short sound after pressing "START". State: Pressed Start
 
 02/ 001xxxxx.mp3  // Cyclic music playback in a folder. State: Hide and seek two minutes
     002xxxxx.mp3  
     ...

 03/ 001xxxxx.mp3  // Short sound after the expiration of two minutes. State: Start of normal time for game

 04/ 001xxxxx.mp3  // Cyclic music playback in a folder. State: Process of the game
     002xxxxx.mp3
     003xxxxx.mp3
     004xxxxx.mp3
     ...

 05/ 001xxxxx.mp3 // Short sound after the finished game. State: Finish game.


if <pressed START> - Short sound 2 sec. - Cyclic music folder 02 - After 2 min. Short sound 2 sec. - Cyclic music folder 04 - Short sound 2 sec. - FInished

if <pressed RESET> - reset time and sound - set pressed RESET

if <NOT press START AND pressed MODE> - time setting [5 10 15 20 25 30] - set pressed START
