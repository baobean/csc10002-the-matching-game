#include "instruction_scene.h"

void InstructionScene::displayInstruction() {
    Screen::setCursorColor(BLACK, BLACK);
    Screen::clearConsole();

    auto printTitle = [](const int padding_left, const int padding_top, const std::string &text) {
        Screen::gotoXY(padding_left, padding_top);
        std::cout << text;
    };

    auto printText = [](const int _background, const int _color, const std::string &text) {
        Screen::setCursorColor(_background, _color);
        std::cout << text;
    };

    Screen::setCursorColor(BLACK, YELLOW);
    printTitle(61, 3, "GAME RULES");
    Screen::setCursorColor(BLACK, WHITE);
    printTitle(10, 4, "The Matching Game includes a board of multiple cells, each of which presents a figure. The player has to find");
    printTitle(10, 5, "and match a pair of cells that contain the same figure and connect each other in some particular pattern.");
    printTitle(10, 6, "The game ends when all matching pairs are found or the player run out of points or allowed mistakes or moves.");

    Screen::setCursorColor(BLACK, YELLOW);
    printTitle(61, 9, "HOW TO PLAY");
    Screen::setCursorColor(BLACK, WHITE);
    printTitle(10, 10, "Press ");
    Screen::setCursorColor(BLACK, GREEN); std::cout<<"Arrow Keys ";
    printText(BLACK, WHITE, "to move around the board");

    Screen::setCursorColor(BLACK, WHITE);
    printTitle(10, 11, "Press ");
    printText(BLACK, GREEN, "Enter "); printText(BLACK, WHITE, "to select//unselect the cell");

    Screen::setCursorColor(BLACK, WHITE);
    printTitle(10, 12, "Press ");
    printText(BLACK, GREEN, "H "); printText(BLACK, WHITE, "to display move suggestion, the limit of allowed suggestions depends on the game mode");

    Screen::setCursorColor(BLACK, WHITE);
    printTitle(10, 13, "Press ");
    printText(BLACK, GREEN, "F "); printText(BLACK, WHITE, "to shuffle the board, which costs the player "); printText(BLACK, LIGHT_RED, "10 points");

    Screen::setCursorColor(BLACK, WHITE);
    printTitle(10, 14, "Press ");
    printText(BLACK, GREEN, "S "); printText(BLACK, WHITE, "to save the game, which cannot be used in "); printText(BLACK, PURPLE, "HARD MODE");

    Screen::setCursorColor(BLACK, WHITE);
    printTitle(10, 15, "Press ");
    printText(BLACK, GREEN, "ESC "); printText(BLACK, WHITE, "to return to the main menu screen");

    Screen::setCursorColor(BLACK, WHITE);
    printTitle(10, 17, "When the player mismatched, the allowed mistakes will decrease, the limit of mistakes depends on the game mode");

    printTitle(10, 19, "The player's score will be displayed on the leaderboard for ");
    printText(BLACK, PURPLE, "HARD MODE "); printText(BLACK, WHITE, "only");

    Screen::setCursorColor(BLACK, YELLOW);
    printTitle(61, 21, "SCORING");
    Screen::setCursorColor(BLACK, PURPLE);
    printTitle(53, 22, "3 points "); printText(BLACK, WHITE, "for "); printText(BLACK, GREEN, "I Matching");
    Screen::setCursorColor(BLACK, PURPLE);
    printTitle(53, 23, "5 points "); printText(BLACK, WHITE, "for "); printText(BLACK, GREEN, "L Matching");
    Screen::setCursorColor(BLACK, PURPLE);
    printTitle(53, 24, "8 points "); printText(BLACK, WHITE, "for "); printText(BLACK, GREEN, "U Matching");
    Screen::setCursorColor(BLACK, PURPLE);
    printTitle(53, 25, "8 points "); printText(BLACK, WHITE, "for "); printText(BLACK, GREEN, "Z Matching");

    Screen::setCursorColor(BLACK, YELLOW);
    printTitle(56, 27, "PRESS ESC TO EXIT");

    Screen::setCursorColor(BLACK, WHITE);

    while (getch() != 27);
}

