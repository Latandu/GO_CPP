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
struct GameInfo {
    int player1Pts = 0;
    int player2Pts = 0;
    int counter = 0;

} gameInfo;


int main() {
    int zn = 0, attr = 7, back = 0, zero = 0;
    char txt[32];
    float komi = 0.5;
    int sign = 0;
    int boardSize, availableColumns = 0, availableRows = 0;
    gameInfo.player1Pts = 0;
    gameInfo.player2Pts = 0;
    gameInfo.counter = 0;
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
    for (int i = 0; i < arrBoardSize; i++) {
        stonePlacement[i] = new int[arrBoardSize];
        tempStonePlacement[i] = new int[arrBoardSize];
        koArray1[i] = new int[arrBoardSize];
        koArray2[i] = new int[arrBoardSize];
    }
    NewBoard(arrBoardSize, stonePlacement, freeSpace);
    NewBoard(arrBoardSize, koArray1, freeSpace);
    NewBoard(arrBoardSize, koArray2, freeSpace);
#ifndef __cplusplus
    Conio2_Init();
    Functions_Init();
#endif
    InitializeHandicap(attr, back, boardSize, arrBoardSize, stonePlacement, &zn, &zero, &x, &y, &availableRows,
                       &availableColumns, &komi);
    do {
        DisplayLegend(txt, x, y, gameInfo.counter, gameInfo.player1Pts, gameInfo.player2Pts);
        CreateBoard(boardSize, &availableRows, &availableColumns);
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
            int confirm;
            gotoxy(legendXPos, legendYPos + 16);
            cputs("Press ENTER to confirm.");
            gotoxy(legendXPos, legendYPos + 17);
            cputs("Press ESC to cancel.");
            do {
                confirm = getch();
            } while (confirm != enterKey && confirm != escKey);
            if (confirm == enterKey) {
                //changing yCord into tabX to inverse 2D array to match positions with board
                int tabX = y - yCord;
                int tabY = x - xCord;
                int player, opponent;
                if (gameInfo.counter % 2 == 0) player = player1, opponent = player2;
                else player = player2, opponent = player1;
                //check whether there is already any other stone
                if (stonePlacement[tabX][tabY] == player1 || stonePlacement[tabX][tabY] == player2) continue;
                //add stone according to player number
                if (player == player1) {
                    stonePlacement[tabX][tabY] = player1;
                } else {
                    stonePlacement[tabX][tabY] = player2;
                }
                //check for all possible combinations of board

                SurroundingLogic(arrBoardSize, stonePlacement, tempStonePlacement, player, opponent, freeSpace,
                                 &gameInfo.player1Pts, &gameInfo.player2Pts);

                if (player == player1) {
                    if (CheckKO(stonePlacement, koArray1, arrBoardSize)) {
                        WriteToArray(arrBoardSize, stonePlacement, koArray2);
                        gameInfo.player1Pts--;
                        continue;
                    }
                } else {
                    if (CheckKO(stonePlacement, koArray2, arrBoardSize)) {
                        WriteToArray(arrBoardSize, stonePlacement, koArray1);
                        gameInfo.player2Pts--;
                        continue;
                    }
                }
                NewBoard(arrBoardSize, tempStonePlacement, freeSpace);
                if (SurroundingCheck(opponent, player, stonePlacement, tempStonePlacement, tabX,
                                     tabY, freeSpace)) {
                    stonePlacement[tabX][tabY] = freeSpace;
                    continue;
                }
                if (player == player1) WriteToArray(arrBoardSize, koArray1, stonePlacement);
                else WriteToArray(arrBoardSize, koArray2, stonePlacement);

                gameInfo.counter++;
            } else continue;
        }

        if (zn == nKey) {
            clrscr();
            gameInfo.counter = 0;
            gameInfo.player1Pts = 0;
            gameInfo.player2Pts = 0;
            initializeBoard(&boardSize, sign, &availableRows, &availableColumns);
            NewBoard(arrBoardSize, stonePlacement, freeSpace);
            NewBoard(arrBoardSize, tempStonePlacement, freeSpace);
            InitializeHandicap(attr, back, boardSize, arrBoardSize, stonePlacement, &zn, &zero, &x, &y, &availableRows,
                               &availableColumns, &komi);
        }
        if (zn == sKey) {
            gotoxy(legendXPos, legendYPos + 14);
            cputs("FILE SAVER");
            char fileName[64] = {0};
            int info;
            ReadFileName(fileName, &info);
            if (info == 1) continue;
            FILE *f = fopen(fileName, "w");
            for (int i = 1; i < arrBoardSize - 1; i++) {
                for (int j = 1; j < arrBoardSize - 1; j++) {
                    fwrite(&stonePlacement[j][i], sizeof(int), 1, f);
                    fwrite(&koArray1[j][i], sizeof(int), 1, f);
                    fwrite(&koArray2[j][i], sizeof(int), 1, f);
                }
            }
            fwrite(&gameInfo, sizeof(struct GameInfo), sizeof(struct GameInfo), f);
            fclose(f);
        }
        if (zn == lKey) {
            gotoxy(legendXPos, legendYPos + 14);
            cputs("FILE LOADER");
            char fileName[64] = {0};
            int info;
            ReadFileName(fileName, &info);
            if (info == 1) continue;
            FILE *f = fopen(fileName, "r");
            rewind(f);
            for (int i = 1; i < arrBoardSize - 1; i++) {
                for (int j = 1; j < arrBoardSize - 1; j++) {
                    fread(&stonePlacement[j][i], sizeof(int), 1, f);
                    fread(&koArray1[j][i], sizeof(int), 1, f);
                    fread(&koArray2[j][i], sizeof(int), 1, f);
                }
            }
            fread(&gameInfo, sizeof(struct GameInfo), sizeof(struct GameInfo), f);
            fclose(f);
        }
        if (zn == fKey) {
            do {
                textbackground(BLACK);
                clrscr();
                textcolor(7);
                gotoxy(legendXPos, legendYPos);
                cputs("Finishing the game. Mark dead stones.");
                CreateBoard(boardSize, &availableRows, &availableColumns);
                gotoxy(x, y);
                if (CheckForBorders(boardSize, &x, &y)) continue;
                InsertStone(arrBoardSize, stonePlacement);
                gotoxy(x, y);
                textcolor(attr);
                textbackground(back);
                putch('*');
                zero = 0;
                zn = getch();
                if (zn == moveArrow) ArrowMovement(&zn, &zero, &x, &y);
                if (zn == iKey) {
                    int confirm;
                    gotoxy(legendXPos, legendYPos + 16);
                    cputs("Press ENTER to confirm.");
                    gotoxy(legendXPos, legendYPos + 17);
                    cputs("Press ESC to cancel.");
                    do {
                        confirm = getch();
                    } while (confirm != enterKey && confirm != escKey);
                    if (confirm == enterKey) {
                        int tabX = y - yCord;
                        int tabY = x - xCord;
                        if (stonePlacement[tabX][tabY] == player1 || stonePlacement[tabX][tabY] == player2) {
                            stonePlacement[tabX][tabY] = 0;
                        } else continue;
                    } else continue;
                }
            } while (zn != enterKey);
            for (int m = 1; m < 3; m++) {
                int player = m;
                int opponent;
                if (player == player1) opponent = player2;
                else opponent = player1;
                SurroundingLogic(arrBoardSize, stonePlacement, tempStonePlacement, player, freeSpace, opponent,
                                 &gameInfo.player2Pts, &gameInfo.player1Pts);
            }
            gotoxy(legendXPos, legendYPos + 1);
            cputs("Game finished, press q to exit");
            gotoxy(legendXPos, legendYPos + 3);
            sprintf(txt, "Player 1 points: %02d", gameInfo.player1Pts);
            cputs(txt);
            gotoxy(legendXPos, legendYPos + 4);
            sprintf(txt, "Player 2 points: %02d", gameInfo.player2Pts );
            cputs(txt);
            do {
                zn = getch();
            } while (zn != qKey);
        }
    } while (zn != qKey);
    delete[] stonePlacement;
    delete[] tempStonePlacement;
    delete[] koArray1;
    delete[] koArray2;
    _setcursortype(_NORMALCURSOR);
    return 0;

}








