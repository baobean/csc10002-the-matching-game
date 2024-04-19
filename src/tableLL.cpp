#include "tableLL.h"

TableLL::TableLL() {}

TableLL::TableLL(int _table_size_m, int _table_size_n) {
    table_size_m = _table_size_m;
    table_size_n = _table_size_n;

    remained_cells = table_size_m * table_size_n;

    table_data = new LinkedList[table_size_m];
}

TableLL::~TableLL() {
    for(int i = 0; i < table_size_m; ++i) {
        table_data[i].removeAll();
    }

    delete []table_data;
    table_data = nullptr;
}

Cell *TableLL::getCell(int row, int col) {
    return table_data[row].getNode(col);
}

int TableLL::getCellState(int row, int col) {
    return getCell(row, col)->cell_state;
}

char TableLL::getCellValue(int row, int col) {
    return getCell(row, col)->cell_value;
}

void TableLL::setCellState(int row, int col, int state) {
    Cell *curr_cell = getCell(row, col);
    curr_cell->cell_state = state;
}

void TableLL::removeCell(int row, int col) {
    Cell *target_cell = getCell(row, col);
    table_data[row].shiftCell(target_cell);
}

void TableLL::generateTableData() {
    srand(time(NULL));

    character_list = new int[table_size_m * table_size_n];
    occurs = new int[26];

    for(int i = 0; i < 26; ++i) occurs[i] = 0;

    int character_number = table_size_m * table_size_n;
    for(int i = 0; i < character_number; i += 2) {
        int character = rand() % 26;
        while (occurs[character] > 1) character = rand() % 26;

        character_list[i] = character_list[i + 1] = character;
        ++occurs[character];
    }

    delete []occurs;
    occurs = nullptr;

    bool *check_appear = new bool[table_size_m * table_size_n];
    character_order = new int[table_size_m * table_size_n];

    for(int i = 0; i < character_number; ++i) check_appear[i] = false;

    for(int i = 0; i < character_number; ++i) {
        int position = rand() % character_number;
        while (check_appear[position]) position = rand() % character_number;

        character_order[position] = character_list[i];
        check_appear[position] = true;
    }

    for(int id = 0; id < table_size_m * table_size_n; ++id) {
        int i = id / table_size_n;

        Cell *curr_node = new Cell((char)(character_order[id] + 'A'), FREE);

        table_data[i].addTail(curr_node);
    }

    delete []character_list;
    character_list = nullptr;
    delete []check_appear;
    check_appear = nullptr;
    delete []character_order;
    character_order = nullptr;
}

void TableLL::shuffleTableData() {
    srand(time(NULL));

    character_list = new int[remained_cells];
    character_order = new int[remained_cells];
    bool *check_appear = new bool[remained_cells];

    int id = 0;
    for(int x = 0; x < table_size_m; ++x) {
        for(int y = 0; y < table_size_n; ++y) {
            Cell *curr_cell = getCell(x, y);
            int curr_cell_state = curr_cell->cell_state;
            if (curr_cell_state == DELETED) continue;

            character_list[id] = curr_cell->cell_value;
            check_appear[id] = false;
            ++id;
        }
    }

    for(int i = 0; i < remained_cells; ++i) {
        int position = rand() % remained_cells;
        while (check_appear[position]) position = rand() % remained_cells;
        character_order[position] = character_list[i];
        check_appear[position] = true;
    }

    id = 0;
    for(int x = 0; x < table_size_m; ++x) {
        for(int y = 0; y < table_size_n; ++y) {
            Cell *curr_cell = getCell(x, y);
            int curr_cell_state = curr_cell->cell_state;
            if (curr_cell_state == DELETED) continue;

            curr_cell->cell_value = character_order[id];
            ++id;
        }
    }

    delete []character_list;
    character_list = nullptr;
    delete []check_appear;
    check_appear = nullptr;
    delete []character_order;
    character_order = nullptr;
}
