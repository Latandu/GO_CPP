#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "conio2.h"
#include "functions.h"
#define xCord 5
#define yCord 5
#define aKey 0x61
#define bKey 0x62
#define cKey 0x63
#define iKey 0x69
#define nKey 0x6e
#define legendXPos 60
#define legendYPos 10
#define boardXYCord 3
#define moveArrow 0

bool KoEnforce(int **stonePlacement, int player, int opponent, int i, int j) {
    if(SurroundingStones(opponent, player, stonePlacement, i, j) &&
    SurroundingStones(player, opponent, stonePlacement, i+1, j)) {
        stonePlacement[i + 1][j] = 0;
        return true;
    }
    else if(SurroundingStones(opponent, player, stonePlacement, i, j) &&
    SurroundingStones(player, opponent, stonePlacement, i-1, j)){
        stonePlacement[i-1][j] = 0;
        return true;
    }
    else if(SurroundingStones(opponent, player, stonePlacement, i, j) &&
    SurroundingStones(player, opponent, stonePlacement, i, j-1)){
            stonePlacement[i][j-1] = 0;
            return true;
    }
    else if(SurroundingStones(opponent, player, stonePlacement, i, j) &&
    SurroundingStones(player, opponent, stonePlacement, i, j+1)){
        stonePlacement[i][j+1] = 0;
        return true;
    }
    else if(SurroundingStones(player, opponent, stonePlacement, i, j)){
        stonePlacement[i][j] = 0;
        return true;
    }
    return false;
}
bool CheckForSuicide(int **stonePlacement, int player, int opponent, int i, int j){
    if(!KoEnforce(stonePlacement, player, opponent, i, j)){
        if(SurroundingStones(opponent, player, stonePlacement, i, j)){
            return false;
        }
        else return true;
    }
    return true;
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
    int** stonePlacement = new int*[arrBoardSize];
    for(int i = 0; i < arrBoardSize; i++) stonePlacement[i] = new int[arrBoardSize];
    NewBoard(arrBoardSize, stonePlacement);


#ifndef __cplusplus
    Conio2_Init();
    Functions_Init();
#endif
    do {
        DisplayLegend(zn, zero, txt, x, y, counter, player1Pts, player2Pts);
        CreateBoard(boardSize);
        if(CheckForBorders(boardSize, &x, &y)) continue;
        InsertStone(arrBoardSize, stonePlacement);
        gotoxy(x, y);
        textcolor(attr);
        textbackground(back);
        putch('*');
        zero = 0;
        zn = getch();
        if(zn == moveArrow) ArrowMovement(&zn, &zero, &x, &y);
        if(zn == iKey){
            int tabX = x - xCord;
            int tabY = y - yCord;
            int player, opponent;

            if(counter % 2 == 0) player = 1, opponent = 2;
            else player = 2, opponent = 1;
            if(stonePlacement[tabX][tabY] == 1 || stonePlacement[tabX][tabY] == 2){
                gotoxy(legendXPos, legendYPos+8);
                cputs("Move Impossible");
                continue;
            }
            if(player == 1) stonePlacement[tabX][tabY] = 1;
            else stonePlacement[tabX][tabY] = 2;

            if(!CheckForSuicide(stonePlacement, player, opponent, tabX, tabY)){
                stonePlacement[tabX][tabY] = 0;
                continue;
            }


            for (int i = 1; i < arrBoardSize - 1; i++){
                for (int j = 1; j < arrBoardSize - 1; j++) {
                    if(KoEnforce(stonePlacement, player, opponent, i, j)) player2Pts++;
                   }
                }

            counter++;
        }

        if(zn == nKey){
            counter = 0;
            player1Pts = 0;
            player2Pts = 0;
            NewBoard(arrBoardSize, stonePlacement);
        }
        if(zn == 0x73){
            FILE *f = fopen("D:\\PG\\BOP\\GO_CPP\\savedgame.txt", "w");
            fprintf(f, "%d ", stonePlacement[j][i]);
            for (int i = 1; i < arrBoardSize - 1; i++){
                for (int j = 1; j < arrBoardSize - 1; j++) {


                }

            }
            fclose(f);
        }

    } while (zn != 'q');

        _setcursortype(_NORMALCURSOR);
        return 0;

    }




