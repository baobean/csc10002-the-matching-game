#include <iostream>
#include <string>
#include <fstream>

#include "screen.h"
#include "player.h"

struct EndgameMenu {
    const std::string options[2] = {"TRY AGAIN", "MAIN MENU"};
    const int option_slot = 2;

    std::string *title;     //store title

    int currentOption = 0;

    bool menuController(Player);        //control menu

    void loadAssets(const std::string &, std::string *&);   //load backgrounds to display
    void displayBackground();                           //display backgrounds

    void changeOption(int);     //change current option
    void selectOption();        //select option
    void unselectOption();      //unselect current option
    void displayOptions();      //display options
};
