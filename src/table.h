#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <time.h>

#include "screen.h"
#include "cell.h"

struct Table {
    int table_size_x;               //rows of table
    int table_size_y;               //columns of table
    int remained_cells;             //remaining cells of table
    int *character_list;            //list of generated characters
    int *character_order;           //list of table order of characters
    int *occurs;                    //check occurrence of characters
    Cell **table_data;              //table data

    Table();                        //table constructor
    Table(int, int);                //table constructor with arguments
    ~Table();                       //table destructor

    int getXInConsole(int);         //get the x distance in console
    int getYInConsole(int);         //get the y distance in console

    void generateTableData();       //generate table data
    void shuffleTableData();        //shuffle table data
};

#endif // TABLE_H
