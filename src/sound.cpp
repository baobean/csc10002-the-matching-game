#include "sound.h"

void playBackgroundMusic() {
    PlaySound("audio/background.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void playMismatchSound() {
    PlaySound("audio/mismatch.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playMatchSound() {
    PlaySound("audio/matched.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playHintSound() {
    PlaySound("audio/hint.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playShuffleSound() {
    PlaySound("audio/shuffle.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playMoveSound() {
    PlaySound("audio/move.wav", NULL, SND_FILENAME | SND_ASYNC);
}
