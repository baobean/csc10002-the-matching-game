#include <iostream>
#include <time.h>

#include "screen.h"
#include "linked_list.h"


struct TableLL {
    int table_size_m;   //table row size
    int table_size_n;   //table column size
    int remained_cells; //remaining cells on the board
    int *character_list;    //list of characters to be generated
    int *character_order;   //list of positions to put the characters on the board
    int *occurs;            //check occurrence of characters
    LinkedList *table_data; //table data

    TableLL();              //table constructor
    TableLL(int, int);      //table constructor with arguments
    ~TableLL();             //table destructor

    Cell *getCell(int, int);//get cell

    int getCellState(int, int);         //get cell state
    char getCellValue(int, int);        //get cell value

    void setCellState(int, int, int);   //set cell value

    void removeCell(int, int);          //remove cell from table

    void generateTableData();           //generate table data
    void shuffleTableData();            //shuffle table data
};
