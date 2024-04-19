#include "cell.h"

Cell::Cell() {
    cell_value = ' ';
    cell_state = 0;
    cell_pos_x = 0;
    cell_pos_y = 0;
    cell_coord_x = 0;
    cell_coord_y = 0;
    next = nullptr;
}

Cell::Cell(Cell *_cell) {
    cell_value = _cell->cell_value;
    cell_state = _cell->cell_state;
    next = _cell->next;
}

Cell::Cell(char _cell_value, int _cell_state, int _cell_pos_x, int _cell_pos_y, int _cell_coord_x, int _cell_coord_y) {
    cell_value = _cell_value;
    cell_state = _cell_state;
    cell_pos_x = _cell_pos_x;
    cell_pos_y = _cell_pos_y;
    cell_coord_x = _cell_coord_x;
    cell_coord_y = _cell_coord_y;
    next = nullptr;
}

Cell::Cell(char _cell_value, int _cell_state) {
    cell_value = _cell_value;
    cell_state = _cell_state;
    next = nullptr;
}
