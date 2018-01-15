#ifndef __search__
#define __search__
#endif


bool isEndFlag;


#include <iostream>
#include <cstdio>
#include <vector>


#ifndef __bitboard__
#include "bitboard.h"
#endif

#ifndef ANQI
#include "anqi.hh"
#endif


using namespace std;

int DEFAULT_TIME = 8;
int DEEP_LIMIT = 12;

int FDEFAULT_TIME = 5;
int FDEEP_LIMIT = 8;


clock_t Tick;     // 開始時刻
clock_t TimeOut;  // 時限

BitboardController controller;

bool TimeUp() {
	return clock() - Tick > TimeOut;
}



class Search{
public:
	void checkEndGame(Bitboard &quickboard){
		isEndFlag = false;
		if(controller.getUnflipCount(quickboard) == 0 && 
				controller.getTotalChess(quickboard) < 12) {
			isEndFlag=true;
			cout << "Start End Game" << endl;
		}
	}

	MOV getMove(BOARD &slow_board, int remain_time){
		

		if(remain_time < 100000){
			DEFAULT_TIME = 1;
			FDEFAULT_TIME = 1;
		}

		// just random pick one dark in first turn
		if(slow_board.who==-1){
			POS firstMove[4]={9,10,21,22};
			POS p=firstMove[rand()%4];
			return MOV(p,p);
		}

		// change slowboard to quickboard(bitboard)
		Bitboard quickboard;
		controller.init_board(quickboard);

		controller.generate_board(quickboard, slow_board);


		// check if end game
		checkEndGame(quickboard);

	
		/****** search for move ******/
		Tick = clock();
		TimeOut = DEFAULT_TIME * CLOCKS_PER_SEC;


		MOVLST lst;
		MOV best_mov;


		controller.get_valid_moves(quickboard, lst);
		int notFlipCnt = controller.getUnflipCount(quickboard);


		if(notFlipCnt == 0){
			if((quickboard.who == 0 && __builtin_popcountll(quickboard.black) == 1)
				|| (quickboard.who == 1 && __builtin_popcountll(quickboard.red) == 1)
				){
				MOVLST wlist;
				cout << "last one to eat!" << endl;
				controller.get_valid_moves(quickboard, wlist, true);
				if(wlist.num != 0){
					cout << "win return" << endl;
					return wlist.mov[0];
				}
			}
		}


		for(int d=1; d<=DEEP_LIMIT; d++){ //iterative deepening
			if(TimeUp()){
				break;
			}

			if(d>1) 
				lst.sort();

			best_mov.score = -MAXINT; //discard previos iteration result
			for(int i = 0; i < lst.num; i++) {
				Bitboard next_board = controller.copy(quickboard);
				controller.update(next_board, lst.mov[i]);
				lst.mov[i].score = -NegaScout(next_board, -MAXINT, MAXINT, d, notFlipCnt);
				if(lst.mov[i].score > best_mov.score){
					best_mov = lst.mov[i];
					best_mov.score = lst.mov[i].score;
				}
				if(best_mov.score == MAXINT) {
					break;
				}
			}
			if(best_mov.score == MAXINT) 
				break; 

			cout << "d is:" << d << endl;
		}



		if(controller.check_eat_move(quickboard,best_mov)){
			return best_mov;
		}

		if(notFlipCnt == 0 || best_mov.score == MAXINT){
			return best_mov;
		}
		

		/****** search for flip ******/
		Tick = clock();
		TimeOut = FDEFAULT_TIME * CLOCKS_PER_SEC;

		MOVLST flst;
		vector<FLIP_PROB> plst;
		MOV best_fmov;
		int fscore;
		int fscore_worst = -MAXINT;

		controller.get_reveal_moves(quickboard, flst);

		controller.get_unflip_prob(quickboard, plst);

		for(int d=1; d<=FDEEP_LIMIT; d++){ //iterative deepening
			if(TimeUp()){
				break;
			}
			if(d>1) {
				flst.sort();
			}

			best_fmov.score = -MAXINT; //discard prevois iteration result

			for(int i = 0; i < flst.num; i++) {

				for(int j = 0; j < plst.size(); j++){
					Bitboard next_board = controller.copy(quickboard);
					controller.reveal(next_board, flst.mov[i], plst[j].fin);
					fscore = -NegaScout(next_board, -MAXINT, MAXINT, d, notFlipCnt-1);
					// if(fscore < fscore_worst){
					// 	flst.mov[i].score = fscore;	
					// }
					flst.mov[i].score += (int)((double)fscore * plst[j].prob);
				}
				if(flst.mov[i].score > best_fmov.score){
					best_fmov = flst.mov[i];
					best_fmov.score = flst.mov[i].score;
				}
				if(best_fmov.score == MAXINT) {
					break;
				}
			}

			if(best_fmov.score == MAXINT) 
				break; 

			cout << "f_d is:" << d << endl;
		}

		// for(int i = 0; i < flst.num; i++){
		// 	cout << flst.mov[i].score << endl;
		// }

		if(best_fmov.score > 0 || best_mov.st == -1){
			return best_fmov;
		}

		return best_mov;

	}


	int NegaScout(Bitboard &quickboard, int alpha, int beta, int d, int notFlipCnt){

		MOVLST lst;

		controller.get_valid_moves(quickboard, lst);

		if(lst.num==0){ // no moves to go
			return controller.evaluate(quickboard);
		}

		if(d <= 0){	// quiescent search
			controller.get_valid_moves(quickboard, lst, true); 
			// cout << "in quiescent search, lst.num: " << lst.num <<", d is: " << d << endl;
			// controller.Display(quickboard);
			if(lst.num==0)
				return controller.evaluate(quickboard);
		}

		if(d > 0 && controller.getUnflipCount(quickboard) > 0){ // enable null move
			lst.mov[lst.num++]=MOV(-1,-1); 
		}

		int m = -MAXINT;
		int n = beta;
		
		if(d>0){ 
			Entry *e = HT.get(quickboard);
			if(e->valid && e->depth > d){
				if(e->exact){
					return e->value;
				}
				m = e->value;
			}
		}

		//search deeper
		for(int i=0;i<lst.num;i++){
			Bitboard next_board = controller.copy(quickboard);
			
			controller.update(next_board, lst.mov[i]);

			int isFlip=0;
			if(lst.mov[i].st == -1){
				isFlip=1;
			}

			int t = -NegaScout(next_board, -n, -max(alpha,m), d-1, notFlipCnt-isFlip); //null window search

			if(t > m){
				if(n == beta || (d < 3 && d > 0) || t >= beta) 
					m=t; 
				else {
					m = -NegaScout(next_board, -beta, -t,  d-1, notFlipCnt-isFlip); //re-search
				}
			}

			if(m>=beta){
				if(d > 0){
					HT.update(quickboard, m, d, false);					
				}
				return m; 
			}

			n = max(alpha,m) + 1; 
		}
		if(d > 0)
			HT.update(quickboard, m, d, true);	
		return m;
	}

};