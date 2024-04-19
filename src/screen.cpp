#include "screen.h"

HWND Screen::console_window = GetConsoleWindow();
HANDLE Screen::console_output = GetStdHandle(STD_OUTPUT_HANDLE);

void Screen::setUpConsole(int font_size) {
    setFontInfo(font_size, font_size);
    setWindow();
    setWindowsTitle();
    disableMaximize();
    disableScrollBar();
    showCursor(false);
    disableMouseInput();
}

void Screen::gotoXY(int posX, int posY) {
    COORD coord = {(short) posX, (short) posY};
    SetConsoleCursorPosition(console_output, coord);
}

void Screen::setFontInfo(int size_x, int size_y) {
	CONSOLE_FONT_INFOEX info;
	info.cbSize = sizeof(info);
	GetCurrentConsoleFontEx(console_output, FALSE, &info);
	info.dwFontSize.X = size_x;
	info.dwFontSize.Y = size_y;
	wcscpy_s(info.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(console_output, FALSE, &info);
}

void Screen::setWindow() {
    const int width = 1200;
    const int height = 800;
    const int posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    const int posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

    MoveWindow(console_window, posX, posY, width, height, TRUE);
}

void Screen::setCursorColor(int background, int text) {
    SetConsoleTextAttribute(console_output, background * 16 + text);
}

void Screen::setWindowsTitle() {
    SetConsoleTitle("The Matching Game");
}

void Screen::disableMaximize() {
    SetWindowLong(console_window, GWL_STYLE, GetWindowLong(console_window, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
}

void Screen::disableScrollBar() {
    ShowScrollBar(console_window, SB_BOTH, FALSE);
}

void Screen::showCursor(bool show) {
    CONSOLE_CURSOR_INFO info = {1, show};
    SetConsoleCursorInfo(console_output, &info);
}

void Screen::disableMouseInput() {
    DWORD pref_mode;
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    GetConsoleMode(hInput, &pref_mode);
    SetConsoleMode(hInput, pref_mode & ~ENABLE_QUICK_EDIT_MODE);
}

void Screen::clearConsole() {
    system("cls");
}

int Screen::getConsoleInput() {
    int key_input = _getch();

    if (key_input == 0 || key_input == 224) {
        key_input = _getch();
        switch (key_input) {
            case 72:            //Up
                return 2;
            case 80:            //Down
                return 3;
            case 75:            //Left
                return 4;
            case 77:            //Right
                return 5;
            default:
                return 0;
        }
    }
    else {
        switch (key_input) {
            case 1: case 27:    //Esc
                return 1;
            case 13: case 28:   //Enter
                return 6;
            case 72: case 104:  //H
                return 7;
            case 70: case 102:  //F
                return 8;
            case 83: case 115:  //S
                return 9;
            default:
                return 0;
        }
    }
}
