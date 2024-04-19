#ifndef SCREEN_H
#define SCREEN_H

#include <windows.h>
#include <conio.h>

#include "sound.h"

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define AQUA 3
#define RED 4
#define PURPLE 5
#define YELLOW 6
#define LIGHT_WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_AQUA 11
#define LIGHT_RED 12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define WHITE 15

struct Screen {
    static HWND console_window;
    static HANDLE console_output;

    Screen() {}         //constructor
    ~Screen() {}        //destructor

    static void setUpConsole(int);          //Set up console
    static void gotoXY(int, int);           //Move to designated coordinate on console
    static void setFontInfo(int, int);      //Set attributes of font
    static void setWindow();                //Set size and locate console position
    static void setCursorColor(int, int);   //Set color of cursor
    static void setWindowsTitle();          //Set title of console
    static void disableMaximize();          //Disable maximizing window
    static void disableScrollBar();         //Disable scroll bar
    static void showCursor(bool);           //Toggle cursor appearance
    static void disableMouseInput();        //Disable mouse input
    static void clearConsole();             //Clear console screen
    static int getConsoleInput();           //Get input from console
};

#endif // SCREEN_H
