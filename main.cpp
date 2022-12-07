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
#define legendXPos 3
#define legendYPos 3
#define boardXYCord 3
#define moveArrow 0
#define sKey 0x73
#define enterKey 0x0d
#define escKey 0x1b
#define lKey 0x6c



int main() {
    int zn = 0, attr = 7, back = 0, zero = 0;
    int boardSize;
    char txt[32];
    int counter = 0;
    int sign = 0;
    int player1Pts = 0;
    int player2Pts = 0;
    int ko = 0;
    settitle("Jakub Andrunik, 193166");
    _setcursortype(_NOCURSOR);
    initializeBoard(&boardSize, sign);
    int x = xCord + (boardSize / 2);
    int y = yCord + (boardSize / 2);
    int arrBoardSize = boardSize +2;
    int** stonePlacement = new int*[arrBoardSize];
    int** koArray1 = new int*[arrBoardSize];
    int** koArray2 = new int*[arrBoardSize];
    int** tempStonePlacement = new int*[arrBoardSize];
    for(int i = 0; i < arrBoardSize; i++) {
        stonePlacement[i] = new int[arrBoardSize];
        tempStonePlacement[i] = new int[arrBoardSize];
        koArray1[i] = new int[arrBoardSize];
        koArray2[i] = new int[arrBoardSize];
    }
    NewBoard(arrBoardSize, stonePlacement, 3);
    NewBoard(arrBoardSize, koArray1, 3);
    NewBoard(arrBoardSize, koArray2, 3);
#ifndef __cplusplus
    Conio2_Init();
    Functions_Init();
#endif
    InitializeHandicap(attr, back, boardSize, arrBoardSize, stonePlacement, &zn, &zero, &x, &y);

    do {
        DisplayLegend(zn, zero, txt, x, y, counter, player1Pts, player2Pts);
        CreateBoard(boardSize);
        gotoxy(x, y);
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
            //changing yCord into tabX to inverse 2D array to match positions with board
            int tabX = y - yCord;
            int tabY = x - xCord;
            int player, opponent;
            if(counter % 2 == 0) player = 1, opponent = 2;
            else player = 2, opponent = 1;
            //check whether there is already any other stone
            if(stonePlacement[tabX][tabY] == 1 || stonePlacement[tabX][tabY] == 2) continue;
            //add stone according to player number
            if(player == 1){
                stonePlacement[tabX][tabY] = 1;
            }
            else{
                stonePlacement[tabX][tabY] = 2;
            }
            //check for all possible combinations of board

            for (int i = 1; i < arrBoardSize - 1; i++){
                for (int j = 1; j < arrBoardSize - 1; j++) {
                    NewBoard(arrBoardSize, tempStonePlacement, 3);
                    //inverted positions for compatibility with 2D array
                    if (stonePlacement[j][i] == opponent && SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, j, i) == 1){
                        for (int k = 1; k < arrBoardSize - 1; k++){
                            for (int l = 1; l < arrBoardSize - 1; l++) {
                                if(stonePlacement[l][k] == tempStonePlacement[l][k] && tempStonePlacement[l][k] == opponent){
                                    stonePlacement[l][k] = 0;
                                    if(player == 1) player1Pts++;
                                    else player2Pts++;
                                }
                            }
                        }
                    }

                }
            }
            if (player == 1){
                if(CheckKO(stonePlacement, koArray1, arrBoardSize)){
                    for(int i = 0; i < arrBoardSize; i++){
                        for(int j =0; j < arrBoardSize; j++) stonePlacement[j][i] = koArray2[j][i];
                    }
                    player1Pts--;
                    continue;
                }
            }else {
                if(CheckKO(stonePlacement, koArray2, arrBoardSize)){
                    for(int i = 0; i < arrBoardSize; i++){
                        for(int j =0; j < arrBoardSize; j++) stonePlacement[j][i] = koArray1[j][i];
                    }
                    player2Pts--;
                    continue;
                }
            }

            if(SurroundingCheck(opponent, player, stonePlacement, tempStonePlacement, tabX, tabY)){
                stonePlacement[tabX][tabY] = 0;
                continue;
            }

            if(player == 1){
                for(int i = 0; i < arrBoardSize; i++){
                    for(int j =0; j < arrBoardSize; j++) koArray1[j][i] = stonePlacement[j][i];
                }
            }
            else {
                for(int i = 0; i < arrBoardSize; i++){
                    for(int j =0; j < arrBoardSize; j++) koArray2[j][i] = stonePlacement[j][i];
                }
            }
            counter++;
        }
        if(zn == nKey){
            clrscr();
            counter = 0;
            player1Pts = 0;
            player2Pts = 0;
            initializeBoard(&boardSize, sign);
            NewBoard(arrBoardSize, stonePlacement, 3);
            NewBoard(arrBoardSize, tempStonePlacement, 3);
            InitializeHandicap(attr, back, boardSize, arrBoardSize, stonePlacement, &zn, &zero, &x, &y);
        }
        if(zn == sKey){
            char fileName[64] = {0};
            char fileCPuts[64] = {0};
            int k = 0, info = 0;
            char c;
            do {
                gotoxy(legendXPos, legendYPos+10);
                sprintf(fileCPuts, "Enter file name to save: %s", fileName);
                cputs(fileCPuts);
                c = (char)getch();
                if(c == escKey){
                    info = 1;
                    break;
                }
                if(c != enterKey) {
                    fileName[k] = c;
                    k++;
                }
            }while(c != enterKey && k < 63);
            if(info == 1) continue;
            FILE *f = fopen(fileName, "w");
            for (int i = 1; i < arrBoardSize - 1; i++){
                for (int j = 1; j < arrBoardSize - 1; j++) {
                    //fprintf(f, "%d ", stonePlacement[i][j]);
                    fprintf(f, "%d", stonePlacement[i][j]);
                }
            }
            fclose(f);
        }
        if(zn == lKey){
            char fileName[64] = {0};
            char fileCPuts[64] = {0};
            int k = 0, info = 0, num;
            int fileBoardSize = arrBoardSize^2;
            char str[fileBoardSize];
            char c;
            int len;
            do {
                gotoxy(legendXPos, legendYPos+10);
                sprintf(fileCPuts, "Enter file name to save: %s", fileName);
                cputs(fileCPuts);
                c = (char)getch();
                if(c == escKey){
                    info = 1;
                    break;
                }
                if(c != enterKey) {
                    fileName[k] = c;
                    k++;
                }
            }while(c != enterKey && k < 63);
            if(info == 1) continue;
            FILE *f = fopen(fileName, "r");
            rewind(f);
            for (int i = 1; i < arrBoardSize - 1; i++){
                for (int j = 1; j < arrBoardSize - 1; j++) {
                    while(fscanf(f, "%01d", &num) != ' ') {
                        stonePlacement[i][j] = num;
                    }
                }
            }
            fclose(f);
        }


    } while (zn != 'q');
        delete[] stonePlacement;
        delete[] tempStonePlacement;
        delete[] koArray1;
        delete[] koArray2;
        _setcursortype(_NORMALCURSOR);
        return 0;

    }





