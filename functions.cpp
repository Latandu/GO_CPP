//
// Created by Latandu on 01/12/2022.
//
#define _CRT_SECURE_NO_WARNINGS

#include "conio2.h"
#include "functions.h"
#include <windows.h>

void CreateBoard(int boardSize, const int *availableRows, const int *availableColumns) {
    gotoxy(xCord, yCord);
    int lastXCord = boardSize + xCord;
    int lastYCord = boardSize + yCord;
    int tempXCord = boardSize + xCord;
    int tempYCord = boardSize + yCord;
    if (*availableRows < boardSize) tempYCord = *availableRows + yCord;
    if (*availableColumns < boardSize) tempXCord = *availableColumns + xCord;

    for (int i = xCord; i <= tempXCord + 1; i++) {
        for (int j = yCord; j <= tempYCord + 1; j++) {
            gotoxy(i, j);
            if (j == yCord || j == lastYCord + 1) cputs("*");
            else if (i == xCord || i == lastXCord + 1) cputs("|");
        }
        cputs("\n");
    }
}

void InitializeHandicap(int attr, int back, int boardSize, int arrBoardSize, int **stonePlacement, int *zn, int *zero,
                        int *x, int *y, int *availableRows, int *availableColumns, float* komi) {
    do {

        textbackground(BLACK);
        clrscr();
        textcolor(7);
        gotoxy(legendXPos, legendYPos);
        cputs("HANDICAP MODE. Press ESC to cancel.");
        CreateBoard(boardSize, availableRows, availableColumns);
        gotoxy(*x, *y);
        if (CheckForBorders(boardSize, x, y)) continue;
        InsertStone(arrBoardSize, stonePlacement);
        gotoxy(*x, *y);
        textcolor(attr);
        textbackground(back);
        putch('*');
        *zero = 0;
        *zn = getch();
        if (*zn == moveArrow) ArrowMovement(zn, zero, x, y);
        if (*zn == iKey) {
            int confirm;
            gotoxy(legendXPos, legendYPos + 16);
            cputs("Press ENTER to confirm.");
            gotoxy(legendXPos, legendYPos + 17);
            cputs("Press ESC to cancel.");
            do {
                confirm = getch();
            } while (confirm != enterKey && confirm != escKey);
            if (confirm == enterKey) {
                int tabX = *y - yCord;
                int tabY = *x - xCord;
                if (stonePlacement[tabX][tabY] == player1 || stonePlacement[tabX][tabY] == player2) continue;
                stonePlacement[tabX][tabY] = player1;
            } else continue;
        }
        if (*zn == escKey) {
            NewBoard(arrBoardSize, stonePlacement, freeSpace);
            break;
        }
    } while (*zn != enterKey);
}

void initializeBoard(int *boardSize, int sign, int *availableRows, int *availableColumns) {
    CONSOLE_SCREEN_BUFFER_INFO screenBuffer;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenBuffer);
    *availableRows = screenBuffer.srWindow.Bottom - screenBuffer.srWindow.Top - yCord - 1;
    *availableColumns = screenBuffer.srWindow.Right - screenBuffer.srWindow.Left - xCord - 1;
    while (true) {
        int input = 0;
        int sizeOfBoard;
        char sizeInput[4] = {'\0'};
        char text[32];
        char *ptr;
        gotoxy(boardXYCord, boardXYCord);
        // we print out a text at a given cursor position
        // the cursor will move by the length of the text
        cputs("a) board 9x9");
        gotoxy(boardXYCord, boardXYCord + 1);
        cputs("b) board 13x13");
        gotoxy(boardXYCord, boardXYCord + 2);
        cputs("c) board 19x19");
        gotoxy(boardXYCord, boardXYCord + 3);
        cputs("d) Custom size");
        sign = getch();
        switch (sign) {
            case aKey:
                *boardSize = board9;
                break;
            case bKey:
                *boardSize = board13;
                break;
            case cKey:
                *boardSize = board19;
                break;
            case dKey:
                while (true) {
                    gotoxy(legendXPos, legendYPos + 6);
                    sprintf(text, "Enter a size: %s", sizeInput);
                    cputs(text);
                    sign = getch();
                    if (sign == enterKey) {
                        sizeInput[input] = '\000';
                        break;
                    }
                    sizeInput[input] = (char) sign;
                    input++;
                }

                sizeOfBoard = strtol(sizeInput, &ptr, 10);
                *boardSize = sizeOfBoard;
                break;
            default:
                gotoxy(boardXYCord, boardXYCord + 5);
                cputs("Incorrect key, try again");
                continue;
        }
        break;
    }
}

void NewBoard(int arrBoardSize, int **stonePlacement, int filling) {
    for (int i = 0; i < arrBoardSize; i++) {
        for (int j = 0; j < arrBoardSize; j++) {
            if (i == 0 || i == arrBoardSize - 1 || j == 0 || j == arrBoardSize - 1) stonePlacement[i][j] = borders;
            else stonePlacement[i][j] = filling;
        }
    }
}

void WriteToArray(int arrBoardSize, int **arrayWrite, int **arrayRead) {
    for (int i = 0; i < arrBoardSize; i++) {
        for (int j = 0; j < arrBoardSize; j++) arrayWrite[j][i] = arrayRead[j][i];
    }
}

int SurroundingCheck(int player, int opponent, int **stonePlacement, int **tempStonePlacement, int i, int j,
                     int nullSpace) {
    static int point;
    if (stonePlacement[i][j] == opponent
        && ((stonePlacement[i - 1][j] == nullSpace)
            || (stonePlacement[i + 1][j] == nullSpace)
            || (stonePlacement[i][j - 1] == nullSpace)
            || (stonePlacement[i][j + 1] == nullSpace)))
        return 0;

    tempStonePlacement[i][j] = opponent;
    if ((stonePlacement[i - 1][j] == player || stonePlacement[i - 1][j] == borders ||
         tempStonePlacement[i - 1][j] == opponent)
        && (stonePlacement[i + 1][j] == player || stonePlacement[i + 1][j] == borders ||
            tempStonePlacement[i + 1][j] == opponent)
        && (stonePlacement[i][j + 1] == player || stonePlacement[i][j + 1] == borders ||
            tempStonePlacement[i][j + 1] == opponent)
        && (stonePlacement[i][j - 1] == player || stonePlacement[i][j - 1] == borders ||
            tempStonePlacement[i][j - 1] == opponent))
        return 1;


    if (stonePlacement[i][j - 1] == opponent && tempStonePlacement[i][j - 1] != opponent) {
        point = SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, i, j - 1, nullSpace);
        if (point == 0) return 0;
    }
    if (stonePlacement[i][j + 1] == opponent && tempStonePlacement[i][j + 1] != opponent) {
        point = SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, i, j + 1, nullSpace);
        if (point == 0) return 0;
    }
    if (stonePlacement[i - 1][j] == opponent && tempStonePlacement[i - 1][j] != opponent) {
        point = SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, i - 1, j, nullSpace);
        if (point == 0) return 0;
    }
    if (stonePlacement[i + 1][j] == opponent && tempStonePlacement[i + 1][j] != opponent) {
        point = SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, i + 1, j, nullSpace);
        if (point == 0) return 0;
    }
    return point;
}

void SurroundingLogic(int arrBoardSize, int **stonePlacement, int **tempStonePlacement, int player, int opponent,
                      int space, int *pts1, int *pts2) {
    for (int i = 1; i < arrBoardSize - 1; i++) {
        for (int j = 1; j < arrBoardSize - 1; j++) {
            NewBoard(arrBoardSize, tempStonePlacement, space);
            //inverted positions for compatibility with 2D array
            if (stonePlacement[j][i] == opponent &&
                SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, j, i, space) == 1) {
                for (int k = 1; k < arrBoardSize - 1; k++) {
                    for (int l = 1; l < arrBoardSize - 1; l++) {
                        if (stonePlacement[l][k] == tempStonePlacement[l][k] && tempStonePlacement[l][k] == opponent) {
                            stonePlacement[l][k] = space;
                            if (player == player1) ++*pts1;
                            else ++*pts2;
                        }
                    }
                }
            }
        }
    }
}

bool CheckKO(int **stonePlacement, int **koArray, int arrBoardSize) {
    for (int i = 0; i < arrBoardSize - 1; i++) {
        for (int j = 0; j < arrBoardSize - 1; j++) {
            if (stonePlacement[j][i] == koArray[j][i]) continue;
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
    for (int i = 1; i < arrBoardSize - 1; i++) {
        for (int j = 1; j < arrBoardSize - 1; j++) {
            if (stonePlacement[j][i] == player1) {
                gotoxy(i + xCord, j + yCord);
                textcolor(colorRed);
                putch('X');
            } else if (stonePlacement[j][i] == player2) {
                gotoxy(i + xCord, j + yCord);
                textcolor(colorBlue);
                putch('O');
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
        case 0x48:
            --*y;
            break;
        case 0x50:
            ++*y;
            break;
        case 0x4b:
            --*x;
            break;
        case 0x4d:
            ++*x;
            break;
        default:
            break;
    }

}

void ReadFileName(char *fileName, int *info) {
    *info = 0;
    char fileCPuts[64] = {0};
    int k = 0;
    char c;
    do {
        gotoxy(legendXPos, legendYPos + 15);
        sprintf(fileCPuts, "Enter file name: %s", fileName);
        cputs(fileCPuts);
        c = (char) getch();
        if (c == escKey) {
            *info = 1;
            break;
        }
        if (c != enterKey) {
            fileName[k] = c;
            k++;
        }
    } while (c != enterKey && k < 63);
}

void DisplayLegend(char *txt, int x, int y, int counter, int player1Pts, int player2Pts) {
    textbackground(BLACK);
    clrscr();
    textcolor(7);
    gotoxy(legendXPos, legendYPos);
    cputs("Jakub Andrunik 193166");
    gotoxy(legendXPos, legendYPos + 1);
    cputs("Implemented functionalities:");
    gotoxy(legendXPos, legendYPos + 1);
    cputs("a,b,c,d,e,f,g,h,j,k,l");
    gotoxy(legendXPos, legendYPos + 3);
    cputs("arrows: moving the cursor over the board");
    gotoxy(legendXPos, legendYPos + 4);
    cputs("q: quit the program");
    gotoxy(legendXPos, legendYPos + 5);
    cputs("n: start a new game");
    gotoxy(legendXPos, legendYPos + 6);
    cputs("i: place a stone on the board");
    gotoxy(legendXPos, legendYPos + 7);
    cputs("s: save the game state");
    gotoxy(legendXPos, legendYPos + 8);
    cputs("l: load the game state");
    gotoxy(legendXPos, legendYPos + 9);
    sprintf(txt, "x coordinates: %02d", x - xCord);
    cputs(txt);
    gotoxy(legendXPos, legendYPos + 10);
    sprintf(txt, "y coordinates: %02d", y - yCord);
    cputs(txt);
    gotoxy(legendXPos, legendYPos + 11);
    sprintf(txt, "Player 1 points: %02d", player1Pts);
    cputs(txt);
    gotoxy(legendXPos, legendYPos + 12);
    sprintf(txt, "Player 2 points: %02d", player2Pts);
    cputs(txt);
    gotoxy(legendXPos, legendYPos + 13);
    sprintf(txt, "counter: %02d", counter);
    cputs(txt);
}