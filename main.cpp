#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"
#define xCord 5
#define yCord 5
#define aKey 0x61
#define bKey 0x62
#define cKey 0x63
#define legendXPos 60
#define legendYPos 10

enum codes {
    black_stone = 3,

};
char* AllocateBoard(int arrBoardSize){}


void DisplayLegend(int zn, int zero, char *txt, int x, int y, int counter, int player1Pts, int player2Pts) {
    // check conio2.h for available colors
    textbackground(BLACK);
    // clear the screen: we fill it out with spaces with
    // the specified background color
    clrscr();
    // we set the text color (7 == LIGHTGRAY)
    textcolor(7);
    // we move the coursor to column 48 and row 1
    // rows and column are numbered starting with 1

    gotoxy(legendXPos, legendYPos);
    // we print out a text at a given cursor position
    // the cursor will move by the length of the text
    cputs("q       = exit");
    gotoxy(legendXPos, legendYPos+1);
    cputs("cursors = moving");
    gotoxy(legendXPos, legendYPos+2);
    cputs("space   = change color");
    gotoxy(legendXPos, legendYPos+3);
    cputs("enter   = change background color");
    // print out the code of the last key pressed
    gotoxy(legendXPos, legendYPos+4);
    if (zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
    else sprintf(txt, "key code: 0x%02x", zn);
    cputs(txt);
    gotoxy(legendXPos, legendYPos+5);
    sprintf(txt, "x coordinates: %02d", x-xCord);
    cputs(txt);
    gotoxy(legendXPos, legendYPos+6);
    sprintf(txt, "y coordinates: %02d", y-yCord);
    cputs(txt);
    gotoxy(legendXPos, legendYPos+7);
    sprintf(txt, "counter: %02d", counter);
    cputs(txt);
    gotoxy(legendXPos, legendYPos+8);
    sprintf(txt, "Player 1 points: %02d", player1Pts);
    cputs(txt);
    gotoxy(legendXPos, legendYPos+9);
    sprintf(txt, "Player 2 points: %02d", player2Pts);
    cputs(txt);

}

void initializeBoard(int *boardSize, int sign) {
    while (true) {
        gotoxy(3, 2);
        // we print out a text at a given cursor position
        // the cursor will move by the length of the text
        cputs("a = board 9x9");
        gotoxy(3, 3);
        cputs("b = board 13x13");
        gotoxy(3, 4);
        cputs("c = board 19x19");
        sign = getch();
        switch (sign) {
            case aKey:
                *boardSize = 9;
                break;
            case bKey:
                *boardSize = 13;
                break;
            case cKey:
                *boardSize = 19;
                break;
            default:
                gotoxy(3, 5);
                cputs("Incorrect key, try again");
                continue;
        }
        break;
    }
}

void CreateBoard(int boardSize){
    gotoxy(xCord, yCord);
    int lastXCord = boardSize + xCord;
    int lastYCord = boardSize + yCord;
    for(int i = xCord;  i <= (lastXCord)+1; i++){
        for(int j = yCord; j <= lastYCord+1; j++) {
            gotoxy(i, j);
            if (j == yCord || j == lastYCord+1) cputs("*");
            else if (i == xCord || i == lastXCord+1) cputs("|");
        }
        cputs("\n");
    }
}

int main() {
    int zn = 0, attr = 7, back = 0, zero = 0;
    int boardSize;
    char txt[32];
    int counter = 0;
    int sign = 0;
    int player1Pts = 0;
    int player2Pts = 0;
    settitle("Jakub Andrunik, 193166");
    _setcursortype(_NOCURSOR);
    initializeBoard(&boardSize, sign);
    int x = (boardSize+xCord) / 2;
    int y = (boardSize + yCord) / 2;
    int arrBoardSize = boardSize +2;

    int stonePlacement[arrBoardSize][arrBoardSize];
    for (int i = 0; i < arrBoardSize; i++){
        for (int j = 0; j < arrBoardSize; j++) {
            if(i == 0 || i == arrBoardSize - 1 || j == 0 || j == arrBoardSize - 1) stonePlacement[i][j] = 3;
            else stonePlacement[i][j] = 0;
        }
    }


#ifndef __cplusplus
            Conio2_Init();
#endif
    do {
        DisplayLegend(zn, zero, txt, x, y, counter, player1Pts, player2Pts);
        CreateBoard(boardSize);
        if (x <= xCord) {
            x++;
            continue;
        } else if (x > (xCord + boardSize)) {
            x--;
            continue;
        } else if (y <= yCord) {
            y++;
            continue;
        } else if (y > yCord + boardSize) {
            y--;
            continue;
        }
        for(int i = 1; i < arrBoardSize - 1; i++){
            for(int j = 1; j < arrBoardSize - 1; j++){
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
                case 0x48: y--; break;
                case 0x50: y++; break;
                case 0x4b: x--; break;
                case 0x4d: x++; break;
                default: break;
            }
        }
        if(zn == 0x69){
            int tabX = x - xCord;
            int tabY = y - yCord;

            if(stonePlacement[tabX][tabY] == 1 || stonePlacement[tabX][tabY] == 2){
                gotoxy(legendXPos, legendYPos+8);
                cputs("Move Impossible");
                continue;
            }
            if((stonePlacement[tabX-1][tabY] == 1 || stonePlacement[tabX-1][tabY] == 2
                || stonePlacement[tabX-1][tabY] == 3)
                && (stonePlacement[tabX+1][tabY] == 1 || stonePlacement[tabX+1][tabY] == 2
                || stonePlacement[tabX+1][tabY] == 3)
                && (stonePlacement[tabX][tabY-1] == 1 || stonePlacement[tabX][tabY-1] == 2
                || stonePlacement[tabX][tabY-1] == 3)
                && (stonePlacement[tabX][tabY+1] == 1 || stonePlacement[tabX][tabY+1] == 2
                || stonePlacement[tabX][tabY+1] == 3)){

                stonePlacement[tabX][tabY] = 0;
                gotoxy(legendXPos, legendYPos+10);
                cputs("Move Impossible");
                continue;
            }
            if(counter % 2 == 0) stonePlacement[tabX][tabY] = 1;
            else stonePlacement[tabX][tabY] = 2;

            for (int i = 1; i < arrBoardSize - 1; i++){
                for (int j = 1; j < arrBoardSize - 1; j++) {
                    if (stonePlacement[i][j] == 2
                        && (stonePlacement[i - 1][j] == 1 || stonePlacement[i - 1][j] == 3)
                        && (stonePlacement[i + 1][j] == 1 || stonePlacement[i + 1][j] == 3)
                        && (stonePlacement[i][j - 1] == 1 || stonePlacement[i][j - 1] == 3)
                        && (stonePlacement[i][j + 1] == 1 || stonePlacement[i][j + 1] == 3)) {
                        stonePlacement[i][j] = 0;
                        player1Pts++;
                    }
                    if ((stonePlacement[i][j] == 1 &&
                         stonePlacement[i - 1][j] == 2 || stonePlacement[i - 1][j] == 3)
                        && (stonePlacement[i + 1][j] == 2 || stonePlacement[i + 1][j] == 3)
                        && (stonePlacement[i][j - 1] == 2 || stonePlacement[i][j - 1] == 3)
                        && (stonePlacement[i][j + 1] == 2 || stonePlacement[i][j + 1] == 3)) {
                        stonePlacement[i][j] = 0;
                        player2Pts++;
                    }
                }
            }
            counter++;
        }


        if(zn == 0x6e){
            counter = 0;
            for (int i = 0; i < arrBoardSize; i++){
                for (int j = 0; j < arrBoardSize; j++) {
                    if(i == 0 || i == arrBoardSize - 1 || j == 0 || j == arrBoardSize - 1) stonePlacement[i][j] = 3;
                    else stonePlacement[i][j] = 0;
                }
            }
        }


    } while (zn != 'q');
        _setcursortype(_NORMALCURSOR);    // show the cursor so it will be
        // visible after the program ends

        return 0;

    }




