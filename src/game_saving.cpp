#include "game_saving.h"

int SavingMenu::menuController() {
    displayOptions();

    bool in_menu = true;
    while (in_menu) {
        switch (Screen::getConsoleInput()) {
            case 2:
                changeOption(-1);
                playMoveSound();
                break;
            case 3:
                changeOption(1);
                playMoveSound();
                break;
            case 6:
                in_menu = 0;
                playHintSound();
                switch (currentOption) {
                    case 0:
                        return 0;
                        break;
                    case 1:
                        return 1;
                        break;
                }
                break;
            default:
                break;
        }
    }

    return true;
}

void SavingMenu::changeOption(int change) {
    int updatedOption = currentOption + change;
    if (updatedOption < 0 || updatedOption >= option_slot) return;

    unselectOption();

    currentOption = updatedOption;

    selectOption();
}

void SavingMenu::selectOption() {
    Screen::setCursorColor(BLACK, GREEN);

    int padding_left = 60, padding_top = 28;

    Screen::gotoXY(padding_left - 2, padding_top + currentOption * 5);
    putchar('>');

    Screen::gotoXY(padding_left + options[currentOption].length() + 1, padding_top + currentOption * 5);
    putchar('<');

    Screen::gotoXY(padding_left, padding_top + currentOption * 5);
    std::cout << options[currentOption];
}

void SavingMenu::unselectOption() {
    Screen::setCursorColor(BLACK, WHITE);

    int padding_left = 60, padding_top = 28;

    Screen::gotoXY(padding_left - 2, padding_top + currentOption * 5);
    putchar(' ');

    Screen::gotoXY(padding_left + options[currentOption].length() + 1, padding_top + currentOption * 5);
    putchar(' ');

    Screen::gotoXY(padding_left, padding_top + currentOption * 5);
    std::cout << options[currentOption];
}

void SavingMenu::displayOptions() {
    Screen::setCursorColor(BLACK, WHITE);

    int padding_left = 60, padding_top = 28; //(132 - 11) / 2 - 1 = 59
    Screen::gotoXY(padding_left, padding_top);

    for(int i = 0; i < option_slot; ++i) {
        Screen::gotoXY(padding_left, padding_top + i * 5);
        std::cout << options[i] << "\n";
    }

    selectOption();
}

void SavingMenu::loadGame(Player *&user, Table *&game_state) {
    std::string path = "saves/" + user->ID + ".bin";
    std::ifstream in(path, std::ios::binary);

    in.read(reinterpret_cast<char *> (&user->score), 4);

    int table_size_x = 0;
    int table_size_y = 0;
    in.read(reinterpret_cast<char *> (&table_size_x), 4);
    in.read(reinterpret_cast<char *> (&table_size_y), 4);

    game_state = new Table(table_size_x, table_size_y);

    for (int i = 0; i < table_size_x; ++i) {
        for (int j = 0; j < table_size_y; ++j) {
            in.read(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_state), 4);
        }
    }

    for (int i = 0; i < table_size_x; ++i) {
        for (int j = 0; j < table_size_y; ++j) {
            in.read(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_value), 1);
        }
    }

    for (int i = 0; i < table_size_x; ++i) {
        for (int j = 0; j < table_size_y; ++j) {
            game_state->table_data[i][j].cell_pos_x = i;
            game_state->table_data[i][j].cell_pos_y = j;
            in.read(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_coord_x), 4);
            in.read(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_coord_y), 4);
        }
    }

    in.close();
}

void SavingMenu::saveGame(Player user, Table *game_state) {
    std::string path = "saves/" + user.ID + ".bin";
    std::ofstream out(path, std::ios::binary);

    out.write(reinterpret_cast<char *> (&user.score), sizeof(user.score));

    int table_size_x = game_state->table_size_x;
    int table_size_y = game_state->table_size_y;

    out.write(reinterpret_cast<char *> (&table_size_x), sizeof(table_size_x));
    out.write(reinterpret_cast<char *> (&table_size_y), sizeof(table_size_y));

    for (int i = 0; i < table_size_x; ++i) {
        for (int j = 0; j < table_size_y; ++j) {
            out.write(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_state), sizeof(game_state->table_data[i][j].cell_state));
        }
    }

    for (int i = 0; i < table_size_x; ++i) {
        for (int j = 0; j < table_size_y; ++j) {
            out.write(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_value), sizeof(game_state->table_data[i][j].cell_value));
        }
    }

    for (int i = 0; i < table_size_x; ++i) {
        for (int j = 0; j < table_size_y; ++j) {
            out.write(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_coord_x), sizeof(game_state->table_data[i][j].cell_coord_x));
            out.write(reinterpret_cast<char *> (&game_state->table_data[i][j].cell_coord_y), sizeof(game_state->table_data[i][j].cell_coord_y));
        }
    }

    out.close();
}
