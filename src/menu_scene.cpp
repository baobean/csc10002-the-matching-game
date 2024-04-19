#include "menu_scene.h"

Menu::Menu() {
    currentOption = 0;
    game_title = new std::string[18];
    menu_panel = new std::string[20];
}

Menu::~Menu() {
    delete []game_title;
    game_title = nullptr;

    delete []menu_panel;
    menu_panel = nullptr;
}

Player Menu::playCasualMode() {
    Screen::clearConsole();
    CasualMode game(EMenu.user, nullptr, nullptr);
    return game.gameplay();
}

Player Menu::playCasualMode(Player *user, Table *gameTable) {
    Screen::clearConsole();
    CasualMode game(EMenu.user, user, gameTable);
    return game.gameplay();
}

Player Menu::playHardMode() {
    Screen::clearConsole();
    HardMode game(EMenu.user, nullptr);
    return game.gameplay();
}

bool Menu::menuController(bool isLogin) {
    if (isLogin == false) {
        while (EMenu.menuController() == false);
    }

    loadAssets("assets/game_title.txt", game_title);
    loadAssets("assets/menu_panel.txt", menu_panel);

    displayBackground();
    displayOptions();

    bool in_menu = true;
    Player player_state;
    Player current_play;

    auto validateFileOpened = [](const std::string &name) {
        std::ifstream file(name);
        if (!file) return false;
    return true;
    };

    while (in_menu) {
        switch (Screen::getConsoleInput()) {
            case 2:
                changeOption(-1);
                playMoveSound();
                break;
            case 3:
                changeOption(1);
                playMoveSound();
                break;
            case 6:
                in_menu = false;
                playHintSound();
                switch (currentOption) {
                    case 0: {
                        Screen::clearConsole();
                        displayBackground();
                        int choice = SMenu.menuController();
                        if (choice == 0) {
                            EMenu.loadUserData();
                            player_state = playCasualMode();
                            current_play = player_state;
                            while (EndMenu.menuController(current_play) == true) {
                                EMenu.loadUserData();
                                player_state = playCasualMode();
                                current_play = player_state;
                            }
                            return true;
                        }
                        else if (choice == 1) {
                            EMenu.loadUserData();
                            Player *last_play = new Player;
                            last_play->ID = EMenu.user.ID;
                            last_play->password = EMenu.user.password;
                            Table *gameTable = nullptr;
                            SMenu.loadGame(last_play, gameTable);
                            if (validateFileOpened("saves/" + last_play->ID + ".bin") == false) {
                                Screen::setCursorColor(BLACK, RED);
                                Screen::gotoXY(56, 20);
                                std::cout << "SAVE FILES NOT EXIST";
                                Sleep(1500);
                                return true;
                            }
                            current_play = playCasualMode(last_play, gameTable);
                            while (EndMenu.menuController(current_play) == true) {
                                EMenu.loadUserData();
                                player_state = playCasualMode();
                                current_play = player_state;
                            }
                            return true;
                        }
                        else return true;
                        return true;
                    }
                    case 1: {
                        Screen::clearConsole();
                        EMenu.loadUserData();
                        player_state = playHardMode();
                        current_play = player_state;
                        while (EndMenu.menuController(current_play) == true) {
                            EMenu.loadUserData();
                            player_state = playHardMode();
                            current_play = player_state;
                        }
                        return true;
                    }
                    case 2:
                        IScene.displayInstruction();
                        return true;
                    case 3:
                        LScene.displayLeaderboard();
                        return true;
                    case 4:
                        CScene.displayCredit();
                        return true;
                    case 5:
                        Screen::clearConsole();
                        exit(0);
                }
            case 1:
                Screen::clearConsole();
                exit(0);
                break;
            default:
                continue;
        }
    }

    return true;
}

void Menu::loadAssets(const std::string &path, std::string *&dest) {
    std::fstream asset(path, std::ios::in);

    for(int id = 0; !asset.eof(); ++id) {
        getline(asset, dest[id]);
    }

    asset.close();
}

void Menu::displayBackground() {
    Screen::clearConsole();

    Screen::setCursorColor(BLACK, GREEN);

    int padding_left = 10, padding_top = 1; //(132 - 112) / 2 = 10
    Screen::gotoXY(padding_left, padding_top);

    for(int i = 0; i < 18; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout << game_title[i] << "\n";
    }

    Screen::setCursorColor(BLACK, LIGHT_WHITE);

    padding_left = 48, padding_top = 21; //(132 - 36) / 2 = 48

    for(int i = 0; i < 20; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout << menu_panel[i] << "\n";
    }
}

void Menu::changeOption(int change) {
    int updatedOption = currentOption + change;
    if (updatedOption < 0 || updatedOption >= option_slot) return;

    unselectOption();

    currentOption = updatedOption;

    selectOption();
}

void Menu::selectOption() {
    Screen::setCursorColor(BLACK, GREEN);

    int padding_left = 59, padding_top = 26;

    Screen::gotoXY(padding_left - 2, padding_top + currentOption * 2);
    putchar('>');

    Screen::gotoXY(padding_left + options[currentOption].length() + 1, padding_top + currentOption * 2);
    putchar('<');

    Screen::gotoXY(padding_left, padding_top + currentOption * 2);
    std::cout << options[currentOption];
}

void Menu::unselectOption() {
    Screen::setCursorColor(BLACK, WHITE);

    int padding_left = 59, padding_top = 26;

    Screen::gotoXY(padding_left - 2, padding_top + currentOption * 2);
    putchar(' ');

    Screen::gotoXY(padding_left + options[currentOption].length() + 1, padding_top + currentOption * 2);
    putchar(' ');

    Screen::gotoXY(padding_left, padding_top + currentOption * 2);
    std::cout << options[currentOption];
}

void Menu::displayOptions() {
    Screen::setCursorColor(BLACK, WHITE);

    int padding_left = 59, padding_top = 26; //(132 - 11) / 2 - 1 = 59
    Screen::gotoXY(padding_left, padding_top);

    for(int i = 0; i < option_slot; ++i) {
        Screen::gotoXY(padding_left, padding_top + i * 2);
        std::cout << options[i] << "\n";
    }

    selectOption();
}
