#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "table.h"
#include "screen.h"
#include "game_saving.h"

struct CasualMode {
    int table_size_x = 6;           //number of rows
    int table_size_y = 6;           //number of columns
    int padding_left = 6;           //distance from the left border of console
    int padding_top = 2;            //same as above but from the top
    int curr_cell_pos_x = 0;        //current selected row
    int curr_cell_pos_y = 0;        //current selected column
    int remained_pairs;             //remaining pairs of the game
    std::vector<std::pair<int, int>> locked_cells;  //maintain locked cells
    std::string *table_image;       //store table image

    Table *gameTable;           //game table
    bool table_loaded = false;  //check if table loaded from save file

    Player *playerObj;      //user
    Player current_play;    //current user play
    int mistake;
    int hint;

    SavingMenu SMenu;

    CasualMode(Player, Player *, Table *);
    ~CasualMode();

    void loadAssets(const std::string &, std::string *&);

    //interaction with cells on board
    void selectCell(int, int, int);
    void unselectCell();
    void lockCell();
    void deleteCell();

    //navigate through board
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();

    //display game information and player information
    void displayInfoFrame();
    void displayPlayerInfo(int, int);
    void displayNotification(int, int, std::string, int);

    //display aspects of game table
    void displayTableBorder();
    void displayCellData(int, int, int, int, int, int);
    void displayTableData();
    void setupTable();

    int getCellState(int, int);     //get cell state

    //check matching and display it

    bool checkMatchingTypeI(std::pair<int, int>, std::pair<int, int>, bool);
    void displayMatchingI(std::pair<int, int>, std::pair<int, int>, bool);

    bool checkMatchingTypeL(std::pair<int, int>, std::pair<int, int>);
    void displayMatchingL(std::pair<int, int>, std::pair<int, int>, bool);

    int checkMatchingTypeU(std::pair<int, int>, std::pair<int, int>);
    void displayMatchingU(std::pair<int, int>, std::pair<int, int>, int, bool);

    bool checkMatchingTypeZ(std::pair<int, int>, std::pair<int, int>);
    void displayMatchingZ(std::pair<int, int>, std::pair<int, int>, bool);

    bool checkEqualCharacter(std::pair<int, int>, std::pair<int, int>);
    int checkMatching(std::pair<int, int>, std::pair<int, int>, bool);

    bool findValidPairs(bool);

    Player gameplay();
};
