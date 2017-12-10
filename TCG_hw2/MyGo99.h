#ifndef MYGO99
#define MYGO99

#define BOARDSIZE        9
#define BOUNDARYSIZE    11
#define COMMANDLENGTH 1000
#define DEFAULTTIME     10
#define DEFAULTKOMI      7

#define MAXGAMELENGTH 1000
#define MAXSTRING       50
#define MAXDIRECTION     4

#define NUMINTERSECTION 81
#define HISTORYLENGTH   200

#define EMPTY            0
#define BLACK            1
#define WHITE            2
#define BOUNDARY         3

#define SELF             1
#define OPPONENT         2

#define NUMGTPCOMMANDS      15

#define LOCALVERSION      1
#define GTPVERSION        2

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

int gen_legal_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], int MoveList[HISTORYLENGTH]);

int rand_pick_move(int num_legal_moves, int MoveList[HISTORYLENGTH]);

void do_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int move);






#endif