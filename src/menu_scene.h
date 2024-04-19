#include <iostream>
#include <string>
#include <fstream>

#include "entry_scene.h"
#include "game_saving.h"
#include "casual_mode.h"
#include "hard_mode.h"
#include "endgame_scene.h"
#include "instruction_scene.h"
#include "leaderboard_scene.h"
#include "credit_scene.h"
#include "sound.h"

struct Menu {
    const std::string options[6] = {"CASUAL MODE", "HARD MODE", "INSTRUCTION", "LEADERBOARD", "CREDIT", "QUIT"};
    const int option_slot = 6;

    std::string *game_title;
    std::string *menu_panel;

    int currentOption;

    EntryMenu EMenu;
    EndgameMenu EndMenu;
    SavingMenu SMenu;
    InstructionScene IScene;
    LeaderboardScene LScene;
    CreditScene CScene;

    Menu();
    ~Menu();

    Player playCasualMode();
    Player playCasualMode(Player *, Table *);
    Player playHardMode();

    bool menuController(bool);                                  //control menu
    void loadAssets(const std::string &, std::string *&);       //load backgrounds
    void displayBackground();                                   //display backgrounds

    void changeOption(int);         //change current option
    void selectOption();            //select option
    void unselectOption();          //unselect current option
    void displayOptions();          //display options
};
