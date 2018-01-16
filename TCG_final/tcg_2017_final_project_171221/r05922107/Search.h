#ifndef __search__
#define __search__
#endif

#include <iostream>
#include <cstdio>
#include <vector>
//#include <algorithm>

#ifndef ANQI
#include "anqi.hh"
#endif

using namespace std;

int MAXINT = 1000000;

int DEFAULT_TIME = 8;
int DEEP_LIMIT = 12;

int FLIP_DEFAULT_TIME = 5;
int FLIP_DEEP_LIMIT = 8;

int WIN_POINT = 999990;


int STATIC_VAL[16] = {
        11000, 4900, 2400, 1100, 500, 3000, 50,
        11000, 4900, 2400, 1100, 500, 3000, 50,
        0, 0
};

int DYNAMIC_VAL[16] = {
        11000, 4900, 2400, 1100, 500, 3000, 50,
        11000, 4900, 2400, 1100, 500, 3000, 50,
        0, 0
};

int K_ENEMY_VAL[6] = {11000, 6000, 4000, 3000, 2300, 2000};
int ENEMY_VAL[9] = {11000, 4900, 2400, 1100, 500, 240, 110, 50, 20};

clock_t startTime;  // start time
clock_t timeOut;

bool isEndPhase;

int timeUp(){
#ifdef _WINDOWS
    return GetTickCount()-startTime>=timeOut;
#else
    return clock() - startTime > timeOut;
#endif
}

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

            //search move
            startTime = clock();
            timeOut = DEFAULT_TIME * CLOCKS_PER_SEC;

            MOVLST moveList;
            MOV bestMove;

            board.MoveGen(moveList);
            int cntNum = board.getCntNum();

            //some heuristic


            //iterative deepening
            for(int d=1; d< DEEP_LIMIT; d++){
                if(timeUp()){
                    break;
                }

                if(d>1){
                    moveList.sort();
                }

                bestMove.score = -MAXINT;

                for(int i=0; i< moveList.num; i++){
                    BOARD nextBoard = copy(board);
                    nextBoard.Move(moveList.mov[i]);
                    moveList.mov[i].score = -NegaScout(nextBoard, -MAXINT, MAXINT, d, cntNum);
                    if(moveList.mov[i].score > bestMove.score){
                        bestMove = moveList.mov[i];
                        bestMove.score = moveList.mov[i].score;
                    }
                    if(bestMove.score == MAXINT){
                        break;
                    }
                }

                cout << "depth = " << d << endl;
            }

            //some heuristic to fast return
            if(cntNum == 0 || bestMove.score == MAXINT){
                return bestMove;
            }


            //search flip
            startTime = clock();
            timeOut = DEFAULT_TIME * CLOCKS_PER_SEC;

            MOVLST flipList;
            vector<FLIP_PROB> probList;
            MOV bestFlip;
            int flipScore;

            getFlipMove(board, flipList);
            getCntProbability(board, probList);

            //iterative deepening
            for(int d=0; d<FLIP_DEEP_LIMIT; d++){
                if(timeUp()){
                    break;
                }

                if(d>1){
                    flipList.sort();
                }

                bestFlip.score = -MAXINT;

                for(int i=0; i<flipList.num; i++){
                    for(int j=0; j<probList.size(); j++){
                        BOARD nextBoard = copy(board);
                        nextBoard.DoMove(flipList.mov[i], probList[j].fin);
                        flipScore = -NegaScout(nextBoard, -MAXINT, MAXINT, d, cntNum-1);
                        flipList.mov[i].score += (int)((double)flipScore * probList[j].probability);
                    }
                    if(flipList.mov[i].score > bestFlip.score){
                        bestFlip = flipList.mov[i];
                        bestFlip.score = flipList.mov[i].score;
                    }
                    if(bestFlip.score == MAXINT){
                        break;
                    }
                }

                cout << "flip depth = " << d << endl;
            }

            //flip first?
            if(bestFlip.score > 0 || bestMove.st == -1){
                return bestFlip;
            }

            return bestMove;
		}

        void getFlipMove(BOARD &board, MOVLST &flipList){
            flipList.num = 0;
            for(int i=0; i<32; i++){
                if(board.fin[i] == FIN_X){
                    flipList.mov[flipList.num++] = MOV(i, i);
                }
            }
        }

        void getCntProbability(BOARD &board, vector<FLIP_PROB> &probList){
            int cnt = board.getCntNum();
            for(int i=0; i<14; i++){
                if(board.cnt[i] > 0){
                    FLIP_PROB flip_prob;
                    flip_prob.fin = FIN(i);
                    flip_prob.probability = (double)board.cnt[i] / (double)cnt;
                    probList.push_back(flip_prob);
                }
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

            //quiescent search




            int m = -MAXINT;
            int n = beta;

            //transposition table hit or not





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

            //update transposition table




            return m;
		}


		int evaluate(BOARD &board){
            int blackNum = 0;  //bright black chess num
            int redNum = 0;  //bright red chess num
            int totalNum = 0;  //total bright and dark chess num
            int live[14] = {0};  //every chess alive num

			for(int i=0; i<32; i++){
                if(board.fin[i] < 7){
                    redNum++;
                    live[board.fin[i]]++;
                }else if(board.fin[i] < 14){
                    blackNum++;
                    live[board.fin[i]]++;
                }
                if(board.fin[i] != FIN_E){
                    totalNum++;
                }
            }

            //check end phase
            isEndPhase = false;
            if(totalNum == redNum+blackNum && totalNum < 12){
                isEndPhase = true;
            }


            int cntNum = board.getCntNum();

            if(redNum ==0 && cntNum == 0){
                if(board.who == 0){
                    return -WIN_POINT;
                }else{
                    return WIN_POINT;
                }
            }
            if(blackNum ==0 && cntNum ==0){
                if(board.who == 1){
                    return -WIN_POINT;
                }else{
                    return WIN_POINT;
                }
            }

            //red and black total value
            int value[2] = {0, 0};

            //count live
            for(int i=0; i<14; i++){
                live[i] += board.cnt[i];
            }

            //dynamic piece value
            bool isDynamic = false;
            if((live[0]>0 && live[13] > 0) || (live[7] > 0 && live[6] > 0)){
                isDynamic = true;

                //king
                //the enemy pawn number decide king's value
                value[0] += K_ENEMY_VAL[live[13]];
                value[1] += K_ENEMY_VAL[live[6]];

                if(isEndPhase){
                    DYNAMIC_VAL[0] = K_ENEMY_VAL[live[13]];
                    DYNAMIC_VAL[7] = K_ENEMY_VAL[live[6]];
                }

                //G, M, R, N dynamic value
                //count enemy G,M,R,N,C number as enemies
                int enemies[2] = {0};
                for(int i=0; i< 5; i++){
                    enemies[0] += live[i+7];
                    enemies[1] += live[i];

                    //decide G, M, R, N value according to how many pieces can eat them
                    if(i < 4){
                        value[0] += ENEMY_VAL[enemies[0]] * live[i+1];
                        value[1] += ENEMY_VAL[enemies[1]] * live[i+8];

                        if(isEndPhase){
                            DYNAMIC_VAL[i+1] = ENEMY_VAL[enemies[0]];
                            DYNAMIC_VAL[i+8] = ENEMY_VAL[enemies[1]];
                        }
                    }
                }
                //int ENEMY_VAL[9] = {11000, 4900, 2400, 1100, 500, 240, 110, 50, 20};

                //Cannon
                //Cannon's value use static value
                value[0] += live[5] * STATIC_VAL[5];
                value[1] += live[12] * STATIC_VAL[12];

                //Pawn
                //if enemy's king is not dead, give pawn more value
                value[0] += ENEMY_VAL[enemies[0] - live[7]] * live[6];
                value[1] += ENEMY_VAL[enemies[1] - live[0]] * live[13];

                if(isEndPhase){
                    DYNAMIC_VAL[6] = ENEMY_VAL[enemies[0] - live[7]];
                    DYNAMIC_VAL[13] = ENEMY_VAL[enemies[1] - live[0]];
                }



            }else{  //static piece value
                for(int i=0; i< 7; i++){
                    value[0] += live[i] * STATIC_VAL[i];
                    value[1] += live[i+7] * STATIC_VAL[i+7];
                }

                //edge decreasing score
                //edge -5 point, corner -10 point
                for(int i=0; i<32; i++){
                    if(i%4 == 0 || i%4 == 3){
                        CLR clr = GetColor(board.fin[i]);
                        if(clr != -1){
                            value[clr] -= 5;
                        }
                    }

                    if((i>=0 && i<=3) || (i>=28 && i<=31)){
                        CLR clr = GetColor(board.fin[i]);
                        if(clr != -1){
                            value[clr] -= 5;
                        }
                    }
                }
            }


            //end phase
//            if(isEndPhase){
//                int *endValue;
//                if(isDynamic){
//                    endValue = DYNAMIC_VAL;
//                }else{
//                    endValue = STATIC_VAL;
//                }
//
//                for(int i=0; i<7; i++){
//
//                }
//
//
//
//                //edge decreasing score
//                //edge -20 point, corner -40 point
//                for(int i=0; i<32; i++){
//                    if(i%4 == 0 || i%4 == 3){
//                        CLR clr = GetColor(board.fin[i]);
//                        if(clr != -1){
//                            value[clr] -= 20;
//                        }
//                    }
//
//                    if((i>=0 && i<=3) || (i>=28 && i<=31)){
//                        CLR clr = GetColor(board.fin[i]);
//                        if(clr != -1){
//                            value[clr] -= 20;
//                        }
//                    }
//                }
//            }

            if(board.who == 0){
                return value[0] - value[1];
            }else{
                return value[1] - value[0];
            }

		}







};