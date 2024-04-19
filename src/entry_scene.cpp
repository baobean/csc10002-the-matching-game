#include "entry_scene.h"

EntryMenu::EntryMenu() {
    currentOption = 0;
    game_title = new std::string[18];
    menu_panel = new std::string[20];
}

EntryMenu::~EntryMenu() {
    delete []game_title;
    game_title = nullptr;

    delete []menu_panel;
    menu_panel = nullptr;
}

void EntryMenu::loadUserData() {
    std::fstream ifs("account_data.txt", std::ios::in);

    std::string input;
    Player player_temp;
    int id = 0;
    while (getline(ifs, input, '/')) {
        player_temp.ID = input;

        getline(ifs, input);
        player_temp.password = input;

        user_list[id++] = player_temp;
    }

    number_user = id;
    ifs.close();
}

void EntryMenu::saveUserData() {
    std::fstream ofs("account_data.txt", std::ios::app);

    if (number_user > 0) ofs << '\n';
    ofs << user.ID << '/' << user.password;

    ofs.close();
}

void EntryMenu::loginMenu() {
    Screen::setCursorColor(BLACK, LIGHT_AQUA);

    Screen::gotoXY(60, 22);
    std::cout << "<< LOGIN >>";
    Screen::gotoXY(50, 23);
    std::cout << "-------------------------------";
    Screen::gotoXY(52, 25);
    std::cout << "USERNAME:";
    Screen::gotoXY(52, 28);
    std::cout << "PASSWORD:";
    Screen::gotoXY(50, 30);
    std::cout << "-------------------------------";

    Screen::gotoXY(62, 25);
    for(int i = 0; i < 100; ++i) std::cout << ' ';
    Screen::gotoXY(62, 28);
    for(int i = 0; i < 100; ++i) std::cout << ' ';

    Screen::showCursor(true);

    Screen::gotoXY(62, 25);
    user.ID.clear();
    getline(std::cin, user.ID);

    Screen::gotoXY(62, 28);
    user.password.clear();
    getline(std::cin, user.password);

    Screen::showCursor(false);
}

void EntryMenu::registerMenu() {
    Screen::setCursorColor(BLACK, LIGHT_AQUA);

    Screen::gotoXY(59, 22);
    std::cout << "<< REGISTER >>";
    Screen::gotoXY(39, 23);
    std::cout << "USERNAME AND PASSWORD MUST BE LESS THAN 10 CHARACTERS";
    Screen::gotoXY(59, 24);
    std::cout << "WITH NO SPACES";
    Screen::gotoXY(50, 25);
    std::cout << "-------------------------------";
    Screen::gotoXY(52, 27);
    std::cout << "USERNAME:";
    Screen::gotoXY(52, 30);
    std::cout << "PASSWORD:";
    Screen::gotoXY(50, 32);
    std::cout << "-------------------------------";

    Screen::gotoXY(62, 27);
    for(int i = 0; i < 100; ++i) std::cout << ' ';
    Screen::gotoXY(62, 30);
    for(int i = 0; i < 100; ++i) std::cout << ' ';

    Screen::showCursor(true);

    Screen::gotoXY(62, 27);
    user.ID.clear();
    getline(std::cin, user.ID);

    Screen::gotoXY(62, 30);
    user.password.clear();
    getline(std::cin, user.password);

    Screen::showCursor(false);
}

bool EntryMenu::menuController() {
    loadAssets("assets/game_title.txt", game_title);
    loadAssets("assets/menu_panel.txt", menu_panel);

    displayBackground(true);
    displayOptions();

    bool in_menu = true;
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
                        displayBackground(false);
                        loadUserData();

                        while (true) {
                            loginMenu();

                            bool validLogin = false;
                            for(int i = 0; i < number_user; ++i) {
                                if (user.ID == user_list[i].ID && user.password == user_list[i].password) {
                                    validLogin = true;
                                    user.score = user_list[i].score;
                                    break;
                                }
                            }

                            if (validLogin) break;
                            else {
                                Screen::setCursorColor(BLACK, RED);
                                Screen::gotoXY(53, 32);
                                std::cout << "WRONG USERNAME OR PASSWORD";
                                Screen::gotoXY(58, 33);
                                std::cout << "PLEASE RE-ENTER";

                                Sleep(500);

                                Screen::setCursorColor(BLACK, WHITE);
                                Screen::gotoXY(53, 32);
                                std::cout << "                          ";
                                Screen::gotoXY(58, 33);
                                std::cout << "               ";
                            }
                        }

                        return true;
                    }
                    case 1: {
                        Screen::clearConsole();
                        displayBackground(false);
                        loadUserData();

                        while (true) {
                            registerMenu();
                            int validRegister = 1;
                            for(int i = 0; i < (int)user.ID.length(); ++i) {
                                if (user.ID[i] == ' ') validRegister = -1;
                                break;
                            }
                            for(int i = 0; i < (int)user.password.length(); ++i) {
                                if (user.password[i] == ' ') validRegister = -1;
                                break;
                            }
                            if (validRegister == 1) {
                                for(int i = 0; i < number_user; ++i) {
                                    if (user.ID == user_list[i].ID) {
                                        validRegister = 0;
                                        break;
                                    }
                                }
                            }

                            if (validRegister == 1) {
                                saveUserData();
                                break;
                            }
                            else if (validRegister == 0) {
                                Screen::setCursorColor(BLACK, RED);
                                Screen::gotoXY(57, 34);
                                std::cout << "USERNAME WAS TAKEN";
                                Screen::gotoXY(58, 35);
                                std::cout << "PLEASE RE-ENTER";

                                Sleep(500);

                                Screen::setCursorColor(BLACK, WHITE);
                                Screen::gotoXY(57, 34);
                                std::cout << "                  ";
                                Screen::gotoXY(58, 35);
                                std::cout << "               ";
                            }
                            else {
                                Screen::setCursorColor(BLACK, RED);
                                Screen::gotoXY(49, 34);
                                std::cout << "WRONG USERNAME OR PASSWORD FORMAT";
                                Screen::gotoXY(58, 35);
                                std::cout << "PLEASE RE-ENTER";

                                Sleep(500);

                                Screen::setCursorColor(BLACK, WHITE);
                                Screen::gotoXY(49, 34);
                                std::cout << "                                 ";
                                Screen::gotoXY(58, 35);
                                std::cout << "               ";
                            }
                        }

                        return true;
                    }
                    default:
                        Screen::clearConsole();
                        exit(0);
                }
            case 1: {
                in_menu = false;
                break;
            }
            default:
                continue;
        }
    }

    return true;
}

void EntryMenu::loadAssets(const std::string &path, std::string *&dest) {
    std::fstream asset(path, std::ios::in);

    for(int id = 0; !asset.eof(); ++id) {
        getline(asset, dest[id]);
    }

    asset.close();
}

void EntryMenu::displayBackground(bool hasInfoPanel) {
    Screen::clearConsole();

    Screen::setCursorColor(BLACK, GREEN);

    int padding_left = 10, padding_top = 1; //(132 - 112) / 2 = 10
    Screen::gotoXY(padding_left, padding_top);

    for(int i = 0; i < 18; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout << game_title[i] << "\n";
    }

    if (hasInfoPanel) {
        Screen::setCursorColor(BLACK, LIGHT_WHITE);

        padding_left = 48, padding_top = 21; //(132 - 36) / 2 = 48

        for(int i = 0; i < 20; ++i) {
            Screen::gotoXY(padding_left, padding_top + i);
            std::cout << menu_panel[i] << "\n";
        }
    }
}

void EntryMenu::changeOption(int change) {
    int updatedOption = currentOption + change;
    if (updatedOption < 0 || updatedOption >= option_slot) return;

    unselectOption();

    currentOption = updatedOption;

    selectOption();
}

void EntryMenu::selectOption() {
    Screen::setCursorColor(BLACK, GREEN);

    int padding_left = 60, padding_top = 27;

    Screen::gotoXY(padding_left - 2, padding_top + currentOption * 4);
    putchar('>');

    Screen::gotoXY(padding_left + options[currentOption].length() + 1, padding_top + currentOption * 4);
    putchar('<');

    Screen::gotoXY(padding_left, padding_top + currentOption * 4);
    std::cout << options[currentOption];
}

void EntryMenu::unselectOption() {
    Screen::setCursorColor(BLACK, WHITE);

    int padding_left = 60, padding_top = 27;

    Screen::gotoXY(padding_left - 2, padding_top + currentOption * 4);
    putchar(' ');

    Screen::gotoXY(padding_left + options[currentOption].length() + 1, padding_top + currentOption * 4);
    putchar(' ');

    Screen::gotoXY(padding_left, padding_top + currentOption * 4);
    std::cout << options[currentOption];
}

void EntryMenu::displayOptions() {
    Screen::setCursorColor(BLACK, WHITE);

    int padding_left = 60, padding_top = 27;
    Screen::gotoXY(padding_left, padding_top);

    for(int i = 0; i < option_slot; ++i) {
        Screen::gotoXY(padding_left, padding_top + i * 4);
        std::cout << options[i] << "\n";
    }

    selectOption();
}
