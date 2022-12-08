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
#include <stdio.h>
#include <stdlib.h>
void CreateBoard(int boardSize);
bool CheckForBorders(int boardSize, int *x, int *y);
void initializeBoard(int *boardSize, int sign);
void ArrowMovement(int *zn, int *zero, int *x, int *y);
void InitializeHandicap(int attr, int back, int boardSize, int arrBoardSize, int **stonePlacement, int *zn, int *zero,
                        int *x, int *y);
void ReadFileName(char *fileName, int *info);
void DisplayLegend(int zn, int zero, char *txt, int x, int y, int counter, int player1Pts, int player2Pts);
void NewBoard(int arrBoardSize, int **stonePlacement, int borders);
int SurroundingCheck(int player, int opponent, int **stonePlacement, int **tempStonePlacement, int i, int j);
bool CheckKO(int **stonePlacement, int **koArray, int arrBoardSize);
void InsertStone(int arrBoardSize, int **stonePlacement);
#endif //GO_CPP_FUNCTIONS_H
