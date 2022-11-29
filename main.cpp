#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <iostream>
#include"conio2.h"
#define xCord 5
#define yCord 5
#define aKey 0x61
#define bKey 0x62
#define cKey 0x63

void initializeBoard( int &boardSize) {
    int sign;
    while (true) {
            sign = getch();
        switch (sign) {
            case aKey:
                boardSize = 9;
                break;
            case bKey:
                boardSize = 13;
                break;
            case cKey:
                boardSize = 19;
                break;
            default:
                continue;
        }
        break;
    }
}

void CreateBoard(int boardSize){
    int lastXCord = boardSize + xCord;
    int lastYCord = boardSize + yCord;
    for(int i = xCord;  i <= (lastXCord); i++){
        for(int j = yCord; j < lastYCord+2; j++) {
            gotoxy(i, j);
            if (j == yCord || j == (lastYCord) + 1) cputs("*");
            else if (i == xCord || i == lastXCord) cputs("|");
        }
        cputs("\n");
    }
}


int main() {
    int zn, attr = 7, back = 0, zero = 0;
    int  boardSize;
    char txt[32];

    initializeBoard(boardSize);
    int x = (boardSize+xCord) / 2;
    int y = (boardSize + yCord) / 2;

    int stonePlacement[boardSize][boardSize];
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; j < boardSize; j++) stonePlacement[i][j] = 0;

            // if the program is compiled in pure C, then we need to initialize
            // the library ourselves; __cplusplus is defined only if a C++ compiler
            // is used
#ifndef __cplusplus
            Conio2_Init();
#endif
            // settitle sets the window title
            settitle("Jakub Andrunik, 193166");
        _setcursortype(_NOCURSOR);
        // hide the blinking cursor
        do {
            // we set black to be the background color
            // check conio2.h for available colors
            textbackground(BLACK);
            // clear the screen: we fill it out with spaces with
            // the specified background color
            clrscr();
            // we set the text color (7 == LIGHTGRAY)
            textcolor(7);
            // we move the coursor to column 48 and row 1
            // rows and column are numbered starting with 1

            gotoxy(50, 1);
            // we print out a text at a given cursor position
            // the cursor will move by the length of the text
            cputs("q       = exit");
            gotoxy(50, 2);
            cputs("cursors = moving");
            gotoxy(50, 3);
            cputs("space   = change color");
            gotoxy(50, 4);
            cputs("enter   = change background color");
            // print out the code of the last key pressed
            gotoxy(50, 5);
            if (zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
            else sprintf(txt, "key code: 0x%02x", zn);
            cputs(txt);
            gotoxy(50, 6);
            sprintf(txt, "x coordinates: %02d", x);
            cputs(txt);
            gotoxy(50, 7);
            sprintf(txt, "y coordinates: %02d", y);
            cputs(txt);
            // we draw a star
            gotoxy(xCord, yCord);
            CreateBoard(boardSize);
            if (x <= xCord) {
                x++;
                continue;
            } else if (x > (xCord + boardSize) -1) {
                x--;
                continue;
            } else if (y <= yCord) {
                y++;
                continue;
            } else if (y > yCord + boardSize) {
                y--;
                continue;
            }
            for(int i = 0; i < boardSize; i++){
                for(int j = 0; j < boardSize; j++){
                    if(stonePlacement[i][j] == 1){
                        gotoxy(i+xCord, j+yCord);
                        textcolor(3);
                        putch('*');
                    } else if(stonePlacement[i][j] == 2){
                        gotoxy(i+xCord, j+yCord);
                        textcolor(4);
                        putch('*');
                    }
                }
            }
            gotoxy(x, y);
            textcolor(attr);
            textbackground(back);
            putch('*');
            // putch prints one character and moves the cursor to the right
            zero = 0;

            zn = getch();

            // we do not want the key 'H' to play role of "up arrow"
            // so we check if the first code is zero
            if (zn == 0) {
                zero = 1;        // if this is the case then we read
                zn = getch();
                // the next code knowing that thi;
                // enter key is 0x0d or '\r'
                switch (zn) {
                    case 0x48:y--; break;
                    case 0x50: y++; break;
                    case 0x4b: x--; break;
                    case 0x4d: x++; break;
                    default: break;
                }
            }
            if(zn == 0x69){
                int tabX = x - xCord;
                int tabY = y - yCord;
                static int counter = 0;
                if(counter % 2 == 0) stonePlacement[tabX][tabY] = 1;
                else stonePlacement[tabX][tabY] = 2;
                counter++;
            }

        } while (zn != 'q');
        _setcursortype(_NORMALCURSOR);    // show the cursor so it will be
        // visible after the program ends

        return 0;

    }
