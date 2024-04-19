#pragma once
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

//play sounds

void playBackgroundMusic();
void playMismatchSound();
void playMatchSound();
void playHintSound();
void playShuffleSound();
void playMoveSound();
