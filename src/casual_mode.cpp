#include "casual_mode.h"

CasualMode::CasualMode(Player _player, Player *_current_play, Table *_gameTable) {
    if (_gameTable == nullptr) gameTable = new Table(table_size_x, table_size_y);
    else {
        table_loaded = true;
        gameTable = _gameTable;
    }

    playerObj = new Player(_player);
    mistake = 10;
    hint = 10;

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

    remained_pairs = table_size_x * table_size_y;
    mistake = 10;

    table_image = new std::string[24];
    loadAssets("assets/stars.txt", table_image);
}

CasualMode::~CasualMode() {
    delete gameTable;
    gameTable = nullptr;

    delete playerObj;
    playerObj = nullptr;

    delete []table_image;
    table_image = nullptr;
}

void CasualMode::selectCell(int _cell_pos_x, int _cell_pos_y, int _background) {
    int curr_cell_coord_x = gameTable->table_data[_cell_pos_x][_cell_pos_y].cell_coord_x;
    int curr_cell_coord_y = gameTable->table_data[_cell_pos_x][_cell_pos_y].cell_coord_y;
    int origin_coord_x = curr_cell_coord_x - 3;
    int origin_coord_y = curr_cell_coord_y - 1;
    int cell_state = gameTable->table_data[_cell_pos_x][_cell_pos_y].cell_state;
    char cell_char = gameTable->table_data[_cell_pos_x][_cell_pos_y].cell_value;
    Screen::setCursorColor(_background, WHITE);

    for(int i = origin_coord_x; i < origin_coord_x + CELL_WIDTH; ++i) {
        for(int j = origin_coord_y; j < origin_coord_y + CELL_HEIGHT; ++j) {
            Screen::gotoXY(i, j);

            if (cell_state == DELETED) {
                putchar(table_image[j - padding_top][i - padding_left]);
            }
            else {
                if (i == curr_cell_coord_x && j == curr_cell_coord_y) {
                    putchar(cell_char);
                }
                else putchar(' ');
            }
        }
    }
}

void CasualMode::loadAssets(const std::string &path, std::string *&dest) {
    std::fstream asset(path, std::ios::in);

    for(int id = 0; id < 24; ++id) {
        getline(asset, dest[id]);
    }

    asset.close();
}

void CasualMode::unselectCell() {
    int curr_cell_coord_x = gameTable->table_data[curr_cell_pos_x][curr_cell_pos_y].cell_coord_x;
    int curr_cell_coord_y = gameTable->table_data[curr_cell_pos_x][curr_cell_pos_y].cell_coord_y;
    int origin_coord_x = curr_cell_coord_x - 3;
    int origin_coord_y = curr_cell_coord_y - 1;
    int curr_cell_state = gameTable->table_data[curr_cell_pos_x][curr_cell_pos_y].cell_state;
    char cell_char = gameTable->table_data[curr_cell_pos_x][curr_cell_pos_y].cell_value;

    Screen::gotoXY(curr_cell_coord_x, curr_cell_coord_y);

    if (curr_cell_state == LOCKED) {
        Screen::setCursorColor(GREEN, WHITE);
    }
    else Screen::setCursorColor(BLACK, WHITE);

    for(int i = origin_coord_x; i < origin_coord_x + CELL_WIDTH; ++i) {
        for(int j = origin_coord_y; j < origin_coord_y + CELL_HEIGHT; ++j) {
            Screen::gotoXY(i, j);

            if (curr_cell_state == DELETED) {
                putchar(table_image[j - padding_top][i - padding_left]);
            }
            else {
                if (i == curr_cell_coord_x && j == curr_cell_coord_y) {
                    putchar(cell_char);
                }
                else putchar(' ');
            }
        }
    }
}

void CasualMode::lockCell() {
    int curr_cell_state = gameTable->table_data[curr_cell_pos_x][curr_cell_pos_y].cell_state;

    if (curr_cell_state == DELETED) return;

    if (curr_cell_state == LOCKED) {
        locked_cells.pop_back();
        gameTable->table_data[curr_cell_pos_x][curr_cell_pos_y].cell_state = FREE;
        selectCell(curr_cell_pos_x, curr_cell_pos_y, GREEN);
        return;
    }

    selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    gameTable->table_data[curr_cell_pos_x][curr_cell_pos_y].cell_state = LOCKED;
    locked_cells.push_back({curr_cell_pos_x, curr_cell_pos_y});

    if (locked_cells.size() == 2) {
        Sleep(100);
        deleteCell();
        Sleep(100);
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void CasualMode::deleteCell() {
    std::pair<int, int> first_cell = locked_cells[0], second_cell = locked_cells[1];
    int result = checkMatching(first_cell, second_cell, true);
    if (result == false) {
        --mistake;
        for(auto curr_locked_cell : locked_cells) {
            gameTable->table_data[curr_locked_cell.first][curr_locked_cell.second].cell_state = FREE;
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
        for(auto curr_locked_cell : locked_cells) {
            gameTable->table_data[curr_locked_cell.first][curr_locked_cell.second].cell_state = DELETED;
            selectCell(curr_locked_cell.first, curr_locked_cell.second, BLACK);
        }

        locked_cells.clear();
        gameTable->remained_cells -= 2;
        remained_pairs -= 2;
        playMatchSound();
    }
}

void CasualMode::moveUp() {
    if (curr_cell_pos_x > 0) {
        unselectCell();
        --curr_cell_pos_x;
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void CasualMode::moveDown() {
    if (curr_cell_pos_x < table_size_x - 1) {
        unselectCell();
        ++curr_cell_pos_x;
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void CasualMode::moveRight() {
    if (curr_cell_pos_y < table_size_y - 1) {
        unselectCell();
        ++curr_cell_pos_y;
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void CasualMode::moveLeft() {
    if (curr_cell_pos_y > 0) {
        unselectCell();
        --curr_cell_pos_y;
        selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
    }
}

void CasualMode::displayInfoFrame() {
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
    std::cout << "S";
    Screen::setCursorColor(BLACK, WHITE);
    std::cout << ": SAVE";

    Screen::setCursorColor(BLACK, YELLOW);
    Screen::gotoXY(81, 27);
    std::cout << "ESC";
    Screen::setCursorColor(BLACK, WHITE);
    std::cout << ": EXIT";
}

void CasualMode::displayPlayerInfo(int mistake, int hints) {
    Screen::setCursorColor(BLACK, LIGHT_BLUE);
    Screen::gotoXY(92, 5);
    std::cout << playerObj->ID;
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

void CasualMode::displayNotification(int _padding_left, int _padding_top, std::string text, int _text_color) {
    Screen::setCursorColor(BLACK, _text_color);
    Screen::gotoXY(_padding_left, _padding_top);
    std::cout << text;
}

void CasualMode::displayTableBorder() {
    Screen::setCursorColor(BLACK, WHITE);
    Screen::clearConsole();

    for(int i = 0; i <= table_size_x; ++i) {
        for(int j = 0; j < table_size_y; ++j) {
            for(int k = 1; k <= CELL_WIDTH; ++k) {
                Screen::gotoXY(padding_left + (CELL_WIDTH + 1) * j + k, padding_top + i * CELL_HEIGHT + i);
                putchar(196);
            }
        }
    }

    for(int j = 0; j <= table_size_y; ++j) {
        for(int i = 0; i < table_size_x; ++i) {
            Screen::gotoXY(padding_left + j * CELL_WIDTH + j, padding_top + (CELL_HEIGHT + 1) * i);
            for(int k = 1; k <= CELL_HEIGHT; ++k) {
                Screen::gotoXY(padding_left + j * CELL_WIDTH + j, padding_top + (CELL_HEIGHT + 1) * i + k);
                putchar(179);
            }
        }
    }
}

void CasualMode::displayCellData(int _cell_pos_x, int _cell_pos_y, int _cell_coord_x, int _cell_coord_y, int _background, int _text) {
    int origin_coord_x = _cell_coord_x - 3;
    int origin_coord_y = _cell_coord_y - 1;
    int cell_state = gameTable->table_data[_cell_pos_x][_cell_pos_y].cell_state;
    char cell_char = gameTable->table_data[_cell_pos_x][_cell_pos_y].cell_value;

    Screen::setCursorColor(_background, _text);

    for(int i = origin_coord_x; i < origin_coord_x + CELL_WIDTH; ++i) {
        for(int j = origin_coord_y; j < origin_coord_y + CELL_HEIGHT; ++j) {
            Screen::gotoXY(i, j);

            if (cell_state == DELETED) {
                putchar(table_image[j - padding_top][i - padding_left]);
            }
            else {
                if (i == _cell_coord_x && j == _cell_coord_y) {
                    putchar(cell_char);
                }
                else putchar(' ');
            }
        }
    }
}

void CasualMode::displayTableData() {
    for(int i = 0; i < table_size_x; ++i) {
        for(int j = 0; j < table_size_y; ++j) {
            int _cell_coord_x = gameTable->table_data[i][j].cell_coord_x;
            int _cell_coord_y = gameTable->table_data[i][j].cell_coord_y;
            displayCellData(i, j, _cell_coord_x, _cell_coord_y, BLACK, WHITE);
        }
    }
}

void CasualMode::setupTable() {
    if (table_loaded == false) {
        gameTable = new Table(table_size_x, table_size_y);
        gameTable->generateTableData();
    }
    displayTableBorder();
    displayTableData();
}

int CasualMode::getCellState(int _cell_pos_x, int _cell_pos_y) {
    return gameTable->table_data[_cell_pos_x][_cell_pos_y].cell_state;
}

bool CasualMode::checkMatchingTypeI(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool flag = false) {
    int xA = first_cell.first;
    int yA = first_cell.second;
    int xB = second_cell.first;
    int yB = second_cell.second;

    if ((xA != xB) && (yA != yB)) {
        return false;
    }
    if (!flag) {
        if (checkEqualCharacter(first_cell, second_cell) == false) {
            return false;
        }
    }
    if (abs(xA - xB) == 1 || abs(yA - yB) == 1) {
        return true;
    }

    const int xMin = std::min(xA, xB);
    const int yMin = std::min(yA, yB);
    const int xMax = std::max(xA, xB);
    const int yMax = std::max(yA, yB);

    if (xA == xB) {
        for (int i = yMin + 1; i < yMax; i++) {
            int curr_state = gameTable->table_data[xA][i].cell_state;
            if (curr_state != DELETED) {
                return false;
            }
        }
    }
    else {
        for (int i = xMin + 1; i < xMax; i++) {
            int curr_state = gameTable->table_data[i][yA].cell_state;
            if (curr_state != DELETED) {
                return false;
            }
        }
    }

    return true;
}

void CasualMode::displayMatchingI(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
    if (first_cell.first == second_cell.first) {
        int start_point = first_cell.second;
        int end_point = second_cell.second;
        if (start_point > end_point) std::swap(start_point, end_point);

        int y = gameTable->getYInConsole(first_cell.first);
        start_point = gameTable->getXInConsole(start_point);
        end_point = gameTable->getXInConsole(end_point);

        if (overwrite == false) {
            Screen::setCursorColor(GREEN, WHITE);
        }
        else {
            Screen::setCursorColor(BLACK, WHITE);
        }

        for (int current_pos_x = start_point + 1; current_pos_x < end_point; ++current_pos_x) {
            Screen::gotoXY(current_pos_x, y);

            if (overwrite == false) {
                putchar(196);
            }
            else {
                if (current_pos_x % (CELL_WIDTH + 1) == 6) {
                    putchar(179);
                }
                else {
                    putchar(32);
                }
            }
		}

		return;
    }

    if (first_cell.second == second_cell.second) {
        int start_point = first_cell.first;
        int end_point = second_cell.first;
        if (start_point > end_point) std::swap(start_point, end_point);

        int x = gameTable->getXInConsole(first_cell.second);
        start_point = gameTable->getYInConsole(start_point);
        end_point = gameTable->getYInConsole(end_point);

        if (overwrite == false) {
            Screen::setCursorColor(GREEN, WHITE);
        }
        else {
            Screen::setCursorColor(BLACK, WHITE);
        }

        for (int current_pos_y = start_point + 1; current_pos_y < end_point; ++current_pos_y) {
            Screen::gotoXY(x, current_pos_y);

            if (overwrite == false) {
                putchar(179);
            }
            else {
                if (current_pos_y % (CELL_HEIGHT + 1) == 2) {
                    putchar(196);
                }
                else {
                    putchar(32);
                }
            }
        }

        return;
    }
}

bool CasualMode::checkMatchingTypeL(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    int xA = first_cell.first;
    int yA = first_cell.second;
    int xB = second_cell.first;
    int yB = second_cell.second;

    if (checkEqualCharacter(first_cell, second_cell) == false) {
        return false;
    }

    if (xA == xB || yA == yB) {
        return false;
    }

    if (xA > xB) {
        std::swap(xA, xB);
        std::swap(yA, yB);
    }


    const int xMin = std::min(xA, xB);
    const int yMin = std::min(yA, yB);
    const int xMax = std::max(xA, xB);
    const int yMax = std::max(yA, yB);
    std::pair<int, int>temp;
    if (yA < yB) {
        temp.first = xMax;
        temp.second = yMin;
        if(getCellState(xMax,yMin)==DELETED){
            if (checkMatchingTypeI(first_cell, temp, true) && checkMatchingTypeI(temp, second_cell, true)) {
                return true;
            }
        }
        else {
            temp.first = xMin; temp.second = yMax;
            if(getCellState(xMin,yMax)==DELETED){
                if (checkMatchingTypeI(first_cell, temp, true) && checkMatchingTypeI(temp, second_cell, true)) {
                    return true;
                }
            }
            else return false;
        }
    }
    else {
        temp.first = xMax;
        temp.second = yMax;
        if(getCellState(xMax,yMax)==DELETED){
            if (checkMatchingTypeI(first_cell, temp, true) && checkMatchingTypeI(temp, second_cell, true)) {
                return true;
            }
        }
        else {
            temp.first = xMin; temp.second = yMin;
            if(getCellState(xMin,yMin)==DELETED){
            if (checkMatchingTypeI(first_cell, temp, true) && checkMatchingTypeI(temp, second_cell, true)) {
                return true;
            }
            else return false;
            }
        }
    }
    return false;
}

void CasualMode::displayMatchingL(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
    std::pair<int, int> corner_cell = {first_cell.first, second_cell.second};
    if (getCellState(corner_cell.first, corner_cell.second) == DELETED) {
        if (checkMatchingTypeI(first_cell, corner_cell, true) == true && checkMatchingTypeI(second_cell, corner_cell, true) == true) {
            displayMatchingI(first_cell, corner_cell, overwrite);

            Screen::gotoXY(gameTable->table_data[corner_cell.first][corner_cell.second].cell_coord_x,
                           gameTable->table_data[corner_cell.first][corner_cell.second].cell_coord_y);
            if (overwrite == false) {
                std::cout<<'x';
            } else {
                std::cout<<" ";
            }

            displayMatchingI(second_cell, corner_cell, overwrite);
            return;
        }
    }

    corner_cell = {second_cell.first, first_cell.second};
    if (getCellState(corner_cell.first, corner_cell.second) == DELETED) {
        if (checkMatchingTypeI(first_cell, corner_cell, true) == true && checkMatchingTypeI(second_cell, corner_cell, true) == true) {
            displayMatchingI(first_cell, corner_cell, overwrite);

            Screen::gotoXY(gameTable->table_data[corner_cell.first][corner_cell.second].cell_coord_x,
                           gameTable->table_data[corner_cell.first][corner_cell.second].cell_coord_y);
            if (overwrite == false) {
                std::cout<<'x';
            } else {
                std::cout<<" ";
            }

            displayMatchingI(second_cell, corner_cell, overwrite);
            return;
        }
    }
}

int CasualMode::checkMatchingTypeU(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (first_cell.first == second_cell.first) {
        if (first_cell.first == 0 || first_cell.first == table_size_x - 1) return 101;
    }
    if (first_cell.second == second_cell.second) {
        if (first_cell.second == 0 || first_cell.second == table_size_y - 1) return 102;
    }

    if (first_cell.first == 0 || first_cell.first == table_size_x - 1) {
        std::pair<int, int> cellTemp = {first_cell.first, second_cell.second};
        int state = getCellState(cellTemp.first, cellTemp.second);
        if (checkMatchingTypeI(second_cell, cellTemp, true) && state == DELETED) return 201;
    }

    if (second_cell.first == 0 || second_cell.first == table_size_x - 1) {
        std::pair<int, int> cellTemp = {second_cell.first, first_cell.second};
        int state = getCellState(cellTemp.first, cellTemp.second);
        if (checkMatchingTypeI(first_cell, cellTemp, true) && state == DELETED) return 201;
    }

    if (first_cell.second == 0 || first_cell.second == table_size_y - 1) {
        std::pair<int, int> cellTemp = {second_cell.first, first_cell. second};
        int state = getCellState(cellTemp.first, cellTemp.second);
        if (checkMatchingTypeI(second_cell, cellTemp, true) && state == DELETED) return 202;
    }

    if (second_cell.second == 0 || second_cell.second == table_size_y - 1) {
        std::pair<int, int> cellTemp = {first_cell.first, second_cell.second};
        int state = getCellState(cellTemp.first, cellTemp.second);
        if (checkMatchingTypeI(first_cell, cellTemp, true) && state == DELETED) return 202;
    }

    int minX = std::min(first_cell.first, second_cell.first);
    int maxX = std::max(first_cell.first, second_cell.first);
    int minY = std::min(first_cell.second, second_cell.second);
    int maxY = std::max(first_cell.second, second_cell.second);

    for(int tempX = minX - 1; tempX >= 0; --tempX) {
        std::pair<int, int> cellA = {tempX, first_cell.second};
        std::pair<int, int> cellB = {tempX, second_cell.second};

        if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

        if (tempX == 0) {
            bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
            bool secondPath = checkMatchingTypeI(second_cell, cellB, true);

            if (firstPath && secondPath) return 301;
        }
        else {
            bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
            bool secondPath = checkMatchingTypeI(cellA, cellB, true);
            bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

            if (firstPath && secondPath && thirdPath) return 301;
        }
    }

    for(int tempX = maxX + 1; tempX < table_size_x; ++tempX) {
        std::pair<int, int> cellA = {tempX, first_cell.second};
        std::pair<int, int> cellB = {tempX, second_cell.second};

        if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

        if (tempX == table_size_x - 1) {
            bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
            bool secondPath = checkMatchingTypeI(second_cell, cellB, true);

            if (firstPath && secondPath) return 302;
        }
        else {
            bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
            bool secondPath = checkMatchingTypeI(cellA, cellB, true);
            bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

            if (firstPath && secondPath && thirdPath) return 302;
        }
    }

    for(int tempY = minY - 1; tempY >= 0; --tempY) {
        std::pair<int, int> cellA = {first_cell.first, tempY};
        std::pair<int, int> cellB = {second_cell.first, tempY};

        if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

        if (tempY == 0) {
            bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
            bool secondPath = checkMatchingTypeI(second_cell, cellB, true);

            if (firstPath && secondPath) return 303;
        }
        else {
            bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
            bool secondPath = checkMatchingTypeI(cellA, cellB, true);
            bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

            if (firstPath && secondPath && thirdPath) return 303;
        }
    }

    for(int tempY = maxY + 1; tempY < table_size_y; ++tempY) {
        std::pair<int, int> cellA = {first_cell.first, tempY};
        std::pair<int, int> cellB = {second_cell.first, tempY};

        if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

        if (tempY == table_size_y - 1) {
            bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
            bool secondPath = checkMatchingTypeI(second_cell, cellB, true);

            if (firstPath && secondPath) return 304;
        }
        else {
            bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
            bool secondPath = checkMatchingTypeI(cellA, cellB, true);
            bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

            if (firstPath && secondPath && thirdPath) return 304;
        }
    }
    return 0;
}

void CasualMode::displayMatchingU(std::pair<int, int> first_cell, std::pair<int, int> second_cell, int code, bool overwrite) {
    switch (code) {
        case 101:
            if (first_cell.first == 0) {
                int start_point = first_cell.second;
                int end_point = second_cell.second;
                if (start_point > end_point) std::swap(start_point, end_point);

                int y = 1;
                start_point = gameTable->getXInConsole(start_point);
                end_point = gameTable->getXInConsole(end_point);

                if (overwrite == false) {
                    Screen::setCursorColor(GREEN, WHITE);

                    Screen::gotoXY(start_point, y);
                    putchar('x');
                    Screen::gotoXY(start_point, y + 1);
                    putchar(179);
                    Screen::gotoXY(start_point, y + 2);
                    putchar(179);
                    Screen::gotoXY(end_point, y);
                    putchar('x');
                    Screen::gotoXY(end_point, y + 1);
                    putchar(179);
                    Screen::gotoXY(end_point, y + 2);
                    putchar(179);
                }
                else {
                    Screen::setCursorColor(BLACK, WHITE);

                    Screen::gotoXY(start_point, y);
                    putchar(32);
                    Screen::gotoXY(start_point, y + 1);
                    putchar(196);
                    Screen::gotoXY(start_point, y + 2);
                    putchar(32);
                    Screen::gotoXY(end_point, y);
                    putchar(32);
                    Screen::gotoXY(end_point, y + 1);
                    putchar(196);
                    Screen::gotoXY(end_point, y + 2);
                    putchar(32);
                }

                for(int current_pos_x = start_point + 1; current_pos_x < end_point; ++current_pos_x) {
                    Screen::gotoXY(current_pos_x, y);

                    if (overwrite == false) {
                        putchar(196);
                    }
                    else {
                        putchar(32);
                    }
                }

                return;
            }

            if (first_cell.first == table_size_x - 1) {
                int start_point = first_cell.second;
                int end_point = second_cell.second;
                if (start_point > end_point) std::swap(start_point, end_point);

                int y = 27;
                start_point = gameTable->getXInConsole(start_point);
                end_point = gameTable->getXInConsole(end_point);

                if (overwrite == false) {
                    Screen::setCursorColor(GREEN, WHITE);

                    Screen::gotoXY(start_point, y);
                    putchar('x');
                    Screen::gotoXY(start_point, y - 1);
                    putchar(179);
                    Screen::gotoXY(start_point, y - 2);
                    putchar(179);
                    Screen::gotoXY(end_point, y);
                    putchar('x');
                    Screen::gotoXY(end_point, y - 1);
                    putchar(179);
                    Screen::gotoXY(end_point, y - 2);
                    putchar(179);
                }
                else {
                    Screen::setCursorColor(BLACK, WHITE);

                    Screen::gotoXY(start_point, y);
                    putchar(32);
                    Screen::gotoXY(start_point, y - 1);
                    putchar(196);
                    Screen::gotoXY(start_point, y - 2);
                    putchar(32);
                    Screen::gotoXY(end_point, y);
                    putchar(32);
                    Screen::gotoXY(end_point, y - 1);
                    putchar(196);
                    Screen::gotoXY(end_point, y - 2);
                    putchar(32);
                }

                for(int current_pos_x = start_point + 1; current_pos_x < end_point; ++current_pos_x) {
                    Screen::gotoXY(current_pos_x, y);

                    if (overwrite == false) {
                        putchar(196);
                    }
                    else {
                        putchar(32);
                    }
                }

                return;
            }

            break;
        case 102:
            if (first_cell.second == 0) {
                int start_point = first_cell.first;
                int end_point = second_cell.first;
                if (start_point > end_point) std::swap(start_point, end_point);

                int x = 2;
                start_point = gameTable->getYInConsole(start_point);
                end_point = gameTable->getYInConsole(end_point);

                if (overwrite == false) {
                    Screen::setCursorColor(GREEN, WHITE);

                    int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                    for(int tempX = 3; tempX <= 8; ++tempX) {
                        Screen::gotoXY(tempX, tempY);
                        putchar(196);
                    }

                    tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    for(int tempX = 3; tempX <= 8; ++tempX) {
                        Screen::gotoXY(tempX, tempY);
                        putchar(196);
                    }

                    Screen::gotoXY(x, start_point);
                    putchar('x');
                    Screen::gotoXY(x, end_point);
                    putchar('x');
                }
                else {
                    Screen::setCursorColor(BLACK, WHITE);

                    int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                    for(int tempX = 3; tempX <= 8; ++tempX) {
                        Screen::gotoXY(tempX, tempY);

                        if (tempX == 6) putchar(179);
                        else putchar(32);
                    }

                    tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    for(int tempX = 3; tempX <= 8; ++tempX) {
                        Screen::gotoXY(tempX, tempY);

                        if (tempX == 6) putchar(179);
                        putchar(32);
                    }

                    Screen::gotoXY(x, start_point);
                    putchar(32);
                    Screen::gotoXY(x, end_point);
                    putchar(32);
                }

                for(int current_pos_y = start_point + 1; current_pos_y < end_point; ++current_pos_y) {
                    Screen::gotoXY(x, current_pos_y);

                    if (overwrite == false) {
                        putchar(179);
                    }
                    else {
                        putchar(32);
                    }
                }

                return;
            }

            if (first_cell.second == table_size_y - 1) {
                int start_point = first_cell.first;
                int end_point = second_cell.first;
                if (start_point > end_point) std::swap(start_point, end_point);

                int x = 57;
                start_point = gameTable->getYInConsole(start_point);
                end_point = gameTable->getYInConsole(end_point);

                if (overwrite == false) {
                    Screen::setCursorColor(GREEN, WHITE);

                    int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                    for(int tempX = 51; tempX <= 56; ++tempX) {
                        Screen::gotoXY(tempX, tempY);
                        putchar(196);
                    }

                    tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    for(int tempX = 51; tempX <= 56; ++tempX) {
                        Screen::gotoXY(tempX, tempY);
                        putchar(196);
                    }

                    Screen::gotoXY(x, start_point);
                    putchar('x');
                    Screen::gotoXY(x, end_point);
                    putchar('x');
                }
                else {
                    Screen::setCursorColor(BLACK, WHITE);

                    int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                    for(int tempX = 51; tempX <= 56; ++tempX) {
                        Screen::gotoXY(tempX, tempY);

                        if (tempX == 54) putchar(179);
                        else putchar(32);
                    }

                    tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    for(int tempX = 51; tempX <= 56; ++tempX) {
                        Screen::gotoXY(tempX, tempY);

                        if (tempX == 54) putchar(179);
                        putchar(32);
                    }

                    Screen::gotoXY(x, start_point);
                    putchar(32);
                    Screen::gotoXY(x, end_point);
                    putchar(32);
                }

                for(int current_pos_y = start_point + 1; current_pos_y < end_point; ++current_pos_y) {
                    Screen::gotoXY(x, current_pos_y);

                    if (overwrite == false) {
                        putchar(179);
                    }
                    else {
                        putchar(32);
                    }
                }

                return;
            }

            break;
        case 201:
            if (first_cell.first > second_cell.first) std::swap(first_cell, second_cell);
            if (first_cell.first == 0) {
                int start_point = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_x;
                int end_point = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_x;

                int y = 1;

                if (overwrite == false) {
                    Screen::setCursorColor(GREEN, WHITE);

                    Screen::gotoXY(start_point, y);
                    putchar('x');
                    Screen::gotoXY(start_point, y + 1);
                    putchar(179);
                    Screen::gotoXY(start_point, y + 2);
                    putchar(179);
                    Screen::gotoXY(end_point, y);
                    putchar('x');

                    int tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    for(int current_pos_y = 2; current_pos_y < tempY; ++current_pos_y) {
                        Screen::gotoXY(end_point, current_pos_y);
                        putchar(179);
                    }
                }
                else {
                    Screen::setCursorColor(BLACK, WHITE);

                    Screen::gotoXY(start_point, y);
                    putchar(32);
                    Screen::gotoXY(start_point, y + 1);
                    putchar(196);
                    Screen::gotoXY(start_point, y + 2);
                    putchar(32);
                    Screen::gotoXY(end_point, y);
                    putchar(32);

                    int tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    for(int current_pos_y = 2; current_pos_y < tempY; ++current_pos_y) {
                        Screen::gotoXY(end_point, current_pos_y);
                        if (current_pos_y % 4 == 2) putchar(196);
                        else putchar(32);
                    }
                }

                start_point = first_cell.second;
                end_point = second_cell.second;
                if (start_point > end_point) std::swap(start_point, end_point);

                start_point = gameTable->getXInConsole(start_point);
                end_point = gameTable->getXInConsole(end_point);

                for(int current_pos_x = start_point + 1; current_pos_x < end_point; ++current_pos_x) {
                    Screen::gotoXY(current_pos_x, y);

                    if (overwrite == false) {
                        putchar(196);
                    }
                    else {
                        putchar(32);
                    }
                }

                return;
            }

            if (first_cell.first < second_cell.first) std::swap(first_cell, second_cell);
            if (first_cell.first == table_size_x - 1) {
                int start_point = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_x;
                int end_point = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_x;

                int y = 27;

                if (overwrite == false) {
                    Screen::setCursorColor(GREEN, WHITE);

                    Screen::gotoXY(start_point, y);
                    putchar('x');
                    Screen::gotoXY(start_point, y - 1);
                    putchar(179);
                    Screen::gotoXY(start_point, y - 2);
                    putchar(179);
                    Screen::gotoXY(end_point, y);
                    putchar('x');

                    int tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    for(int current_pos_y = tempY + 1; current_pos_y <= 26; ++current_pos_y) {
                        Screen::gotoXY(end_point, current_pos_y);
                        putchar(179);
                    }
                }
                else {
                    Screen::setCursorColor(BLACK, WHITE);

                    Screen::gotoXY(start_point, y);
                    putchar(32);
                    Screen::gotoXY(start_point, y - 1);
                    putchar(196);
                    Screen::gotoXY(start_point, y - 2);
                    putchar(32);
                    Screen::gotoXY(end_point, y);
                    putchar(32);

                    int tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    for(int current_pos_y = tempY + 1; current_pos_y <= 26; ++current_pos_y) {
                        Screen::gotoXY(end_point, current_pos_y);
                        if (current_pos_y % 4 == 2) putchar(196);
                        else putchar(32);
                    }
                }

                start_point = first_cell.second;
                end_point = second_cell.second;
                if (start_point > end_point) std::swap(start_point, end_point);

                start_point = gameTable->getXInConsole(start_point);
                end_point = gameTable->getXInConsole(end_point);

                for(int current_pos_x = start_point + 1; current_pos_x < end_point; ++current_pos_x) {
                    Screen::gotoXY(current_pos_x, y);

                    if (overwrite == false) {
                        putchar(196);
                    }
                    else {
                        putchar(32);
                    }
                }

                return;
            }

            break;
        case 202:
            if (first_cell.second > second_cell.second) std::swap(first_cell, second_cell);
            if (first_cell.second == 0) {
                int start_point = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                int end_point = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                if (start_point > end_point) std::swap(start_point, end_point);

                int x = 2;

                if (overwrite == false) {
                    Screen::setCursorColor(GREEN, WHITE);

                    int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                    for(int tempX = 3; tempX <= 8; ++tempX) {
                        Screen::gotoXY(tempX, tempY);
                        putchar(196);
                    }

                    Screen::gotoXY(x, start_point);
                    putchar('x');
                    Screen::gotoXY(x, end_point);
                    putchar('x');

                    tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    int endX = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_x;
                    for(int tempX = 3; tempX <= endX; ++tempX) {
                        Screen::gotoXY(tempX, tempY);
                        putchar(196);
                    }
                }
                else {
                    Screen::setCursorColor(BLACK, WHITE);

                    int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                    for(int tempX = 3; tempX <= 8; ++tempX) {
                        Screen::gotoXY(tempX, tempY);

                        if (tempX == 6) putchar(179);
                        else putchar(32);
                    }

                    Screen::gotoXY(x, start_point);
                    putchar(32);
                    Screen::gotoXY(x, end_point);
                    putchar(32);

                    tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    int endX = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_x;
                    for(int tempX = 3; tempX <= endX; ++tempX) {
                        Screen::gotoXY(tempX, tempY);

                        if (tempX % 8 == 6 || tempX == 6) putchar(179);
                        else putchar(32);
                    }
                }

                for(int current_pos_y = start_point + 1; current_pos_y < end_point; ++current_pos_y) {
                    Screen::gotoXY(x, current_pos_y);

                    if (overwrite == false) {
                        putchar(179);
                    }
                    else {
                        putchar(32);
                    }
                }

                return;
            }

            if (first_cell.second < second_cell.second) std::swap(first_cell, second_cell);
            if (first_cell.second == table_size_y - 1) {
                int start_point = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                int end_point = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                if (start_point > end_point) std::swap(start_point, end_point);

                int x = 57;

                if (overwrite == false) {
                    Screen::setCursorColor(GREEN, WHITE);

                    int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                    for(int tempX = 51; tempX < 57; ++tempX) {
                        Screen::gotoXY(tempX, tempY);
                        putchar(196);
                    }

                    Screen::gotoXY(x, start_point);
                    putchar('x');
                    Screen::gotoXY(x, end_point);
                    putchar('x');

                    tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    int begX = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_x;
                    for(int tempX = begX + 1; tempX < 57; ++tempX) {
                        Screen::gotoXY(tempX, tempY);
                        putchar(196);
                    }
                }
                else {
                    Screen::setCursorColor(BLACK, WHITE);

                    int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                    for(int tempX = 51; tempX < 57; ++tempX) {
                        Screen::gotoXY(tempX, tempY);

                        if (tempX == 54) putchar(179);
                        else putchar(32);
                    }

                    Screen::gotoXY(x, start_point);
                    putchar(32);
                    Screen::gotoXY(x, end_point);
                    putchar(32);

                    tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                    int begX = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_x;
                    for(int tempX = begX + 1; tempX < 57; ++tempX) {
                        Screen::gotoXY(tempX, tempY);

                        if (tempX % 8 == 6) putchar(179);
                        else putchar(32);
                    }
                }

                for(int current_pos_y = start_point + 1; current_pos_y < end_point; ++current_pos_y) {
                    Screen::gotoXY(x, current_pos_y);

                    if (overwrite == false) {
                        putchar(179);
                    }
                    else {
                        putchar(32);
                    }
                }

                return;
            }

            break;
        case 301: {
            int minX = std::min(first_cell.first, second_cell.first);

            for(int tempX = minX - 1; tempX >= 0; --tempX) {
                std::pair<int, int> cellA = {tempX, first_cell.second};
                std::pair<int, int> cellB = {tempX, second_cell.second};

                if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

                if (tempX == 0) {
                    bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
                    bool secondPath = checkMatchingTypeI(second_cell, cellB, true);

                    if (firstPath && secondPath) {
                        int y = 1;
                        int start_point = gameTable->table_data[tempX][first_cell.second].cell_coord_x;
                        int end_point = gameTable->table_data[tempX][second_cell.second].cell_coord_x;
                        if (start_point > end_point) std::swap(start_point, end_point);

                        if (overwrite == false) {
                            Screen::setCursorColor(GREEN, WHITE);

                            Screen::gotoXY(start_point, y);
                            putchar('x');
                            Screen::gotoXY(start_point, y + 1);
                            putchar(179);
                            Screen::gotoXY(start_point, y + 2);
                            putchar(179);
                            Screen::gotoXY(end_point, y);
                            putchar('x');
                            Screen::gotoXY(end_point, y + 1);
                            putchar(179);
                            Screen::gotoXY(end_point, y + 2);
                            putchar(179);
                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(179);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(179);

                            displayMatchingI(first_cell, cellA, false);
                            displayMatchingI(cellB, second_cell, false);
                        }
                        else {
                            Screen::setCursorColor(BLACK, WHITE);

                            Screen::gotoXY(start_point, y);
                            putchar(' ');
                            Screen::gotoXY(start_point, y + 1);
                            putchar(196);
                            Screen::gotoXY(start_point, y + 2);
                            putchar(32);
                            Screen::gotoXY(end_point, y);
                            putchar(' ');
                            Screen::gotoXY(end_point, y + 1);
                            putchar(196);
                            Screen::gotoXY(end_point, y + 2);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(32);

                            displayMatchingI(first_cell, cellA, true);
                            displayMatchingI(cellB, second_cell, true);
                        }

                        for(int current_pos_x = start_point + 1; current_pos_x < end_point; ++current_pos_x) {
                            Screen::gotoXY(current_pos_x, y);

                            if (overwrite == false) {
                                putchar(196);
                            }
                            else {
                                putchar(32);
                            }
                        }

                        return;
                    }
                }
                else {
                    bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
                    bool secondPath = checkMatchingTypeI(cellA, cellB, true);
                    bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

                    if (firstPath && secondPath && thirdPath) {
                        if (overwrite == false) {
                            Screen::setCursorColor(GREEN, WHITE);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar('x');
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar('x');

                            displayMatchingI(first_cell, cellA, false);
                            displayMatchingI(cellA, cellB, false);
                            displayMatchingI(cellB, second_cell, false);
                        }
                        else {
                            Screen::setCursorColor(BLACK, WHITE);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(32);

                            displayMatchingI(first_cell, cellA, true);
                            displayMatchingI(cellA, cellB, true);
                            displayMatchingI(cellB, second_cell, true);
                        }
                        return;
                    }
                }
            }
            break;
        }
        case 302: {
            int maxX = std::max(first_cell.first, second_cell.first);

            for(int tempX = maxX + 1; tempX < table_size_x; ++tempX) {
                std::pair<int, int> cellA = {tempX, first_cell.second};
                std::pair<int, int> cellB = {tempX, second_cell.second};

                if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

                if (tempX == table_size_x - 1) {
                    bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
                    bool secondPath = checkMatchingTypeI(second_cell, cellB, true);

                    if (firstPath && secondPath) {
                        int y = 27;
                        int start_point = gameTable->table_data[tempX][first_cell.second].cell_coord_x;
                        int end_point = gameTable->table_data[tempX][second_cell.second].cell_coord_x;
                        if (start_point > end_point) std::swap(start_point, end_point);

                        if (overwrite == false) {
                            Screen::setCursorColor(GREEN, WHITE);

                            Screen::gotoXY(start_point, y);
                            putchar('x');
                            Screen::gotoXY(start_point, y - 1);
                            putchar(179);
                            Screen::gotoXY(start_point, y - 2);
                            putchar(179);
                            Screen::gotoXY(end_point, y);
                            putchar('x');
                            Screen::gotoXY(end_point, y - 1);
                            putchar(179);
                            Screen::gotoXY(end_point, y - 2);
                            putchar(179);
                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(179);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(179);

                            displayMatchingI(first_cell, cellA, true);
                            displayMatchingI(cellB, second_cell, true);
                        }
                        else {
                            Screen::setCursorColor(BLACK, WHITE);

                            Screen::gotoXY(start_point, y);
                            putchar(' ');
                            Screen::gotoXY(start_point, y - 1);
                            putchar(196);
                            Screen::gotoXY(start_point, y - 2);
                            putchar(32);
                            Screen::gotoXY(end_point, y);
                            putchar(' ');
                            Screen::gotoXY(end_point, y - 1);
                            putchar(196);
                            Screen::gotoXY(end_point, y - 2);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(32);

                            displayMatchingI(first_cell, cellA, true);
                            displayMatchingI(cellB, second_cell, true);
                        }

                        for(int current_pos_x = start_point + 1; current_pos_x <= end_point; ++current_pos_x) {
                            Screen::gotoXY(current_pos_x, y);

                            if (overwrite == false) {
                                putchar(196);
                            }
                            else {
                                putchar(32);
                            }
                        }

                        return;
                    }
                }
                else {
                    bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
                    bool secondPath = checkMatchingTypeI(cellA, cellB, true);
                    bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

                    if (firstPath && secondPath && thirdPath) {
                        if (overwrite == false) {
                            Screen::setCursorColor(GREEN, WHITE);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar('x');
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar('x');

                            displayMatchingI(first_cell, cellA, false);
                            displayMatchingI(cellA, cellB, false);
                            displayMatchingI(cellB, second_cell, false);
                        }
                        else {
                            Screen::setCursorColor(BLACK, WHITE);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(32);

                            displayMatchingI(first_cell, cellA, true);
                            displayMatchingI(cellA, cellB, true);
                            displayMatchingI(cellB, second_cell, true);
                        }
                        return;
                    }
                }
            }

            break;
        }
        case 303: {
            int minY = std::min(first_cell.second, second_cell.second);

            for(int tempY = minY - 1; tempY >= 0; --tempY) {
                std::pair<int, int> cellA = {first_cell.first, tempY};
                std::pair<int, int> cellB = {second_cell.first, tempY};

                if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

                if (tempY == 0) {
                    bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
                    bool secondPath = checkMatchingTypeI(second_cell, cellB, true);

                    if (firstPath && secondPath) {
                        int x = 2;
                        int start_point = gameTable->table_data[first_cell.first][tempY].cell_coord_y;
                        int end_point = gameTable->table_data[second_cell.first][tempY].cell_coord_y;
                        if (start_point > end_point) std::swap(start_point, end_point);

                        if (overwrite == false) {
                            Screen::setCursorColor(GREEN, WHITE);

                            int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                            for(int tempX = 3; tempX <= 8; ++tempX) {
                                Screen::gotoXY(tempX, tempY);
                                putchar(196);
                            }

                            tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                            for(int tempX = 3; tempX <= 8; ++tempX) {
                                Screen::gotoXY(tempX, tempY);
                                putchar(196);
                            }

                            Screen::gotoXY(x, start_point);
                            putchar('x');
                            Screen::gotoXY(x, end_point);
                            putchar('x');

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(196);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(196);

                            displayMatchingI(first_cell, cellA, false);
                            displayMatchingI(cellB, second_cell, false);
                        }
                        else {
                            Screen::setCursorColor(BLACK, WHITE);

                            int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                            for(int tempX = 3; tempX <= 8; ++tempX) {
                                Screen::gotoXY(tempX, tempY);

                                if (tempX == 6) putchar(179);
                                else putchar(32);
                            }

                            tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                            for(int tempX = 3; tempX <= 8; ++tempX) {
                                Screen::gotoXY(tempX, tempY);

                                if (tempX == 6) putchar(179);
                                putchar(32);
                            }

                            Screen::gotoXY(x, start_point);
                            putchar(32);
                            Screen::gotoXY(x, end_point);
                            putchar(32);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(32);

                            displayMatchingI(first_cell, cellA, true);
                            displayMatchingI(cellB, second_cell, true);
                        }

                        for(int current_pos_y = start_point + 1; current_pos_y < end_point; ++current_pos_y) {
                            Screen::gotoXY(x, current_pos_y);

                            if (overwrite == false) {
                                putchar(179);
                            }
                            else {
                                putchar(32);
                            }
                        }
                        return;
                    }
                }
                else {
                    bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
                    bool secondPath = checkMatchingTypeI(cellA, cellB, true);
                    bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

                    if (firstPath && secondPath && thirdPath) {
                        if (overwrite == false) {
                            Screen::setCursorColor(GREEN, WHITE);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar('x');
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar('x');

                            displayMatchingI(first_cell, cellA, false);
                            displayMatchingI(cellA, cellB, false);
                            displayMatchingI(cellB, second_cell, false);
                        }
                        else {
                            Screen::setCursorColor(BLACK, WHITE);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(32);

                            displayMatchingI(first_cell, cellA, true);
                            displayMatchingI(cellA, cellB, true);
                            displayMatchingI(cellB, second_cell, true);
                        }
                        return;
                    }
                }
            }

            break;
        }
        case 304: {
            int maxY = std::max(first_cell.second, second_cell.second);

            for(int tempY = maxY + 1; tempY < table_size_y; ++tempY) {
                std::pair<int, int> cellA = {first_cell.first, tempY};
                std::pair<int, int> cellB = {second_cell.first, tempY};

                if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

                if (tempY == table_size_y - 1) {
                    bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
                    bool secondPath = checkMatchingTypeI(second_cell, cellB, true);

                    if (firstPath && secondPath) {
                        int x = 57;
                        int start_point = gameTable->table_data[first_cell.first][tempY].cell_coord_y;
                        int end_point = gameTable->table_data[second_cell.first][tempY].cell_coord_y;
                        if (start_point > end_point) std::swap(start_point, end_point);

                        if (overwrite == false) {
                            Screen::setCursorColor(GREEN, WHITE);

                            int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                            for(int tempX = 51; tempX <= 56; ++tempX) {
                                Screen::gotoXY(tempX, tempY);
                                putchar(196);
                            }

                            tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                            for(int tempX = 51; tempX <= 56; ++tempX) {
                                Screen::gotoXY(tempX, tempY);
                                putchar(196);
                            }

                            Screen::gotoXY(x, start_point);
                            putchar('x');
                            Screen::gotoXY(x, end_point);
                            putchar('x');

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(196);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(196);

                            displayMatchingI(first_cell, cellA, false);
                            displayMatchingI(cellB, second_cell, false);
                        }
                        else {
                            Screen::setCursorColor(BLACK, WHITE);

                            int tempY = gameTable->table_data[first_cell.first][first_cell.second].cell_coord_y;
                            for(int tempX = 51; tempX <= 56; ++tempX) {
                                Screen::gotoXY(tempX, tempY);

                                if (tempX == 54) putchar(179);
                                else putchar(32);
                            }

                            tempY = gameTable->table_data[second_cell.first][second_cell.second].cell_coord_y;
                            for(int tempX = 51; tempX <= 56; ++tempX) {
                                Screen::gotoXY(tempX, tempY);

                                if (tempX == 54) putchar(179);
                                putchar(32);
                            }

                            Screen::gotoXY(x, start_point);
                            putchar(32);
                            Screen::gotoXY(x, end_point);
                            putchar(32);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(32);

                            displayMatchingI(first_cell, cellA, true);
                            displayMatchingI(cellB, second_cell, true);
                        }

                        for(int current_pos_y = start_point + 1; current_pos_y < end_point; ++current_pos_y) {
                            Screen::gotoXY(x, current_pos_y);

                            if (overwrite == false) {
                                putchar(179);
                            }
                            else {
                                putchar(32);
                            }
                        }
                        return;
                    }
                }
                else {
                    bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
                    bool secondPath = checkMatchingTypeI(cellA, cellB, true);
                    bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

                    if (firstPath && secondPath && thirdPath) {
                        if (overwrite == false) {
                            Screen::setCursorColor(GREEN, WHITE);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar('x');
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar('x');

                            displayMatchingI(first_cell, cellA, false);
                            displayMatchingI(cellA, cellB, false);
                            displayMatchingI(cellB, second_cell, false);
                        }
                        else {
                            Screen::setCursorColor(BLACK, WHITE);

                            Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                                           gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                            putchar(32);
                            Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                                           gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                            putchar(32);

                            displayMatchingI(first_cell, cellA, true);
                            displayMatchingI(cellA, cellB, true);
                            displayMatchingI(cellB, second_cell, true);
                        }
                        return;
                    }
                }
            }

            break;
        }
        default:
            return;
    }
}

bool CasualMode::checkMatchingTypeZ(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    if (first_cell.second > second_cell.second) std::swap(first_cell, second_cell);

    for(int tempY = first_cell.second + 1; tempY < second_cell.second; ++tempY) {
        std::pair<int, int> cellA = {first_cell.first, tempY};
        std::pair<int, int> cellB = {second_cell.first, tempY};

        if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

        bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
        bool secondPath = checkMatchingTypeI(cellA, cellB, true);
        bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

        if (firstPath && secondPath && thirdPath) return true;
    }

    if (first_cell.first > second_cell.first) std::swap(first_cell, second_cell);

    for(int tempX = first_cell.first + 1; tempX < second_cell.first; ++tempX) {
        std::pair<int, int> cellA = {tempX, first_cell.second};
        std::pair<int, int> cellB = {tempX, second_cell.second};

        if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

        bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
        bool secondPath = checkMatchingTypeI(cellA, cellB, true);
        bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

        if (firstPath && secondPath && thirdPath) return true;
    }

    return false;
}

void CasualMode::displayMatchingZ(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool overwrite) {
    if (first_cell.second > second_cell.second) std::swap(first_cell, second_cell);

    for(int tempY = first_cell.second + 1; tempY < second_cell.second; ++tempY) {
        std::pair<int, int> cellA = {first_cell.first, tempY};
        std::pair<int, int> cellB = {second_cell.first, tempY};

        if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

        bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
        bool secondPath = checkMatchingTypeI(cellA, cellB, true);
        bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

        if (firstPath && secondPath && thirdPath) {
            if (overwrite == false) {
                Screen::setCursorColor(GREEN, WHITE);

                Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                               gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                putchar('x');
                Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                               gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                putchar('x');

                displayMatchingI(first_cell, cellA, false);
                displayMatchingI(cellA, cellB, false);
                displayMatchingI(cellB, second_cell, false);
                return;
            }
            else {
                Screen::setCursorColor(BLACK, WHITE);

                Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                               gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                putchar(32);
                Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                               gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                putchar(32);

                displayMatchingI(first_cell, cellA, true);
                displayMatchingI(cellA, cellB, true);
                displayMatchingI(cellB, second_cell, true);
                return;
            }
        }
    }

    if (first_cell.first > second_cell.first) std::swap(first_cell, second_cell);

    for(int tempX = first_cell.first + 1; tempX < second_cell.first; ++tempX) {
        std::pair<int, int> cellA = {tempX, first_cell.second};
        std::pair<int, int> cellB = {tempX, second_cell.second};

        if (getCellState(cellA.first, cellA.second) != DELETED || getCellState(cellB.first, cellB.second) != DELETED) continue;

        bool firstPath = checkMatchingTypeI(first_cell, cellA, true);
        bool secondPath = checkMatchingTypeI(cellA, cellB, true);
        bool thirdPath = checkMatchingTypeI(cellB, second_cell, true);

        if (firstPath && secondPath && thirdPath) {
            if (overwrite == false) {
                Screen::setCursorColor(GREEN, WHITE);

                Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                               gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                putchar('x');
                Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                               gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                putchar('x');

                displayMatchingI(first_cell, cellA, false);
                displayMatchingI(cellA, cellB, false);
                displayMatchingI(cellB, second_cell, false);
                return;
            }
            else {
                Screen::setCursorColor(BLACK, WHITE);

                Screen::gotoXY(gameTable->table_data[cellA.first][cellA.second].cell_coord_x,
                               gameTable->table_data[cellA.first][cellA.second].cell_coord_y);
                putchar(32);
                Screen::gotoXY(gameTable->table_data[cellB.first][cellB.second].cell_coord_x,
                               gameTable->table_data[cellB.first][cellB.second].cell_coord_y);
                putchar(32);

                displayMatchingI(first_cell, cellA, true);
                displayMatchingI(cellA, cellB, true);
                displayMatchingI(cellB, second_cell, true);
                return;
            }
        }
    }
}

bool CasualMode::checkEqualCharacter(std::pair<int, int> first_cell, std::pair<int, int> second_cell) {
    char first_cell_value = gameTable->table_data[first_cell.first][first_cell.second].cell_value;
    char second_cell_value = gameTable->table_data[second_cell.first][second_cell.second].cell_value;

    if (first_cell_value == second_cell_value) return true;
    return false;
}

int CasualMode::checkMatching(std::pair<int, int> first_cell, std::pair<int, int> second_cell, bool isDisplay) {
    if (checkEqualCharacter(first_cell, second_cell) == false) return 0;

    if (checkMatchingTypeI(first_cell, second_cell)) {
        if (isDisplay) {
            current_play.score += 3;
            displayMatchingI(first_cell, second_cell, false);
            Sleep(500);
            displayMatchingI(first_cell, second_cell, true);
        }
        return 1;
    }

    if (checkMatchingTypeL(first_cell, second_cell)) {
        if (isDisplay) {
            current_play.score += 5;
            displayMatchingL(first_cell, second_cell, false);
            Sleep(500);
            displayMatchingL(first_cell, second_cell, true);
        }
        return 2;
    }

    int codeMatchingU = checkMatchingTypeU(first_cell, second_cell);
    if (codeMatchingU) {
        if (isDisplay) {
            current_play.score += 8;
            displayMatchingU(first_cell, second_cell, codeMatchingU, false);
            Sleep(500);
            displayMatchingU(first_cell, second_cell, codeMatchingU, true);
        }
        return 3;
    }

    if (checkMatchingTypeZ(first_cell, second_cell)) {
        if (isDisplay) {
            current_play.score += 8;
            displayMatchingZ(first_cell, second_cell, false);
            Sleep(500);
            displayMatchingZ(first_cell, second_cell, true);
        }
        return 4;
    }

    return 0;
}

bool CasualMode::findValidPairs(bool isDisplay) {
    for(int xA = 0; xA < table_size_x; ++xA) {
        for(int yA = 0; yA < table_size_y; ++yA) {
            if (getCellState(xA, yA) == DELETED) continue;

            for(int xB = xA; xB < table_size_x; ++xB) {
                for(int yB = 0; yB < table_size_y; ++yB) {
                    if (xB == xA && yB <= yA) continue;
                    if (getCellState(xB, yB) == DELETED) continue;

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

Player CasualMode::gameplay() {
    Screen::clearConsole();
    setupTable();
    displayInfoFrame();
    selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);

    bool in_game = true;
    while (in_game && mistake > 0 && current_play.score >= 0 && remained_pairs > 0) {
        playerObj->score = std::max(playerObj->score, current_play.score);
        displayPlayerInfo(mistake, hint);

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
                if (hint) {
                    playHintSound();
                    if (findValidPairs(true) == true) {
                        --hint;
                    }
                    else {
                        displayNotification(90, 16, "NO MOVES LEFT", LIGHT_RED);
                        displayNotification(92, 17, "SHUFFLING", LIGHT_RED);
                        Sleep(500);
                        displayNotification(90, 16, "             ", WHITE);
                        displayNotification(92, 17, "         ", WHITE);
                        gameTable->shuffleTableData();
                        while (findValidPairs(false) == false) {
                            gameTable->shuffleTableData();
                        }
                        findValidPairs(true);
                    }
                }
                else {
                    displayNotification(90, 16, "NO HINTS LEFT", LIGHT_RED);
                    playMismatchSound();
                    Sleep(500);
                    displayNotification(90, 16, "             ", WHITE);
                }
                break;
            case 8:
                playShuffleSound();
                gameTable->shuffleTableData();
                while (findValidPairs(false) == false) {
                    gameTable->shuffleTableData();
                }
                displayTableData();
                selectCell(curr_cell_pos_x, curr_cell_pos_y, YELLOW);
                displayNotification(91, 16, "SHUFFLED", YELLOW);
                Sleep(500);
                displayNotification(91, 16, "        ", WHITE);
                break;
            case 9:
                SMenu.saveGame(current_play, gameTable);
                displayNotification(90, 16, "GAME SAVED", YELLOW);
                Sleep(500);
                displayNotification(90, 16, "          ", WHITE);
                break;
            default:
                break;
        }
    }

    playerObj->score = std::max(playerObj->score, current_play.score);

    Screen::setCursorColor(BLACK, WHITE);
    return current_play;
}
