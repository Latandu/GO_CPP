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

void NewBoard(int arrBoardSize, int **stonePlacement, int borders) {
    for (int i = 0; i < arrBoardSize; i++){
        for (int j = 0; j < arrBoardSize; j++) {
            if(i == 0 || i == arrBoardSize - 1 || j == 0 || j == arrBoardSize - 1) stonePlacement[i][j] = borders;
            else stonePlacement[i][j] = 0;
        }
    }
}

int SurroundingCheck(int player, int opponent, int **stonePlacement, int **tempStonePlacement, int i, int j){
    static int point;
    if (stonePlacement[i][j] == opponent
        && ((stonePlacement[i - 1][j] == 0)
            || (stonePlacement[i + 1][j] == 0)
            || (stonePlacement[i][j - 1] == 0)
            || (stonePlacement[i][j + 1] == 0))) return 0;

    tempStonePlacement[i][j] = opponent;
    if((stonePlacement[i - 1][j] == player || stonePlacement[i - 1][j] == 3 || tempStonePlacement[i - 1][j] == opponent)
    && (stonePlacement[i + 1][j] == player || stonePlacement[i + 1][j] == 3 || tempStonePlacement[i + 1][j] == opponent)
    && (stonePlacement[i][j + 1] == player || stonePlacement[i][j + 1] == 3 || tempStonePlacement[i][j + 1] == opponent)
    && (stonePlacement[i][j - 1] == player || stonePlacement[i][j - 1] == 3 || tempStonePlacement[i][j - 1] == opponent))
        return 1;


    if(stonePlacement[i][j - 1] == opponent & tempStonePlacement[i][j - 1] != opponent){
        point = SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, i, j - 1);
        if(point == 0) return 0;
    }
    if(stonePlacement[i][j + 1] == opponent & tempStonePlacement[i][j + 1] != opponent){
        point = SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, i, j + 1);
        if(point == 0) return 0;
    }
    if(stonePlacement[i - 1][j] == opponent & tempStonePlacement[i - 1][j] != opponent){
        point = SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, i - 1, j);
        if(point == 0) return 0;
    }
    if(stonePlacement[i + 1][j] == opponent & tempStonePlacement[i + 1][j] != opponent){
        point = SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, i + 1, j);
        if(point == 0) return 0;
    }
    return point;
}
bool CheckKO(int **stonePlacement, int **tempStonePlacement, int arrBoardSize){
    for(int i = 0; i < arrBoardSize - 1; i++){
        for(int j = 0; j < arrBoardSize - 1; j++){
            if(stonePlacement[i][j] == tempStonePlacement[i][j]) continue;
            else return false;
        }
    }
    return true;
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

void DisplayLegend(int zn, int zero, char *txt, int x, int y, int counter, int player1Pts, int player2Pts) {
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
}