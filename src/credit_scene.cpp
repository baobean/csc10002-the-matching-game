#include "credit_scene.h"

void CreditScene::displayCredit() {
    Screen::setCursorColor(BLACK, BLACK);
    Screen::clearConsole();

    Screen::setCursorColor(BLACK, YELLOW);
    Screen::gotoXY(59, 15);
    std::cout << "CREDITS";

    Screen::setCursorColor(BLACK, WHITE);
    Screen::gotoXY(51, 16);
    std::cout << "Nguyen Do Bao - 23127026";
    Screen::gotoXY(52, 17);
    std::cout << "Ha Bao Ngoc - 23217300";

    Screen::setCursorColor(BLACK, YELLOW);
    Screen::gotoXY(55, 20);
    std::cout << "PRESS ESC TO EXIT";

    Screen::setCursorColor(BLACK, WHITE);

    while (getch() != 27);
}

