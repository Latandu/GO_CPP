//
// Created by Latandu on 01/12/2022.
//

#ifndef GO_CPP_FUNCTIONS_H
#define GO_CPP_FUNCTIONS_H
#define _CRT_SECURE_NO_WARNINGS
#define xCord 5
#define yCord 5
#define aKey 0x61
#define bKey 0x62
#define cKey 0x63
#define legendXPos 60
#define legendYPos 10
#define boardXYCord 3
#include <stdio.h>
#include <stdlib.h>
void CreateBoard(int boardSize);
bool CheckForBorders(int boardSize, int *x, int *y);
void initializeBoard(int *boardSize, int sign);
void ArrowMovement(int *zn, int *zero, int *x, int *y);
void DisplayLegend(int zn, int zero, char *txt, int x, int y, int counter, int player1Pts, int player2Pts);
void NewBoard(int arrBoardSize, int **stonePlacement, int borders);
int SurroundingCheck(int player, int opponent, int **stonePlacement, int **tempStonePlacement, int i, int j);
bool CheckKO(int **stonePlacement, int **tempStonePlacement, int arrBoardSize);
void InsertStone(int arrBoardSize, int **stonePlacement);
#endif //GO_CPP_FUNCTIONS_H
