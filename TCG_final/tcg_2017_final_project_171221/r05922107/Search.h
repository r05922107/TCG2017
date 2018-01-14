#ifndef __search__
#define __search__
#endif

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

#ifndef ANQI
#include "anqi.hh"
#endif

using namespace std;

int MAXINT = 100000;

int DEFAULT_TIME = 8;
int DEEP_LIMIT = 12;

int FLIP_DEFAULT_TIME = 5;
int FLIP_DEEP_LIMIT = 8;


class Search{
	public:
		MOV getMove(BOARD &board, int remain_time){
            if(remain_time < 100000){
                DEFAULT_TIME = 1;
                FLIP_DEFAULT_TIME = 1;
            }


            //who: red = 0, black = 1, no team = -1
            //=> who == -1 => first move, need flip
            if(board.who == -1){
                POS firstMove[4] = {9,10,21,22};
                POS p = firstMove[rand()%4];
                return MOV(p,p);
            }






		}

        struct BOARD copy(struct BOARD &board){
            BOARD newBoard = board;
            return newBoard;
        }

		int NegaScout(BOARD &board, int alpha, int beta, int d, int cntNum){
			MOVLST moveList;
			board.MoveGen(moveList);

			if(moveList.num == 0){
				return evaluate(board);
			}

			if(d == 0){
				return evaluate(board);
			}

            int m = -MAXINT;
            int n = beta;

            for(int i=0; i< moveList.num; i++){
                BOARD nextBoard = copy(board);
                nextBoard.Move(moveList.mov[i]);

                int isFlip = 0;
                if(moveList.mov[i].st == -1){
                    isFlip = 1;
                }

                int t = -NegaScout(nextBoard, -n, -max(alpha, m), d-1, cntNum - isFlip);
                if(t > m){
                    if(n == beta || d < 3 || t >= beta){
                        m = t;
                    }else{
                        m = -NegaScout(nextBoard, -beta, -t, d-1, cntNum - isFlip);
                    }
                }

                if(m >= beta){
                    return m;
                }
                n = max(alpha, m) + 1;
            }

            return m;
		}

		int evaluate(BOARD &board){
			board.ChkLose();
		}






};