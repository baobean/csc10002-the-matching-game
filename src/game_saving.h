#ifndef GAME_SAVING_H
#define GAME_SAVING_H

#include <iostream>
#include <string>
#include <fstream>

#include "screen.h"
#include "table.h"
#include "player.h"

struct SavingMenu {
    const std::string options[2] = {"NEW GAME", "CONTINUE"};
    const int option_slot = 2;

    int currentOption = 0;

    int menuController();                   //control menu

    void changeOption(int);                 //change current option
    void selectOption();                    //select option
    void unselectOption();                  //unselect current option
    void displayOptions();                  //display options

    void loadGame(Player *&, Table *&);     //load game
    void saveGame(Player, Table *);         //save game
};

#endif // GAME_SAVING_H
