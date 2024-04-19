#include "player.h"

Player::Player() {
    score = 0;
}

Player::Player (const Player &_player) {
    ID = _player.ID;
    password = _player.password;
    score = _player.score;
}
