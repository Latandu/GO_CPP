//
// Created by Latandu on 01/12/2022.
//
#define _CRT_SECURE_NO_WARNINGS
#include "conio2.h"
#include "functions.h"
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

void initializeBoard(int *boardSize, int sign) {
    while (true) {
        gotoxy(boardXYCord, boardXYCord);
        // we print out a text at a given cursor position
        // the cursor will move by the length of the text
        cputs("a = board 9x9");
        gotoxy(boardXYCord, boardXYCord+1);
        cputs("b = board 13x13");
        gotoxy(boardXYCord, boardXYCord+2);
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

void NewBoard(int arrBoardSize, int **stonePlacement) {
    for (int i = 0; i < arrBoardSize; i++){
        for (int j = 0; j < arrBoardSize; j++) {
            if(i == 0 || i == arrBoardSize - 1 || j == 0 || j == arrBoardSize - 1) stonePlacement[i][j] = 3;
            else stonePlacement[i][j] = 0;
        }
    }
}

bool SurroundingStones(int player, int opponent, int **stonePlacement, int i, int j) {
    if (stonePlacement[i][j] == opponent
        && (stonePlacement[i - 1][j] == player || stonePlacement[i - 1][j] == 3)
        && (stonePlacement[i + 1][j] == player || stonePlacement[i + 1][j] == 3)
        && (stonePlacement[i][j - 1] == player || stonePlacement[i][j - 1] == 3)
        && (stonePlacement[i][j + 1] == player || stonePlacement[i][j + 1] == 3)){
            return true;
    }
    else return false;
}
//TODO Implement capturing: first check for a border, then check whether the "inside" of the figure is full with
// opponents stones.
/*int** AdvancedSurroundingCheck(int player, int opponent, int **stonePlacement, int **tempStonePlacement, int i, int j){

}*/
/*int** AdvancedSurroundingCheck(int player, int opponent, int **stonePlacement, int **tempStonePlacement, int i, int j){
    if(stonePlacement[i][j] == 0 || stonePlacement[i + 1][j] == 0
    || stonePlacement[i][j - 1] == 0 || stonePlacement[i][j + 1] == 0){
        return nullptr;
    }
    while(stonePlacement[i][j] != player)
        tempStonePlacement[i][j] = stonePlacement[i][j];
        AdvancedSurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, i - 1, j);
    }*/


bool KoEnforce(int **stonePlacement, int player, int opponent, int i, int j, int &koPenalty) {
    static int placeX = 0, placeY = 0;
    if(koPenalty == 0) {
        placeX = NULL;
        placeY = NULL;
        if (SurroundingStones(opponent, player, stonePlacement, i, j) &&
            SurroundingStones(player, opponent, stonePlacement, i + 1, j)) {
            koPenalty = 2;
            placeX = i+1;
            placeY = j;
            stonePlacement[i + 1][j] = 0;
            return true;
        } else if (SurroundingStones(opponent, player, stonePlacement, i, j) &&
                   SurroundingStones(player, opponent, stonePlacement, i - 1, j)) {
            koPenalty = 2;
            placeX = i-1;
            placeY = j;
            stonePlacement[i - 1][j] = 0;
            return true;
        } else if (SurroundingStones(opponent, player, stonePlacement, i, j) &&
                   SurroundingStones(player, opponent, stonePlacement, i, j - 1)) {
            koPenalty = 2;
            placeX = i;
            placeY = j-1;
            stonePlacement[i][j - 1] = 0;
            return true;
        } else if (SurroundingStones(opponent, player, stonePlacement, i, j) &&
                   SurroundingStones(player, opponent, stonePlacement, i, j + 1)) {
            koPenalty = 2;
            placeX = i;
            placeY = j+2;
            stonePlacement[i][j + 1] = 0;
            return true;
        }
    } if (SurroundingStones(player, opponent, stonePlacement, i, j) && i != placeX && j != placeY) {
        stonePlacement[i][j] = 0;
        return true;
    }
    return false;
    }
bool CheckForSuicide(int **stonePlacement, int player, int opponent, int i, int j, int &koPenalty){
    if(!KoEnforce(stonePlacement, player, opponent, i, j, koPenalty) &&
    SurroundingStones(opponent, player, stonePlacement, i, j)){
        return false;
    }
    else return true;
}

bool CheckForBorders(int boardSize, int *x, int *y) {
    if (*x <= xCord) {
        ++*x;
        return true;
    } else if (*x > (xCord + boardSize)) {
        --*x;
        return true;
    } else if (*y <= yCord) {
        ++*y;
        return true;
    } else if (*y > yCord + boardSize) {
        --*y;
        return true;
    }
    return false;
}

void InsertStone(int arrBoardSize, int **stonePlacement) {
    for(int i = 1; i < arrBoardSize - 1; i++){
        for(int j = 1; j < arrBoardSize - 1; j++){
            if(stonePlacement[i][j] == 1){
                gotoxy(j+yCord, i+xCord);
                textcolor(3);
                putch('*');
            } else if(stonePlacement[i][j] == 2){
                gotoxy(j+yCord, i+xCord);
                textcolor(4);
                putch('*');
            }
        }
    }
}
void ArrowMovement(int *zn, int *zero, int *x, int *y) {
        *zero = 1;        // if this is the case then we read
        *zn = getch();
        // the next code knowing that thi;
        // enter key is 0x0d or '\r'
        switch (*zn) {
            case 0x48: --*y; break;
            case 0x50: ++*y; break;
            case 0x4b: --*x; break;
            case 0x4d: ++*x; break;
            default: break;
        }

}

void DisplayLegend(int zn, int zero, char *txt, int x, int y, int counter, int player1Pts, int player2Pts, int ko) {
    textbackground(BLACK);
    clrscr();
    textcolor(7);
    gotoxy(legendXPos, legendYPos);
    cputs("q       = exit");
    gotoxy(legendXPos, legendYPos+1);
    cputs("cursors = moving");
    gotoxy(legendXPos, legendYPos+2);
    cputs("space   = change color");
    gotoxy(legendXPos, legendYPos+3);
    cputs("enter   = change background color");
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
    gotoxy(legendXPos, legendYPos+10);
    if(ko > 0) cputs("KO PENALTY ACTIVE");
}