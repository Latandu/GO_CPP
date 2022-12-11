//
// Created by Latandu on 01/12/2022.
//
#define _CRT_SECURE_NO_WARNINGS

#include "conio2.h"
#include "functions.h"
#include <windows.h>
void CreateBoard(int boardSize) {
    gotoxy(xCord, yCord);
    int lastXCord = boardSize + xCord;
    int lastYCord = boardSize + yCord;

    for (int i = xCord; i <= lastXCord + 1; i++) {
        for (int j = yCord; j <= lastYCord + 1; j++) {
            gotoxy(i, j);
            if (j == yCord || j == lastYCord + 1) cputs("*");
            else if (i == xCord || i == lastXCord + 1) cputs("|");
        }
        cputs("\n");
    }
}

void NKeyFunction(int attr, int back, int *zero, int sign, int *availableColumns, int *availableRows, int arrBoardSize,
                  int **stonePlacement, int **tempStonePlacement, int *zn, float *komi, int *boardSize, int *x,
                  int *y) {
    clrscr();
    gameInfo.counter = 0;
    gameInfo.player1Pts = 0;
    gameInfo.player2Pts = 0;
    initializeBoard(boardSize, sign, availableRows, availableColumns);
    NewBoard(arrBoardSize, stonePlacement, freeSpace);
    NewBoard(arrBoardSize, tempStonePlacement, freeSpace);
    InitializeHandicap(attr, back, *boardSize, arrBoardSize, stonePlacement, zn, zero, x, y, komi);
}

int IKeyFunction(int x, int y, int arrBoardSize, int **stonePlacement, int **koArray1, int **koArray2,
                 int **tempStonePlacement) {
    int confirm;
    gotoxy(legendXPos, legendYPos + 17);
    cputs("Press ENTER to confirm.");
    gotoxy(legendXPos, legendYPos + 18);
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
        if (stonePlacement[tabX][tabY] == player1 || stonePlacement[tabX][tabY] == player2) return 1;
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
                return 1;
            }
        } else {
            if (CheckKO(stonePlacement, koArray2, arrBoardSize)) {
                WriteToArray(arrBoardSize, stonePlacement, koArray1);
                gameInfo.player2Pts--;
                return 1;
            }
        }
        NewBoard(arrBoardSize, tempStonePlacement, freeSpace);
        if (SurroundingCheck(opponent, player, stonePlacement, tempStonePlacement, tabX,
                             tabY, freeSpace)) {
            stonePlacement[tabX][tabY] = freeSpace;
            return 1;
        }
        if (player == player1) WriteToArray(arrBoardSize, koArray1, stonePlacement);
        else WriteToArray(arrBoardSize, koArray2, stonePlacement);

        gameInfo.counter++;
    } else return 1;
    return 0;
}

int FileSaverFunction(int arrBoardSize, int **stonePlacement, int **koArray1, int **koArray2) {
    gotoxy(legendXPos, legendYPos + 15);
    cputs("FILE SAVER");
    char fileName[64] = {0};
    int info;
    ReadFileName(fileName, &info);
    if (info == 1) return 1;
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
    return 0;
}

void
InitializeArrays(int arrBoardSize, int **stonePlacement, int **koArray1, int **koArray2, int **tempStonePlacement) {
    for (int i = 0; i < arrBoardSize; i++) {
        stonePlacement[i] = new int[arrBoardSize];
        tempStonePlacement[i] = new int[arrBoardSize];
        koArray1[i] = new int[arrBoardSize];
        koArray2[i] = new int[arrBoardSize];
    }
    NewBoard(arrBoardSize, stonePlacement, freeSpace);
    NewBoard(arrBoardSize, koArray1, freeSpace);
    NewBoard(arrBoardSize, koArray2, freeSpace);
}

int
MarkDeathStones(int *zn, int attr, int back, int *zero, int boardSize, int arrBoardSize, int **stonePlacement, int *x,
                int *y) {
    do {
        textbackground(BLACK);
        clrscr();
        textcolor(7);
        gotoxy(legendXPos, legendYPos);
        cputs("Finishing the game. Mark dead stones.");
        gotoxy(legendXPos, legendYPos + 1);
        cputs("Press i to mark stones.");
        CreateBoard(boardSize);
        gotoxy(*x, *y);
        if (CheckForBorders(boardSize, x, y)) return 1;
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
                if (stonePlacement[tabX][tabY] == player1 || stonePlacement[tabX][tabY] == player2) {
                    stonePlacement[tabX][tabY] = 0;
                } else continue;
            } else continue;
        }
    } while (*zn != enterKey);
    return 0;
}

void FKeyFunction(int attr, int back, int *zero, char *txt, float komi, int boardSize, int arrBoardSize,
                  int **stonePlacement,
                  int **tempStonePlacement, int *zn, int *x, int *y) {
    MarkDeathStones(zn, attr, back, zero, boardSize, arrBoardSize, stonePlacement, x, y);
    for (int m = 1; m < 3; m++) {
        int player = m;
        int opponent;
        if (player == player1) opponent = player2;
        else opponent = player1;
        if (gameInfo.counter >= 2)
            SurroundingLogic(arrBoardSize, stonePlacement, tempStonePlacement, player, freeSpace, opponent,
                             &gameInfo.player1Pts, &gameInfo.player2Pts);
    }
    gotoxy(legendXPos, legendYPos + 3);
    cputs("Game finished, press q to exit");
    gotoxy(legendXPos, legendYPos + 5);
    sprintf(txt, "Player 1 points: %02d", gameInfo.player1Pts);
    cputs(txt);
    gotoxy(legendXPos, legendYPos + 6);
    sprintf(txt, "Player 2 points: %0.1f", (float) gameInfo.player2Pts + komi);
    cputs(txt);
    do {
        *zn = getch();
    } while (*zn != qKey);
}

int FileLoaderFunction(int arrBoardSize, int **stonePlacement, int **koArray1, int **koArray2) {
    gotoxy(legendXPos, legendYPos + 15);
    cputs("FILE LOADER");
    char fileName[64] = {0};
    int info;
    ReadFileName(fileName, &info);
    if (info == 1) return 1;
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
    return 0;
}

void InitializeHandicap(int attr, int back, int boardSize, int arrBoardSize, int **stonePlacement, int *zn, int *zero,
                        int *x, int *y, float *komi) {
    do {

        textbackground(BLACK);
        clrscr();
        textcolor(7);
        gotoxy(legendXPos, legendYPos);
        cputs("HANDICAP MODE. Press ESC to cancel.");
        CreateBoard(boardSize);
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
            *komi = 6.5;
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
                //ensure that player cannot put one stone onto another
                if (stonePlacement[tabX][tabY] == player1 || stonePlacement[tabX][tabY] == player2) continue;
                stonePlacement[tabX][tabY] = player1;
            } else continue;
        }
        if (*zn == escKey) {
            *komi = 0.5;
            NewBoard(arrBoardSize, stonePlacement, freeSpace);
            break;
        }
    } while (*zn != enterKey);
}

void initializeBoard(int *boardSize, int sign, int *availableRows, int *availableColumns) {
    //screen buffer info to restrict setting sizes bigger than console window
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
                //change char array into long int
                sizeOfBoard = strtol(sizeInput, &ptr, 10);
                //check if board size is bigger than available rows/columns
                if (sizeOfBoard > *availableRows || sizeOfBoard > *availableColumns) {
                    gotoxy(boardXYCord, boardXYCord + 7);
                    cputs("Size too big - try again");
                    continue;
                } else *boardSize = sizeOfBoard;
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

bool CheckForEndOfRecursion(int player, int opponent, int **stonePlacement, int **tempStonePlacement, int i, int j) {
    if ((stonePlacement[i - 1][j] == player || stonePlacement[i - 1][j] == borders ||
         tempStonePlacement[i - 1][j] == opponent)
        && (stonePlacement[i + 1][j] == player || stonePlacement[i + 1][j] == borders ||
            tempStonePlacement[i + 1][j] == opponent)
        && (stonePlacement[i][j + 1] == player || stonePlacement[i][j + 1] == borders ||
            tempStonePlacement[i][j + 1] == opponent)
        && (stonePlacement[i][j - 1] == player || stonePlacement[i][j - 1] == borders ||
            tempStonePlacement[i][j - 1] == opponent)) return true;
    return false;
}

int SurroundingCheck(int player, int opponent, int **stonePlacement, int **tempStonePlacement, int i, int j,
                     int nullSpace) {
    static int point;
    //break if a function finds any null places around
    if (stonePlacement[i][j] == opponent
        && ((stonePlacement[i - 1][j] == nullSpace)
            || (stonePlacement[i + 1][j] == nullSpace)
            || (stonePlacement[i][j - 1] == nullSpace)
            || (stonePlacement[i][j + 1] == nullSpace)))
        return 0;

    tempStonePlacement[i][j] = opponent;
    //check if any more checks can be done to ensure the recursion will not loop itself
    if(CheckForEndOfRecursion(player, opponent, stonePlacement, tempStonePlacement, i, j)) return 1;
    //check every possible side
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
    //go over whole array to find surrounding stones
    for (int i = 1; i < arrBoardSize - 1; i++) {
        for (int j = 1; j < arrBoardSize - 1; j++) {
            NewBoard(arrBoardSize, tempStonePlacement, space);
            //inverted positions for compatibility with 2D array
            if (stonePlacement[j][i] == opponent &&
                SurroundingCheck(player, opponent, stonePlacement, tempStonePlacement, j, i, space) == 1) {
                for (int k = 1; k < arrBoardSize - 1; k++) {
                    for (int l = 1; l < arrBoardSize - 1; l++) {
                        //if found add points to the according player
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
    //if functions finds that the arrangement of stones is the last like last time, return true, to prevent that
    for (int i = 0; i < arrBoardSize - 1; i++) {
        for (int j = 0; j < arrBoardSize - 1; j++) {
            if (stonePlacement[j][i] == koArray[j][i]) continue;
            else return false;
        }
    }
    return true;
}

bool CheckForBorders(int boardSize, int *x, int *y) {
    //ensure player cannot go outside the function
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

    *zero = 1;
    *zn = getch();
    //arrow movement - ensure that there is zero to not mix it with h sign
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
        gotoxy(legendXPos, legendYPos + 16);
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
    gotoxy(legendXPos, legendYPos + 2);
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
    cputs("f: finish the game");
    gotoxy(legendXPos, legendYPos + 10);
    sprintf(txt, "x coordinates: %02d", x - xCord);
    cputs(txt);
    gotoxy(legendXPos, legendYPos + 11);
    sprintf(txt, "y coordinates: %02d", y - yCord);
    cputs(txt);
    gotoxy(legendXPos, legendYPos + 12);
    sprintf(txt, "Player 1 points: %02d", player1Pts);
    cputs(txt);
    gotoxy(legendXPos, legendYPos + 13);
    sprintf(txt, "Player 2 points: %02d", player2Pts);
    cputs(txt);
    gotoxy(legendXPos, legendYPos + 14);
    sprintf(txt, "counter: %02d", counter);
    cputs(txt);
}