#ifndef PLAYER_H
#define PLAYER_H

#include <string>

struct Player {
    std::string ID;
    std::string password;
    int score;

    Player();                   //constructor
    Player(const Player &);     //constructor with arguments
};

#endif // PLAYER_H
