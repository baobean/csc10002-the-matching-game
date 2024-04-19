#include <iostream>
#include <string>
#include <fstream>

#include "screen.h"
#include "player.h"

struct EntryMenu {
    const std::string options[3] = {"LOGIN", "REGISTER", "QUIT"};
    const int option_slot = 3;

    std::string *game_title;
    std::string *menu_panel;

    Player user;
    Player user_list[1000];
    int number_user = 0;

    int currentOption;

    EntryMenu();
    ~EntryMenu();

    void loadUserData();    //load user data
    void saveUserData();    //save user data

    void loginMenu();       //control login menu
    void registerMenu();    //control register menu

    bool menuController();  //control entry scene menu
    void loadAssets(const std::string &, std::string *&);
    void displayBackground(bool);   //display background

    void changeOption(int); //change options on menu
    void selectOption();    //select options on menu
    void unselectOption();  //unselect options on menu
    void displayOptions();  //display options on menu
};
