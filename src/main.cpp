#include <iostream>

#include "menu_scene.h"

int main() {
    Screen::setUpConsole(18);
    playBackgroundMusic();

    Menu menu;

    menu.menuController(false);
    while (menu.menuController(true) == true);
    Screen::clearConsole();
    int n;
    std::cin >> n;
    return 0;
}
