#include "hard_mode.h"

HardMode::HardMode(Player _player, Player *_current_play) {
    gameTable = new TableLL(table_size_m, table_size_n);

    playerObj = new Player(_player);
    mistake = 5;
    hint = 3;

    if (_current_play == nullptr) {
        current_play.ID = playerObj->ID;
        current_play.password = playerObj->password;
        current_play.score = 0;
    }
    else {
        current_play.ID = _current_play->ID;
        current_play.password = _current_play->password;
        current_play.score = _current_play->score;
    }

    remained_pairs = table_size_m * table_size_n;

    table_image = new std::string[33];
    loadAssets("assets/stars.txt", table_image);
}

HardMode::~HardMode() {
    delete gameTable;
    gameTable = nullptr;

    delete playerObj;
    playerObj = nullptr;

    delete []table_image;
    table_image = nullptr;
}

std::pair<int, int> HardMode::getCellCoord(int _cell_pos_y, int _cell_pos_x) {
    return {10 + _cell_pos_x * (CELL_WIDTH + 1), 4 + _cell_pos_y * (CELL_HEIGHT + 1)};
}

void HardMode::selectCell(int _cell_pos_i, int _cell_pos_j, int _background) {
    std::pair<int, int> curr_cell_coord = getCellCoord(_cell_pos_i, _cell_pos_j);

    int origin_coord_x = curr_cell_coord.first - 3;
    int origin_coord_y = curr_cell_coord.second - 1;

    int cell_state = gameTable->getCellState(_cell_pos_i, _cell_pos_j);
    char cell_char = gameTable->getCellValue(_cell_pos_i, _cell_pos_j);

    Screen::setCursorColor(_background, WHITE);

    for(int i = origin_coord_x; i < origin_coord_x + CELL_WIDTH; ++i) {
        for(int j = origin_coord_y; j < origin_coord_y + CELL_HEIGHT; ++j) {
            Screen::gotoXY(i, j);

            if (cell_state == DELETED) {
                putchar(table_image[j - padding_top][i - padding_left]);
            }
            else {
                if (i == curr_cell_coord.first && j == curr_cell_coord.second) {
                    putchar(cell_char);
                }
                else putchar(' ');
            }
        }
    }
}

void HardMode::loadAssets(const std::string &path, std::string *&dest) {
    std::fstream asset(path, std::ios::in);

    for(int id = 0; id < 33; ++id) {
        getline(asset, dest[id]);
    }

    asset.close();
}

void HardMode::saveUserData(Player *current_player, int score) {
    std::fstream ofs("user_data/score_data.txt", std::ios::app);

    ofs << '\n';
    ofs << current_player->ID << '/' << current_player->password << '/' << score;

    ofs.close();
}

void HardMode::unselectCell() {
    std::pair<int, int> curr_cell_coord = getCellCoord(curr_cell_pos_x, curr_cell_pos_y);

    int origin_coord_x = curr_cell_coord.first - 3;
    int origin_coord_y = curr_cell_coord.second - 1;

    int cell_state = gameTable->getCellState(curr_cell_pos_x, curr_cell_pos_y);
    char cell_char = gameTable->getCellValue(curr_cell_pos_x, curr_cell_pos_y);

    if (cell_state == LOCKED) {
        Screen::setCursorColor(GREEN, WHITE);
    }
    else Screen::setCursorColor(BLACK, WHITE);

    for(int i = origin_coord_x; i < origin_coord_x + CELL_WIDTH; ++i) {
        for(int j = origin_coord_y; j < origin_coord_y + CELL_HEIGHT; ++j) {
            Screen::gotoXY(i, j);

            if (cell_state == DELETED) {
                putchar(table_image[j - padding_top][i - padding_left]);
            }
            else {
                if (i == curr_cell_coord.first && j == curr_cell_coord.second) {
                    putchar(cell_char);
                }
                else putchar(' ');
            }
        }
    }
}

void HardMode::lockCell() {
    int curr_cell_state = gameTable->getCellState(curr_cell_pos_x, curr_cell_pos_y);

    if (curr_cell_state == DELETED) return;

    if (curr_cell_state == LOCKED) {
        locked_cells.pop_back();
        gameTable->setCellState(curr_cell_pos_x,curr_cell_pos_y, FREE);
        selectCell(curr_cell_pos_x, curr_cell_pos_y, GREEN);
        return;
    }

    selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    gameTable->setCellState(curr_cell_pos_x, curr_cell_pos_y, LOCKED);
    locked_cells.push_back({curr_cell_pos_x, curr_cell_pos_y});

    if (locked_cells.size() == 2) {
        Sleep(100);
        deleteCell();
        Sleep(100);
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void HardMode::deleteCell() {
    std::pair<int, int> first_cell = locked_cells[0], second_cell = locked_cells[1];
    int result = checkMatching(first_cell, second_cell, true);
    if (result == false) {
        --mistake;
        for(auto curr_locked_cell : locked_cells) {
            gameTable->setCellState(curr_locked_cell.first, curr_locked_cell.second, FREE);
            selectCell(curr_locked_cell.first, curr_locked_cell.second, RED);
        }

        Sleep(300);

        for(auto curr_locked_cell : locked_cells) {
            selectCell(curr_locked_cell.first, curr_locked_cell.second, BLACK);
        }

        locked_cells.clear();
        playMismatchSound();
    }
    else {
        if (first_cell.first == second_cell.first) {
            if (first_cell.second > second_cell.second) std::swap(locked_cells[0], locked_cells[1]);
        }
        for(auto curr_locked_cell : locked_cells) {
            gameTable->setCellState(curr_locked_cell.first, curr_locked_cell.second, DELETED);
            selectCell(curr_locked_cell.first, curr_locked_cell.second, BLACK);
            gameTable->removeCell(curr_locked_cell.first, curr_locked_cell.second);
        }

        locked_cells.clear();
        gameTable->remained_cells -= 2;
        remained_pairs -= 2;
        displayTableData();
        playMatchSound();
    }
}

void HardMode::moveUp() {
    if (curr_cell_pos_x > 0) {
        unselectCell();
        --curr_cell_pos_x;
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void HardMode::moveDown() {
    if (curr_cell_pos_x < table_size_m - 1) {
        unselectCell();
        ++curr_cell_pos_x;
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void HardMode::moveRight() {
    if (curr_cell_pos_y < table_size_n - 1) {
        unselectCell();
        ++curr_cell_pos_y;
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void HardMode::moveLeft() {
    if (curr_cell_pos_y > 0) {
        unselectCell();
        --curr_cell_pos_y;
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void HardMode::displayInfoFrame() {
    Screen::setCursorColor(BLACK, WHITE);

    for(int i = 1; i <= 30; ++i) {
        Screen::gotoXY(80 + i, 2);
        putchar(196);
        Screen::gotoXY(80 + i, 13);
        putchar(196);
        Screen::gotoXY(80 + i, 19);
        putchar(196);
    }

    Screen::setCursorColor(BLACK, GREEN);
    Screen::gotoXY(92, 3);
    std::cout << "PLAYER";
    Screen::gotoXY(87, 14);
    std::cout << "GAME NOTIFICATION";
    Screen::gotoXY(90, 20);
    std::cout << "INSTRUCTIONS";

    Screen::setCursorColor(BLACK, LIGHT_AQUA);
    Screen::gotoXY(81, 5);
    std::cout << "USERNAME :";
    Screen::gotoXY(81, 7);
    std::cout << "SCORE    :";
    Screen::gotoXY(81, 9);
    std::cout << "MISTAKES :";
    Screen::gotoXY(81, 11);
    std::cout << "HINTS    :";


    Screen::setCursorColor(BLACK, YELLOW);
    Screen::gotoXY(81, 22);
    std::cout << "ARROWS";
    Screen::setCursorColor(BLACK, WHITE);
    std::cout << ": MOVE";

    Screen::setCursorColor(BLACK, YELLOW);
    Screen::gotoXY(81, 23);
    std::cout << "ENTER";
    Screen::setCursorColor(BLACK, WHITE);
    std::cout << ": SELECT/UNSELECT CELL";

    Screen::setCursorColor(BLACK, YELLOW);
    Screen::gotoXY(81, 24);
    std::cout << "H";
    Screen::setCursorColor(BLACK, WHITE);
    std::cout << ": MOVE SUGGESTION";

    Screen::setCursorColor(BLACK, YELLOW);
    Screen::gotoXY(81, 25);
    std::cout << "F";
    Screen::setCursorColor(BLACK, WHITE);
    std::cout << ": SHUFFLE";

    Screen::setCursorColor(BLACK, YELLOW);
    Screen::gotoXY(81, 26);
    std::cout << "ESC";
    Screen::setCursorColor(BLACK, WHITE);
    std::cout << ": EXIT";
}

void HardMode::displayPlayerInfo(int mistake, int hints) {
    Screen::setCursorColor(BLACK, LIGHT_BLUE);
    Screen::gotoXY(92, 5);
    std::cout << playerObj->ID;
    Screen::gotoXY(92, 7);
    std::cout << "     ";
    Screen::gotoXY(92, 7);
    std::cout << current_play.score;
    Screen::gotoXY(92, 9);
    std::cout << "     ";
    Screen::gotoXY(92, 9);
    std::cout << mistake;
    Screen::gotoXY(92, 11);
    std::cout << "     ";
    Screen::gotoXY(92, 11);
    std::cout << hints;
}

void HardMode::displayNotification(int _padding_left, int _padding_top, std::string text, int _text_color) {
    Screen::setCursorColor(BLACK, _text_color);
    Screen::gotoXY(_padding_left, _padding_top);
    std::cout << text;
}

void HardMode::displayTableBorder() {
    Screen::setCursorColor(BLACK, WHITE);
    Screen::clearConsole();

    for(int i = 0; i <= table_size_m; ++i) {
        for(int j = 0; j < table_size_n; ++j) {
            for(int k = 1; k <= CELL_WIDTH; ++k) {
                Screen::gotoXY(padding_left + (CELL_WIDTH + 1) * j + k, padding_top + i * CELL_HEIGHT + i);
                putchar(196);
            }
        }
    }

    for(int j = 0; j <= table_size_m; ++j) {
        for(int i = 0; i < table_size_n; ++i) {
            Screen::gotoXY(padding_left + j * CELL_WIDTH + j, padding_top + (CELL_HEIGHT + 1) * i);
            for(int k = 1; k <= CELL_HEIGHT; ++k) {
                Screen::gotoXY(padding_left + j * CELL_WIDTH + j, padding_top + (CELL_HEIGHT + 1) * i + k);
                putchar(179);
            }
        }
    }
}

void HardMode::displayCellData(int _cell_pos_x, int _cell_pos_y, int _background, int _text) {
    std::pair<int, int> curr_cell_coord = getCellCoord(_cell_pos_x, _cell_pos_y);

    int origin_coord_x = curr_cell_coord.first - 3;
    int origin_coord_y = curr_cell_coord.second - 1;

    int cell_state = gameTable->getCellState(_cell_pos_x, _cell_pos_y);
    char cell_char = gameTable->getCellValue(_cell_pos_x, _cell_pos_y);

    Screen::setCursorColor(_background, _text);

    for(int i = origin_coord_x; i < origin_coord_x + CELL_WIDTH; ++i) {
        for(int j = origin_coord_y; j < origin_coord_y + CELL_HEIGHT; ++j) {
            Screen::gotoXY(i, j);

            if (cell_state == DELETED) {
                putchar(table_image[j - padding_top][i - padding_left]);
            }
            else {
                if (i == curr_cell_coord.first && j == curr_cell_coord.second) {
                    putchar(cell_char);
                }
                else putchar(' ');
            }
        }
    }
}

void HardMode::displayTableData() {
    for(int i = 0; i < table_size_m; ++i) {
        for(int j = 0; j < table_size_n; ++j) {
            displayCellData(i, j, BLACK, WHITE);
        }
    }
}

void HardMode::setupTable() {
    gameTable->generateTableData();
    while (findValidPairs(false) == false) {
        gameTable->generateTableData();
    }
    displayTableBorder();
    displayTableData();
}

bool HardMode::checkMatchingTypeI(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if ((first_cell.first != second_cell.first) && (first_cell.second != second_cell.second)) {
        return false;
    }

    if (abs(first_cell.first - second_cell.first) == 1 || abs(first_cell.second - second_cell.second) == 1) {
        return true;
    }

    if (first_cell.first == second_cell.first) {
        if (first_cell.second > second_cell.second) std::swap(first_cell, second_cell);

        for(int y = first_cell.second + 1; y < second_cell.second; ++y) {
            int curr_state = gameTable->getCellState(first_cell.first, y);
            if (curr_state != DELETED) return false;
        }
    }
    else {
        if (first_cell.first > second_cell.first) std::swap(first_cell, second_cell);

        for (int x = first_cell.first + 1; x < second_cell.first; ++x) {
            int curr_state = gameTable->getCellState(x, first_cell.second);
            if (curr_state != DELETED) return false;
        }
    }

    return true;
}

bool HardMode::checkMatchingTypeL(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (checkEqualCharacter(first_cell, second_cell) == false) {
        return false;
    }

    if (first_cell.first == second_cell.first || first_cell.second == second_cell.second) {
        return false;
    }

    std::pair<int, int> corner_cell = {first_cell.first, second_cell.second};

    if (gameTable->getCellState(corner_cell.first, corner_cell.second) == DELETED) {
        bool first_path = checkMatchingTypeI(first_cell, corner_cell);
        bool second_path = checkMatchingTypeI(second_cell, corner_cell);

        if (first_path && second_path) return true;
    }

    corner_cell = {first_cell.second, second_cell.first};

    if (gameTable->getCellState(corner_cell.first, corner_cell.second) == DELETED) {
        bool first_path = checkMatchingTypeI(first_cell, corner_cell);
        bool second_path = checkMatchingTypeI(second_cell, corner_cell);

        if (first_path && second_path) return true;
    }

    return false;
}

bool HardMode::checkMatchingTypeU(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (first_cell.first == second_cell.first) {
        if (first_cell.first == 0 || first_cell.first == table_size_m - 1) return true;
    }
    if (first_cell.second == second_cell.second) {
        if (first_cell.second == 0 || first_cell.second == table_size_n - 1) return true;
    }

    if (first_cell.first == 0 || first_cell.first == table_size_m - 1) {
        std::pair<int, int> cellTemp = {first_cell.first, second_cell.second};
        int state = gameTable->getCellState(cellTemp.first, cellTemp.second);
        if (checkMatchingTypeI(second_cell, cellTemp) && state == DELETED) return true;
    }

    if (second_cell.first == 0 || second_cell.first == table_size_m - 1) {
        std::pair<int, int> cellTemp = {second_cell.first, first_cell.second};
        int state = gameTable->getCellState(cellTemp.first, cellTemp.second);
        if (checkMatchingTypeI(first_cell, cellTemp) && state == DELETED) return true;
    }

    if (first_cell.second == 0 || first_cell.second == table_size_n - 1) {
        std::pair<int, int> cellTemp = {second_cell.first, first_cell. second};
        int state = gameTable->getCellState(cellTemp.first, cellTemp.second);
        if (checkMatchingTypeI(second_cell, cellTemp) && state == DELETED) return true;
    }

    if (second_cell.second == 0 || second_cell.second == table_size_n - 1) {
        std::pair<int, int> cellTemp = {first_cell.first, second_cell.second};
        int state = gameTable->getCellState(cellTemp.first, cellTemp.second);
        if (checkMatchingTypeI(first_cell, cellTemp) && state == DELETED) return true;
    }

    int minX = std::min(first_cell.first, second_cell.first);
    int maxX = std::max(first_cell.first, second_cell.first);
    int minY = std::min(first_cell.second, second_cell.second);
    int maxY = std::max(first_cell.second, second_cell.second);

    for(int tempX = minX - 1; tempX >= 0; --tempX) {
        std::pair<int, int> cellA = {tempX, first_cell.second};
        std::pair<int, int> cellB = {tempX, second_cell.second};

        if (gameTable->getCellState(cellA.first, cellA.second) != DELETED || gameTable->getCellState(cellB.first, cellB.second) != DELETED) continue;

        if (tempX == 0) {
            bool firstPath = checkMatchingTypeI(first_cell, cellA);
            bool secondPath = checkMatchingTypeI(second_cell, cellB);

            if (firstPath && secondPath) return true;
        }
        else {
            bool firstPath = checkMatchingTypeI(first_cell, cellA);
            bool secondPath = checkMatchingTypeI(cellA, cellB);
            bool thirdPath = checkMatchingTypeI(cellB, second_cell);

            if (firstPath && secondPath && thirdPath) return true;
        }
    }

    for(int tempX = maxX + 1; tempX < table_size_m; ++tempX) {
        std::pair<int, int> cellA = {tempX, first_cell.second};
        std::pair<int, int> cellB = {tempX, second_cell.second};

        if (gameTable->getCellState(cellA.first, cellA.second) != DELETED || gameTable->getCellState(cellB.first, cellB.second) != DELETED) continue;

        if (tempX == table_size_m - 1) {
            bool firstPath = checkMatchingTypeI(first_cell, cellA);
            bool secondPath = checkMatchingTypeI(second_cell, cellB);

            if (firstPath && secondPath) return true;
        }
        else {
            bool firstPath = checkMatchingTypeI(first_cell, cellA);
            bool secondPath = checkMatchingTypeI(cellA, cellB);
            bool thirdPath = checkMatchingTypeI(cellB, second_cell);

            if (firstPath && secondPath && thirdPath) return true;
        }
    }

    for(int tempY = minY - 1; tempY >= 0; --tempY) {
        std::pair<int, int> cellA = {first_cell.first, tempY};
        std::pair<int, int> cellB = {second_cell.first, tempY};

        if (gameTable->getCellState(cellA.first, cellA.second) != DELETED || gameTable->getCellState(cellB.first, cellB.second) != DELETED) continue;

        if (tempY == 0) {
            bool firstPath = checkMatchingTypeI(first_cell, cellA);
            bool secondPath = checkMatchingTypeI(second_cell, cellB);

            if (firstPath && secondPath) return true;
        }
        else {
            bool firstPath = checkMatchingTypeI(first_cell, cellA);
            bool secondPath = checkMatchingTypeI(cellA, cellB);
            bool thirdPath = checkMatchingTypeI(cellB, second_cell);

            if (firstPath && secondPath && thirdPath) return true;
        }
    }

    for(int tempY = maxY + 1; tempY < table_size_n; ++tempY) {
        std::pair<int, int> cellA = {first_cell.first, tempY};
        std::pair<int, int> cellB = {second_cell.first, tempY};

        if (gameTable->getCellState(cellA.first, cellA.second) != DELETED || gameTable->getCellState(cellB.first, cellB.second) != DELETED) continue;

        if (tempY == table_size_n - 1) {
            bool firstPath = checkMatchingTypeI(first_cell, cellA);
            bool secondPath = checkMatchingTypeI(second_cell, cellB);

            if (firstPath && secondPath) return true;
        }
        else {
            bool firstPath = checkMatchingTypeI(first_cell, cellA);
            bool secondPath = checkMatchingTypeI(cellA, cellB);
            bool thirdPath = checkMatchingTypeI(cellB, second_cell);

            if (firstPath && secondPath && thirdPath) return true;
        }
    }
    return 0;
}

bool HardMode::checkMatchingTypeZ(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (first_cell.second > second_cell.second) std::swap(first_cell, second_cell);

    for(int tempY = first_cell.second + 1; tempY < second_cell.second; ++tempY) {
        std::pair<int, int> cellA = {first_cell.first, tempY};
        std::pair<int, int> cellB = {second_cell.first, tempY};

        if (gameTable->getCellState(cellA.first, cellA.second) != DELETED || gameTable->getCellState(cellB.first, cellB.second) != DELETED) continue;

        bool firstPath = checkMatchingTypeI(first_cell, cellA);
        bool secondPath = checkMatchingTypeI(cellA, cellB);
        bool thirdPath = checkMatchingTypeI(cellB, second_cell);

        if (firstPath && secondPath && thirdPath) return true;
    }

    if (first_cell.first > second_cell.first) std::swap(first_cell, second_cell);

    for(int tempX = first_cell.first + 1; tempX < second_cell.first; ++tempX) {
        std::pair<int, int> cellA = {tempX, first_cell.second};
        std::pair<int, int> cellB = {tempX, second_cell.second};

        if (gameTable->getCellState(cellA.first, cellA.second) != DELETED || gameTable->getCellState(cellB.first, cellB.second) != DELETED) continue;

        bool firstPath = checkMatchingTypeI(first_cell, cellA);
        bool secondPath = checkMatchingTypeI(cellA, cellB);
        bool thirdPath = checkMatchingTypeI(cellB, second_cell);

        if (firstPath && secondPath && thirdPath) return true;
    }

    return false;
}

bool HardMode::checkEqualCharacter(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    char first_cell_value = gameTable->getCellValue(first_cell.first, first_cell.second);
    char second_cell_value = gameTable->getCellValue(second_cell.first, second_cell.second);

    if (first_cell_value == second_cell_value) return true;
    return false;
}

int HardMode::checkMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool add_points) {
    if (checkEqualCharacter(first_cell, second_cell) == false) return 0;

    if (checkMatchingTypeI(first_cell, second_cell)) {
        if (add_points) current_play.score += 3;
        return 1;
    }

    if (checkMatchingTypeL(first_cell, second_cell)) {
        if (add_points) current_play.score += 5;
        return 2;
    }

    if (checkMatchingTypeU(first_cell, second_cell)) {
        if (add_points) current_play.score += 8;
        return 3;
    }

    if (checkMatchingTypeZ(first_cell, second_cell)) {
        if (add_points) current_play.score += 8;
        return 4;
    }

    return 0;
}

bool HardMode::findValidPairs(bool isDisplay) {
    for(int xA = 0; xA < table_size_m; ++xA) {
        for(int yA = 0; yA < table_size_n; ++yA) {
            if (gameTable->getCellState(xA, yA) == DELETED) continue;

            for(int xB = xA; xB < table_size_m; ++xB) {
                for(int yB = 0; yB < table_size_n; ++yB) {
                    if (xB == xA && yB <= yA) continue;
                    if (gameTable->getCellState(xB, yB) == DELETED) continue;

                    if (checkMatching({xA, yA}, {xB, yB}, false)) {
                        if (isDisplay) {
                            selectCell(xA, yA, PURPLE);
                            selectCell(xB, yB, PURPLE);
                            Sleep(500);
                            selectCell(xA, yA, BLACK);
                            selectCell(xB, yB, BLACK);

                            selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
                        }
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Player HardMode::gameplay() {
    Screen::clearConsole();
    setupTable();
    displayInfoFrame();
    selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);

    bool in_game = true;
    while (in_game && mistake > 0 && current_play.score >= 0 && remained_pairs > 0) {
        displayPlayerInfo(mistake, hint);

        if (findValidPairs(false) == false && current_play.score < 10) {
            in_game = false;
            displayNotification(83, 16, "NO MOVES LEFT - GAME ENDS", LIGHT_RED);
            Sleep(3000);
            displayNotification(83, 16, "                         ", WHITE);
            continue;
        }

        switch (Screen::getConsoleInput()) {
            case 1:
                in_game = false;
                break;
            case 2:
                moveUp();
                playMoveSound();
                break;
            case 3:
                moveDown();
                playMoveSound();
                break;
            case 4:
                moveLeft();
                playMoveSound();
                break;
            case 5:
                moveRight();
                playMoveSound();
                break;
            case 6:
                playHintSound();
                lockCell();
                break;
            case 7:
                if (hint > 0) {
                    playHintSound();
                    findValidPairs(true);
                    --hint;
                }
                else {
                    displayNotification(90, 16, "NO HINTS LEFT", LIGHT_RED);
                    playMismatchSound();
                    Sleep(500);
                    displayNotification(90, 16, "             ", WHITE);
                }
                break;
            case 8:
                if (current_play.score < 10) {
                    displayNotification(87, 16, "NOT ENOUGH POINTS", LIGHT_RED);
                    playMismatchSound();
                    Sleep(500);
                    displayNotification(87, 16, "                 ", WHITE);
                }
                else {
                    playShuffleSound();
                    current_play.score -= 10;
                    gameTable->shuffleTableData();
                    while (findValidPairs(false) == false) {
                        gameTable->shuffleTableData();
                    }
                    displayTableData();
                    selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
                    displayNotification(91, 16, "SHUFFLED", YELLOW);
                    Sleep(500);
                    displayNotification(91, 16, "        ", WHITE);
                }
                break;
            case 9:
                displayNotification(84, 16, "CANNOT SAVE IN HARD MODE", LIGHT_RED);
                Sleep(500);
                displayNotification(84, 16, "                        ", WHITE);
                break;
            default:
                break;
        }
    }

    playerObj->score = std::max(playerObj->score, current_play.score);

    saveUserData(playerObj, current_play.score);

    Screen::setCursorColor(BLACK, WHITE);
    return current_play;
}

