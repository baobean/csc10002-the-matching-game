#include "leaderboard_scene.h"

void LeaderboardScene::displayBackground() {
    std::fstream in("assets/leaderboard.txt", std::ios::in);

    std::string *title;
    title = new std::string[5];

    for(int i = 0; i < 5; ++i) {
        getline(in, title[i]);
    }

    in.close();

    Screen::setCursorColor(BLACK, GREEN);

    for(int i = 0; i < 5; ++i) {
        Screen::gotoXY(25, i + 1);
        std::cout << title[i];
    }
}

void LeaderboardScene::loadUserData() {
    std::fstream in("user_data/score_data.txt", std::ios::in);

    Player *new_list = new Player[1000];
    std::string input_data;
    Player tempPl;
    int id = 0;
    while (getline(in, input_data, '/')) {
        tempPl.ID = input_data;
        getline(in, input_data, '/');
        tempPl.password = input_data;
        getline(in, input_data);
        tempPl.score = stoi(input_data);
        new_list[id++] = tempPl;
    }

    std::sort(new_list, new_list + id, [](Player a, Player b) {
              if (a.ID < b.ID) return true;
              else if (a.ID == b.ID) {
                if (a.score > b.score) return true;
              }
              return false;
              });

    user_list[user_number++] = new_list[0];
    for(int i = 0; i + 1 < id; ++i) {
        if (new_list[i].ID != new_list[i + 1].ID) {
            user_list[user_number++] = new_list[i + 1];
        }
    }

    std::sort(user_list, user_list + user_number, [](Player a, Player b) {
              if (a.score > b.score) return true;
              return false;
              });

    delete []new_list;
    in.close();
}

void LeaderboardScene::displayLeaderboard() {
    Screen::setCursorColor(BLACK, BLACK);
    Screen::clearConsole();

    displayBackground();

    loadUserData();

    Screen::setCursorColor(BLACK, YELLOW);
    Screen::gotoXY(48, 10);
    std::cout << "NAME";
    Screen::gotoXY(78, 10);
    std::cout << "SCORE";

    Screen::setCursorColor(BLACK, LIGHT_YELLOW);
    Screen::gotoXY(48, 11);
    std::cout << "-----------------------------------";

    Screen::setCursorColor(BLACK, LIGHT_WHITE);
    for(int i = 0; i < std::min(15, user_number); ++i) {
        Screen::gotoXY(48, 12 + i);
        std::cout << user_list[i].ID;
        if (user_list[i].score > 99)
            Screen::gotoXY(80, 12 + i);
        else if (user_list[i].score > 9)
            Screen::gotoXY(81, 12 + i);
        else Screen::gotoXY(82, 12 + i);
        std::cout << user_list[i].score;
    }

    while (getch() != 27);
}
