#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "tableLL.h"
#include "screen.h"
#include "game_saving.h"

struct HardMode {
    int table_size_m = 8;                               //number of rows
    int table_size_n = 8;                               //number of columns
    int padding_left = 6;                               //distance to the left border of console
    int padding_top = 2;                                //distance to the top border of console
    int curr_cell_pos_x = 0;                            //current row index
    int curr_cell_pos_y = 0;                            //current column index
    int remained_pairs;                                 //remaining pairs on the game
    std::vector<std::pair<int, int>> locked_cells;      //control locked cells
    std::string *table_image;                           //store table image

    TableLL *gameTable;                                 //game table

    Player *playerObj;                                  //user
    Player current_play;                                //current game user
    int mistake;
    int hint;

    SavingMenu SMenu;

    HardMode(Player, Player *);                         //constructor
    ~HardMode();                                        //destructor

    void loadAssets(const std::string &, std::string *&);   //load backgrounds
    void saveUserData(Player *, int);                       //save user data

    void selectCell(int, int, int);          //select cell
    void unselectCell();                    //unselect current cell
    void lockCell();                        //lock current cell
    void deleteCell();                      //delete current cell

    void moveUp();                          //move up
    void moveDown();                        //move down
    void moveRight();                       //move right
    void moveLeft();                        //move left

    void displayInfoFrame();                //display information frame
    void displayPlayerInfo(int, int);       //display player information
    void displayNotification(int, int, std::string, int);   //display game notification

    void displayTableBorder();                  //display table border
    void displayCellData(int, int, int, int);   //display cell data
    void displayTableData();                    //display table data
    void setupTable();                          //initialize table

    std::pair<int, int> getCellCoord(int, int); //get cell coordination on console

    bool checkMatchingTypeI(std::pair<int, int>, std::pair<int, int>);  //check matching type I
    bool checkMatchingTypeL(std::pair<int, int>, std::pair<int, int>);  //check matching type L
    bool checkMatchingTypeU(std::pair<int, int>, std::pair<int, int>);  //check matching type U
    bool checkMatchingTypeZ(std::pair<int, int>, std::pair<int, int>);  //check matching type Z

    bool checkEqualCharacter(std::pair<int, int>, std::pair<int, int>);   //check equal characters
    int checkMatching(std::pair<int, int>, std::pair<int, int>, bool);    //check matching of two cells

    bool findValidPairs(bool);  //find if valid pair exists or not

    Player gameplay(); //play the game
};

