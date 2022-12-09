//
// Created by Latandu on 01/12/2022.
//

#ifndef GO_CPP_FUNCTIONS_H
#define GO_CPP_FUNCTIONS_H
#define _CRT_SECURE_NO_WARNINGS
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
#define dKey 0x64
#define enterKey 0x0d
#define escKey 0x1b
#define board9 9
#define board13 13
#define board19 19
#define player1 1
#define player2 2
#define freeSpace 0
#define borders 3
#define colorRed 3
#define colorBlue 4
#include <stdio.h>
#include <stdlib.h>
void CreateBoard(int boardSize, const int* availableRows, const int* availableColumns);
bool CheckForBorders(int boardSize, int *x, int *y);
void initializeBoard(int *boardSize, int sign, int* availableRows, int* availableColumns);
void ArrowMovement(int *zn, int *zero, int *x, int *y);
void InitializeHandicap(int attr, int back, int boardSize, int arrBoardSize, int **stonePlacement, int *zn, int *zero,
                        int *x, int *y,  int* availableRows, int* availableColumns);
void ReadFileName(char *fileName, int *info);
void DisplayLegend(char *txt, int x, int y, int counter, int player1Pts, int player2Pts);
void NewBoard(int arrBoardSize, int **stonePlacement);
int SurroundingCheck(int player, int opponent, int **stonePlacement, int **tempStonePlacement, int i, int j);
bool CheckKO(int **stonePlacement, int **koArray, int arrBoardSize);
void InsertStone(int arrBoardSize, int **stonePlacement);
void WriteToArray(int arrBoardSize, int **arrayWrite, int **arrayRead);
#endif //GO_CPP_FUNCTIONS_H
