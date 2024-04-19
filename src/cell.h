#ifndef CELL_H
#define CELL_H

#define FREE 0
#define LOCKED 1
#define DELETED 2

#define STANDARD_MODE 100
#define HARD_MODE 200

#define CELL_HEIGHT 3
#define CELL_WIDTH 7

struct Cell {
    char cell_value;
    int cell_state;
    int cell_pos_x;
    int cell_pos_y;
    int cell_coord_x;
    int cell_coord_y;
    Cell *next;

    Cell();
    Cell(Cell*);
    Cell(char, int, int, int, int, int);
    Cell(char, int);
};

#endif // CELL_H
