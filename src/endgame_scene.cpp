#include "endgame_scene.h"

bool EndgameMenu::menuController(Player current_play) {
    title = new std::string[13];
    loadAssets("assets/game_over.txt", title);

    displayBackground();

    Screen::setCursorColor(BLACK, LIGHT_AQUA);
    Screen::gotoXY(61, 20);
    std::cout << "POINTS: ";
    Screen::gotoXY(69, 20);
    std::cout << current_play.score;

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
                playHintSound();
                in_menu = false;
                switch (currentOption) {
                    case 0:
                        delete []title;
                        title = nullptr;
                        return true;
                    case 1:
                        delete []title;
                        title = nullptr;
                        return false;
                }
        }
    }
    return false;
}

void EndgameMenu::loadAssets(const std::string &path, std::string *&dest) {
    std::fstream asset(path, std::ios::in);

    for(int id = 0; !asset.eof(); ++id) {
        getline(asset, dest[id]);
    }

    asset.close();
}

void EndgameMenu::displayBackground() {
    Screen::clearConsole();

    Screen::setCursorColor(BLACK, GREEN);

    int padding_left = 42, padding_top = 5;

    for(int i = 0; i < 13; ++i) {
        Screen::gotoXY(padding_left, padding_top + i);
        std::cout << title[i] << "\n";
    }
}

void EndgameMenu::changeOption(int change) {
    int updatedOption = currentOption + change;
    if (updatedOption < 0 || updatedOption >= option_slot) return;

    unselectOption();

    currentOption = updatedOption;

    selectOption();
}

void EndgameMenu::selectOption() {
    Screen::setCursorColor(BLACK, GREEN);

    int padding_left = 61, padding_top = 22;

    Screen::gotoXY(padding_left - 2, padding_top + currentOption * 2);
    putchar('>');

    Screen::gotoXY(padding_left + options[currentOption].length() + 1, padding_top + currentOption * 2);
    putchar('<');

    Screen::gotoXY(padding_left, padding_top + currentOption * 2);
    std::cout << options[currentOption];
}

void EndgameMenu::unselectOption() {
    Screen::setCursorColor(BLACK, WHITE);

    int padding_left = 61, padding_top = 22;

    Screen::gotoXY(padding_left - 2, padding_top + currentOption * 2);
    putchar(' ');

    Screen::gotoXY(padding_left + options[currentOption].length() + 1, padding_top + currentOption * 2);
    putchar(' ');

    Screen::gotoXY(padding_left, padding_top + currentOption * 2);
    std::cout << options[currentOption];
}

void EndgameMenu::displayOptions() {
    Screen::setCursorColor(BLACK, WHITE);

    int padding_left = 61, padding_top = 22;

    for(int i = 0; i < option_slot; ++i) {
        Screen::gotoXY(padding_left, padding_top + i * 2);
        std::cout << options[i] << "\n";
    }

    selectOption();
}
