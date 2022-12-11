#define _CRT_SECURE_NO_WARNINGS

#include "conio2.h"
#include "functions.h"

#define xCord 50
#define yCord 3
#define aKey 0x61
#define bKey 0x62
#define cKey 0x63
#define iKey 0x69
#define nKey 0x6e
#define sKey 0x73
#define enterKey 0x0d
#define escKey 0x1b
#define lKey 0x6c
#define qKey 0x71
#define legendXPos 3
#define legendYPos 3
#define boardXYCord 3
#define moveArrow 0
#define player1 1
#define player2 2
#define freeSpace 0
#define fKey 0x66
struct GameInfo gameInfo;
int main() {
    int zn = 0, attr = 7, back = 0, zero = 0;
    char txt[32];
    float komi = 0.5;
    int sign = 0;
    int boardSize, availableColumns = 0, availableRows = 0;
    settitle("Jakub Andrunik, 193166");
    _setcursortype(_NOCURSOR);
    initializeBoard(&boardSize, sign, &availableRows, &availableColumns);
    // set the starting place in the middle
    int x = xCord + (boardSize / 2);
    int y = yCord + (boardSize / 2);
    int arrBoardSize = boardSize + 2;
    int **stonePlacement = new int *[arrBoardSize];
    int **koArray1 = new int *[arrBoardSize];
    int **koArray2 = new int *[arrBoardSize];
    int **tempStonePlacement = new int *[arrBoardSize];
    InitializeArrays(arrBoardSize, stonePlacement, koArray1, koArray2, tempStonePlacement);
#ifndef __cplusplus
    Conio2_Init();
    Functions_Init();
#endif
    InitializeHandicap(attr, back, boardSize, arrBoardSize, stonePlacement, &zn, &zero, &x, &y, &komi);
    do {
        DisplayLegend(txt, x, y, gameInfo.counter, gameInfo.player1Pts, gameInfo.player2Pts);
        CreateBoard(boardSize);
        gotoxy(x, y);
        if (CheckForBorders(boardSize, &x, &y)) continue;
        InsertStone(arrBoardSize, stonePlacement);
        gotoxy(x, y);
        textcolor(attr);
        textbackground(back);
        putch('*');
        zero = moveArrow;
        zn = getch();
        if (zn == moveArrow) ArrowMovement(&zn, &zero, &x, &y);
        if (zn == iKey) {
            if(IKeyFunction(x, y, arrBoardSize, stonePlacement, koArray1, koArray2, tempStonePlacement) == 1) continue;
        }

        if (zn == nKey) {
            NKeyFunction(attr, back, &zero, sign, &availableColumns, &availableRows, arrBoardSize, stonePlacement,
                         tempStonePlacement, &zn, &komi, &boardSize, &x, &y);
        }
        if (zn == sKey) {
            if (FileSaverFunction(arrBoardSize, stonePlacement, koArray1, koArray2) == 1) continue;
        }
        if (zn == lKey) {
            if (FileLoaderFunction(arrBoardSize, stonePlacement, koArray1, koArray2) == 1) continue;
        }
        if (zn == fKey) {
            FKeyFunction(attr, back, &zero, txt, komi, boardSize, arrBoardSize, stonePlacement, tempStonePlacement, &zn,
                       &x, &y);
        }
    } while (zn != qKey);
    delete[] stonePlacement;
    delete[] tempStonePlacement;
    delete[] koArray1;
    delete[] koArray2;
    _setcursortype(_NORMALCURSOR);
    return 0;

}








