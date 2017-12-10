#include <iostream>
#include <deque>
#include <climits>
#include <vector>
#include <math.h>
using namespace std;

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

#define C_VALUE        1.18

struct Node
{
	int position[BOUNDARYSIZE][BOUNDARYSIZE];
	Node child[HISTORYLENGTH];
	Node parent;
	double ucb_score;
	double win
	double sim_time
	double total_sim


};


double cul_ucb(double win, double sim_time, double total_sim){
	return win/sim_time + C_VALUE*sqrt(log(total_sim)/sim_time);
}

