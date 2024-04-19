#include "table.h"

Table::Table() {}

Table::Table(int _table_size_x, int _table_size_y) {
    table_size_x = _table_size_x;
    table_size_y = _table_size_y;

    remained_cells = table_size_x * table_size_y;

    table_data = new Cell*[table_size_x];
    for(int i = 0; i < table_size_x; ++i) {
        table_data[i] = new Cell[table_size_y];
    }
}

Table::~Table() {
    for(int i = 0; i < table_size_x; ++i) {
        delete []table_data[i];
    }

    delete []table_data;
    table_data = nullptr;
}

int Table::getXInConsole(int col) {
    return 6 + (CELL_WIDTH + 1) * col + 4;
}

int Table::getYInConsole(int row) {
    return 2 + (CELL_HEIGHT + 1) * row + 2;
}

void Table::generateTableData() {
    srand(time(NULL));

    character_list = new int[table_size_x * table_size_y];
    occurs = new int[26];

    for(int i = 0; i < 26; ++i) occurs[i] = 0;

    int character_number = table_size_x * table_size_y;
    for(int i = 0; i < character_number; i += 2) {
        int character = rand() % 26;
        while (occurs[character] > 1) character = rand() % 26;

        character_list[i] = character_list[i + 1] = character;
        ++occurs[character];
    }

    delete []occurs;
    occurs = nullptr;

    bool *check_appear = new bool[table_size_x * table_size_y];
    character_order = new int[table_size_x * table_size_y];

    for(int i = 0; i < character_number; ++i) check_appear[i] = false;

    for(int i = 0; i < character_number; ++i) {
        int position = rand() % character_number;
        while (check_appear[position]) position = rand() % character_number;

        character_order[position] = character_list[i];
        check_appear[position] = true;
    }

    int id = 0;
    int origin_coor_x = 10;
    int origin_coor_y = 4;
    for(int i = 0; i < table_size_x; ++i) {
        for(int j = 0; j < table_size_y; ++j) {
            table_data[i][j].cell_pos_x = i;
            table_data[i][j].cell_pos_y = j;
            table_data[i][j].cell_coord_x = origin_coor_x + j * (CELL_WIDTH + 1);
            table_data[i][j].cell_coord_y = origin_coor_y + i * (CELL_HEIGHT + 1);

            table_data[i][j].cell_value = (char)(character_order[id] + 'A');
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

void Table::shuffleTableData() {
    srand(time(NULL));

    character_list = new int[remained_cells];
    character_order = new int[remained_cells];
    bool *check_appear = new bool[remained_cells];

    int id = 0;
    for(int x = 0; x < table_size_x; ++x) {
        for(int y = 0; y < table_size_y; ++y) {
            int curr_cell_state = table_data[x][y].cell_state;
            if (curr_cell_state == DELETED) continue;

            character_list[id] = table_data[x][y].cell_value;
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
    for(int x = 0; x < table_size_x; ++x) {
        for(int y = 0; y < table_size_y; ++y) {
            int curr_cell_state = table_data[x][y].cell_state;
            if (curr_cell_state == DELETED) continue;

            table_data[x][y].cell_value = character_order[id];
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
