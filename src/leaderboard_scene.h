#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "screen.h"
#include "player.h"

struct LeaderboardScene {
    int user_number = 0;
    Player user_list[1000];

    void displayBackground();  //display background
    void loadUserData();        //load user data

    void displayLeaderboard();  //display leaderboard screen
};
