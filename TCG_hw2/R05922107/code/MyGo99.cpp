/*
 * This code is provied as a sample code of Hw 2 of "Theory of Computer Game".
 * The "genmove" function will randomly output one of the legal move.
 * This code can only be used within the class.
 *
 * 2015 Nov. Hung-Jui Chang
 * */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <math.h>
//#include <set>
#include <unordered_set>
#include <fstream>
#include <chrono>

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

#define C_VALUE         1.18
#define FIRST_TIME_SIM	50
#define SECOND_TIME_SIM	10
#define RD              0.2
#define DEV_E           0.45
 
using namespace std;
int _board_size = BOARDSIZE;
int _board_boundary = BOUNDARYSIZE;
double _komi =  DEFAULTKOMI-0.5;
const int DirectionX[MAXDIRECTION] = {-1, 0, 1, 0};
const int DirectionY[MAXDIRECTION] = { 0, 1, 0,-1};
const char LabelX[]="0ABCDEFGHJ";









/*
 * This function reset the board, the board intersections are labeled with 0,
 * the boundary intersections are labeled with 3.
 * */
void reset(int Board[BOUNDARYSIZE][BOUNDARYSIZE]) {
    for (int i = 1 ; i <= BOARDSIZE; ++i) {
	for (int j = 1 ; j <= BOARDSIZE; ++j) {
	    Board[i][j] = EMPTY;
	}
    }
    for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
	Board[0][i] = Board[BOUNDARYSIZE-1][i] = Board[i][0] = Board[i][BOUNDARYSIZE-1] = BOUNDARY;
    }
}

/*
 * This function return the total number of liberity of the string of (X, Y) and
 * the string will be label with 'label'.
 * */
int find_liberty(int X, int Y, int label, int Board[BOUNDARYSIZE][BOUNDARYSIZE], int ConnectBoard[BOUNDARYSIZE][BOUNDARYSIZE]) {
    // Label the current intersection
    ConnectBoard[X][Y] |= label;
    int total_liberty = 0;
    for (int d = 0 ; d < MAXDIRECTION; ++d) {
	// Check this intersection has been visited or not
	if ((ConnectBoard[X+DirectionX[d]][Y+DirectionY[d]] & (1<<label) )!= 0) continue;

	// Check this intersection is not visited yet
	ConnectBoard[X+DirectionX[d]][Y+DirectionY[d]] |=(1<<label);
	// This neighboorhood is empty
	if (Board[X+DirectionX[d]][Y+DirectionY[d]] == EMPTY){
	    total_liberty++;

		//use for efficacy
		if(total_liberty > 1){
			return total_liberty;
		}
	}
	// This neighboorhood is self stone
	else if (Board[X+DirectionX[d]][Y+DirectionY[d]] == Board[X][Y]) {
	    total_liberty += find_liberty(X+DirectionX[d], Y+DirectionY[d], label, Board, ConnectBoard);

		//use for efficacy
		if(total_liberty > 1){
			return total_liberty;
		}
	}
    }
    return total_liberty;
}

/*
 * This function count the liberties of the given intersection's neighboorhod
 * */
void count_liberty(int X, int Y, int Board[BOUNDARYSIZE][BOUNDARYSIZE], int Liberties[MAXDIRECTION]) {
    int ConnectBoard[BOUNDARYSIZE][BOUNDARYSIZE];
    // Initial the ConnectBoard
    for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
	for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
	    ConnectBoard[i][j] = 0;
	}
    }
    // Find the same connect component and its liberity
    for (int d = 0 ; d < MAXDIRECTION; ++d) {
	Liberties[d] = 0;
	if (Board[X+DirectionX[d]][Y+DirectionY[d]] == BLACK ||  
	    Board[X+DirectionX[d]][Y+DirectionY[d]] == WHITE    ) {
	    Liberties[d] = find_liberty(X+DirectionX[d], Y+DirectionY[d], d, Board, ConnectBoard);
	}
    }
}

/*
 * This function count the number of empty, self, opponent, and boundary intersections of the neighboorhod
 * and saves the type in NeighboorhoodState.
 * */
void count_neighboorhood_state(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn, int* empt, int* self, int* oppo ,int* boun, int NeighboorhoodState[MAXDIRECTION]) {
    for (int d = 0 ; d < MAXDIRECTION; ++d) {
	// check the number of nonempty neighbor
	switch(Board[X+DirectionX[d]][Y+DirectionY[d]]) {
	    case EMPTY:    (*empt)++; 
			   NeighboorhoodState[d] = EMPTY;
			   break;
	    case BLACK:    if (turn == BLACK) {
			       (*self)++;
			       NeighboorhoodState[d] = SELF;
			   }
			   else {
			       (*oppo)++;
			       NeighboorhoodState[d] = OPPONENT;
			   }
			   break;
	    case WHITE:    if (turn == WHITE) {
			       (*self)++;
			       NeighboorhoodState[d] = SELF;
			   }
			   else {
			       (*oppo)++;
			       NeighboorhoodState[d] = OPPONENT;
			   }
			   break;
	    case BOUNDARY: (*boun)++;
			   NeighboorhoodState[d] = BOUNDARY;
			   break;
	}
    }
}

/*
 * This function remove the connect component contains (X, Y) with color "turn" 
 * And return the number of remove stones.
 * */
int remove_piece(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn) {
    int remove_stones = (Board[X][Y]==EMPTY)?0:1;
    Board[X][Y] = EMPTY;
    for (int d = 0; d < MAXDIRECTION; ++d) {
	if (Board[X+DirectionX[d]][Y+DirectionY[d]] == turn) {
	    remove_stones += remove_piece(Board, X+DirectionX[d], Y+DirectionY[d], turn);
	}
    }
    return remove_stones;
}
/*
 * This function update Board with place turn's piece at (X,Y).
 * Note that this function will not check if (X, Y) is a legal move or not.
 * */
void update_board(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn) {
    int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int Liberties[4];
    int NeighboorhoodState[4];
    count_neighboorhood_state(Board, X, Y, turn,
	    &num_neighborhood_empt,
	    &num_neighborhood_self,
	    &num_neighborhood_oppo,
	    &num_neighborhood_boun, NeighboorhoodState);
    // check if there is opponent piece in the neighboorhood
    if (num_neighborhood_oppo != 0) {
	count_liberty(X, Y, Board, Liberties);
	for (int d = 0 ; d < MAXDIRECTION; ++d) {
	    // check if there is opponent component only one liberty
	    if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1 && Board[X+DirectionX[d]][Y+DirectionY[d]]!=EMPTY) {
		remove_piece(Board, X+DirectionX[d], Y+DirectionY[d], Board[X+DirectionX[d]][Y+DirectionY[d]]);
	    }
	}
    }
    Board[X][Y] = turn;
}
/*
 * This function update Board with place turn's piece at (X,Y).
 * Note that this function will check if (X, Y) is a legal move or not.
 * */
int update_board_check(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int X, int Y, int turn) {
    // Check the given coordination is legal or not
    if ( X < 1 || X > BOARDSIZE || Y < 1 || Y > BOARDSIZE || Board[X][Y]!=EMPTY)
	return 0;
    int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int Liberties[4];
    int NeighboorhoodState[4];
    count_neighboorhood_state(Board, X, Y, turn,
	    &num_neighborhood_empt,
	    &num_neighborhood_self,
	    &num_neighborhood_oppo,
	    &num_neighborhood_boun, NeighboorhoodState);
    // Check if the move is a legal move
    // Condition 1: there is a empty intersection in the neighboorhood
    int legal_flag = 0;
    count_liberty(X, Y, Board, Liberties);
    if (num_neighborhood_empt != 0) {
	legal_flag = 1;
    }
    else {
	// Condition 2: there is a self string has more than one liberty
	for (int d = 0; d < MAXDIRECTION; ++d) {
	    if (NeighboorhoodState[d] == SELF && Liberties[d] > 1) {
		legal_flag = 1;
	    }
	}
	if (legal_flag == 0) {
	// Condition 3: there is a opponent string has exactly one liberty
	    for (int d = 0; d < MAXDIRECTION; ++d) {
		if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1) {
		    legal_flag = 1;
		}
	    }
	}
    }

    if (legal_flag == 1) {
    // check if there is opponent piece in the neighboorhood
	if (num_neighborhood_oppo != 0) {
	    for (int d = 0 ; d < MAXDIRECTION; ++d) {
		// check if there is opponent component only one liberty
		if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1 && Board[X+DirectionX[d]][Y+DirectionY[d]]!=EMPTY) {
		    remove_piece(Board, X+DirectionX[d], Y+DirectionY[d], Board[X+DirectionX[d]][Y+DirectionY[d]]);
		}
	    }
	}
	Board[X][Y] = turn;
    }

    return (legal_flag==1)?1:0;
}

/*
 * This function return the number of legal moves with clor "turn" and
 * saves all legal moves in MoveList
 * */
int gen_legal_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], int MoveList[HISTORYLENGTH]) {
    int NextBoard[BOUNDARYSIZE][BOUNDARYSIZE];
    int num_neighborhood_self = 0;
    int num_neighborhood_oppo = 0;
    int num_neighborhood_empt = 0;
    int num_neighborhood_boun = 0;
    int legal_moves = 0;
    int next_x, next_y;
    int Liberties[4];
    int NeighboorhoodState[4];
    bool eat_move = 0;
    for (int x = 1 ; x <= BOARDSIZE; ++x) {
	for (int y = 1 ; y <= BOARDSIZE; ++y) {
	    // check if current 
	    if (Board[x][y] == 0) {
		// check the liberty of the neighborhood intersections
		num_neighborhood_self = 0;
		num_neighborhood_oppo = 0;
		num_neighborhood_empt = 0;
		num_neighborhood_boun = 0;
		// count the number of empy, self, opponent, and boundary neighboorhood
		count_neighboorhood_state(Board, x, y, turn,
			&num_neighborhood_empt,
			&num_neighborhood_self,
			&num_neighborhood_oppo,
			&num_neighborhood_boun, NeighboorhoodState);
		// check if the emtpy intersection is a legal move
		next_x = next_y = 0;
		eat_move = 0;
		count_liberty(x, y, Board, Liberties);
		// Case 1: exist empty intersection in the neighborhood
		 if (num_neighborhood_empt > 0) {
		     next_x = x;
		     next_y = y;
		     // check if it is a capture move
		     for (int d = 0 ; d < MAXDIRECTION; ++d) {
			 if (NeighboorhoodState[d] == OPPONENT && Liberties[d] == 1) {
			     eat_move = 1;
			 }
		     }

		 }
		 // Case 2: no empty intersection in the neighborhood
		 else {
		    // Case 2.1: Surround by the self piece
		    if (num_neighborhood_self + num_neighborhood_boun == MAXDIRECTION) {
			int check_flag = 0, check_eye_flag = num_neighborhood_boun;
			for (int d = 0 ; d < MAXDIRECTION; ++d) {
			    // Avoid fill self eye
			    if (NeighboorhoodState[d]==SELF && Liberties[d] > 1) {
				check_eye_flag++;
			    }
			    // Check if there is one self component which has more than one liberty
			    if (NeighboorhoodState[d]==SELF && Liberties[d] > 1) {
				check_flag = 1;
			    }
			}
			if (check_flag == 1 && check_eye_flag!=4) {
			    next_x = x;
			    next_y = y;
			}
		    }	
		    // Case 2.2: Surround by opponent or both side's pieces.
		    else if (num_neighborhood_oppo > 0) {
			int check_flag = 0;
			int eat_flag = 0;
			for (int d = 0 ; d < MAXDIRECTION; ++d) {
			    // Check if there is one self component which has more than one liberty
			    if (NeighboorhoodState[d]==SELF && Liberties[d] > 1) {
				check_flag = 1;
			    }
			    // Check if there is one opponent's component which has exact one liberty
			    if (NeighboorhoodState[d]==OPPONENT && Liberties[d] == 1) {
				eat_flag = 1;
			    }
			}
			if (check_flag == 1) {
			    next_x = x;
			    next_y = y;
			    if (eat_flag == 1) {
				eat_move = 1;
			    }
			}
			else { // check_flag == 0
			    if (eat_flag == 1) {
				next_x = x;
				next_y = y;
				eat_move = 1;
			    }
			}
		    }	
		 }
		 if (next_x !=0 && next_y !=0) {
		 // copy the current board to next board
		    for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
			for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
			    NextBoard[i][j] = Board[i][j];
			}
		    }
		    // do the move
		    // The move is a capture move and the board needs to be updated.
		    if (eat_move == 1) {
			update_board(NextBoard, next_x, next_y, turn);
		    }
		    else {
			NextBoard[x][y] = turn;
		    }
		    // Check the history to avoid the repeat board

             //set next board stone num
             NextBoard[0][0] = 0;
             NextBoard[0][1] = 0;
             NextBoard[0][2] = 0;
             for(int i = 1; i < BOARDSIZE; i++){
                 for(int j = 1; j< BOARDSIZE; j++){
                     if(NextBoard[i][j] == BLACK){
                         NextBoard[0][1]++;
                     }else if(NextBoard[i][j] == WHITE){
                         NextBoard[0][2]++;
                     }
                 }
             }
             NextBoard[0][0] = NextBoard[0][1] + NextBoard[0][2];


			bool repeat_move = 0;
		    for (int t = 0 ; t < game_length; ++t) {
                bool same_stone_num = false;
                bool repeat_flag = 1;
                if(NextBoard[0][0] != GameRecord[t][0][0]){
                    repeat_flag = 0;
                }else if(NextBoard[0][1] != GameRecord[t][0][1]){
                    repeat_flag = 0;
                }else{
                    same_stone_num = 1;
                }
                if(same_stone_num){
                    for (int i = 1; i <=BOARDSIZE; ++i) {
                        for (int j = 1; j <=BOARDSIZE; ++j) {
                            if (NextBoard[i][j] != GameRecord[t][i][j]) {
                                repeat_flag = 0;
                            }
                        }
                    }
                }
                if (repeat_flag == 1) {
                    repeat_move = 1;
                    break;
                }
		    }

		    if (repeat_move == 0) {
			// 3 digit zxy, z means eat or not, and put at (x, y)
			MoveList[legal_moves] = eat_move * 100 + next_x * 10 + y ;
			legal_moves++;
		    }
		 }
	    }
	}
    }
    return legal_moves;
}
/*
 * This function randomly selects one move from the MoveList.
 * */
int rand_pick_move(int num_legal_moves, int MoveList[HISTORYLENGTH]) {
    if (num_legal_moves == 0)
	return 0;
    else {
	int move_id = rand()%num_legal_moves;
	return MoveList[move_id];
    }
}
/*
 * This function update the Board with put 'turn' at (x,y)
 * where x = (move % 100) / 10 and y = move % 10.
 * Note this function will not check 'move' is legal or not.
 * */
void do_move(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int move) {
    int move_x = (move % 100) / 10;
    int move_y = move % 10;
    if (move<100) {
	Board[move_x][move_y] = turn;
    }
    else {
	update_board(Board, move_x, move_y, turn);
    }

}
/* 
 * This function records the current game baord with current
 * game length "game_length"
 * */
void record(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], int game_length) {
    std::copy(&Board[0][0], &Board[0][0] + BOUNDARYSIZE*BOUNDARYSIZE, &GameRecord[game_length][0][0]);
    GameRecord[game_length][0][0] = 0;
    GameRecord[game_length][0][1] = 0;
    GameRecord[game_length][0][2] = 0;
    for(int i = 1; i < BOARDSIZE; i++){
        for(int j = 1; j< BOARDSIZE; j++){
            if(GameRecord[game_length][i][j] == BLACK){
                GameRecord[game_length][0][1]++;
            }else if(GameRecord[game_length][i][j] == WHITE){
                GameRecord[game_length][0][2]++;
            }
        }
    }
    GameRecord[game_length][0][0] = GameRecord[game_length][0][1] + GameRecord[game_length][0][2];
//		for (int i = 0 ; i < BOUNDARYSIZE; ++i) {
//		    for (int j = 0 ; j < BOUNDARYSIZE; ++j) {
//			GameRecord[game_length][i][j] = Board[i][j];
//		    }
//		}

}

/*
 * This function counts the number of points remains in the board by Black's view
 * */
double final_score(int Board[BOUNDARYSIZE][BOUNDARYSIZE]) {
	int black, white;
	black = white = 0;
	int is_black, is_white;
	for (int i = 1 ; i <= BOARDSIZE; ++i) {
		for (int j = 1; j <= BOARDSIZE; ++j) {
			switch(Board[i][j]) {
				case EMPTY:
					is_black = is_white = 0;
					for(int d = 0 ; d < MAXDIRECTION; ++d) {
						if (Board[i+DirectionX[d]][j+DirectionY[d]] == BLACK) is_black = 1;
						if (Board[i+DirectionX[d]][j+DirectionY[d]] == WHITE) is_white = 1;
					}
					if (is_black + is_white == 1) {
						black += is_black;
						white += is_white;
					}
					break;
				case WHITE:
					white++;
					break;
				case BLACK:
					black++;
					break;
			}
		}
	}
	return black - white;
}

struct Node {
	int Board[BOUNDARYSIZE][BOUNDARYSIZE];
	Node *child[HISTORYLENGTH];
	Node *parent;
	double ucb_score;
	double win;
	double sim_time;
	int num_legal_moves;
    int num_after_prunned;
	// int MoveList[HISTORYLENGTH];
	int turn;
	int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE];
	int game_length;
	int move;
	bool expended;
    bool prunned;
};


double calUcb(double win, double sim_time, double total_sim){
	return win/sim_time + C_VALUE*sqrt(log(total_sim)/sim_time);
}

double calSD(double win, double sim_time){
    double sd = sqrt(((win * pow((1 - win/sim_time), 2)) + ((sim_time - win) * pow(win/sim_time, 2)))/sim_time);
    return sd;
}

int change_turn(int turn){
	if(turn == BLACK){
		return WHITE;
	}else{
		return BLACK;
	}
}

Node *newNode(){
	Node *node = new Node;
	// node->Board = NULL;
//	cout << "Board[0][0]: " << node-> Board[0][0] <<  endl;
	// node->child = NULL;
	// node->GameRecord = NULL;
//	cout << "Record[0][0][0]: " << node-> GameRecord[0][0][0] <<  endl;
	node->parent = NULL;
	node->ucb_score = 0.0;
	node->win = 0.0;
	node->sim_time = 0.0;
	node->num_legal_moves = 0;
	node->turn = 0;
	node->game_length = 0;
	node->move = 0;
	node->expended = false;
    node->prunned = false;
    node->num_after_prunned = 0;

	return node;
}

void copyGameRecord(int From[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], int To[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE], int length){
//	cout << "copy record start" << endl;
//	for(int i=0; i<length; i++){
//		for(int j=0; j<BOUNDARYSIZE; j++){
//			for(int k=0; k<BOUNDARYSIZE; k++){
//				To[i][j][k] = From[i][j][k];
//			}
//		}
//	}
	for(int i=0; i < length; i++){
		std::copy(&From[i][0][0], &From[i][0][0] + BOUNDARYSIZE*BOUNDARYSIZE, &To[i][0][0]);
	}
}

void copyBoard(int From[BOUNDARYSIZE][BOUNDARYSIZE], int To[BOUNDARYSIZE][BOUNDARYSIZE]){
//	cout << "From[0][0]" << From[0][0] << endl;
//	cout << "To[0][0]" << To[0][0] << endl;
//	for(int i=0; i<BOUNDARYSIZE; i++){
//		for(int j=0; j<BOUNDARYSIZE; j++){
//			To[i][j] = From[i][j];
//		}
//	}
	std::copy(&From[0][0], &From[0][0] + BOUNDARYSIZE*BOUNDARYSIZE, &To[0][0]);
//	cout << "copy Board end" << endl;
}

//add a new node to the node at child[index],
//and set parent and turn, game_length+1
//set Board, GameRecord as the same with parent
Node *addNode(Node *node, int index){


	// if(node == NULL){
	// 	retNode = newNode();
	// 	retNode->parent = p;
	// 	return retNode;
	// }

	Node *retNode = newNode();
	node->child[index] = retNode;
	retNode->parent = node;

	//set Board, GameRecord, game_length
	copyBoard(node->Board, retNode->Board);
	copyGameRecord(node->GameRecord, retNode->GameRecord, node->game_length);
	retNode->game_length = node->game_length + 1;

	if(node->turn == BLACK){
		retNode->turn = WHITE;
	}else{
		retNode->turn = BLACK;
	}
	return retNode;
}

void delete_tree(Node *node){
    if(node->num_legal_moves == 0){
        delete(node);
        return;
    }
    for(int i=0; i<node->num_legal_moves; i++){
        delete_tree(node->child[i]);
    }
    delete(node);
}

void prunning(Node *node){
    if(node->num_after_prunned <= 1){
        return;
    }
    double sd_child[node->num_legal_moves];

    for(int i=0; i<node->num_legal_moves-1; i++){
        if(node->child[i]->prunned || node->child[i]->sim_time < 100){
            continue;
        }
        //set child1 sd and mean
        Node *childNode = node->child[i];
        double sd;
        if(sd_child[i] > 0.000001){
            sd = sd_child[i];
        }else{
            sd = calSD(childNode->win, childNode->sim_time);
            sd_child[i] = sd;
        }
        double mean = childNode->win/childNode->sim_time;

        for(int j=i+1; j<node->num_legal_moves; j++){
            if(node->child[j]->prunned || node->child[j]->sim_time < 100){
                continue;
            }

            //set child2 sd and mean
            Node *childNode2 = node->child[j];
            double sd2;
            if(sd_child[j] > 0.000001){
                sd2 = sd_child[j];
            }else{
                sd2 = calSD(childNode2->win, childNode2->sim_time);
                sd_child[j] = sd2;
            }
            double mean2 = childNode2->win/childNode2->sim_time;

            if(sd < DEV_E && sd2 < DEV_E){
                if(mean - RD*sd > mean2 + RD*sd2){
                    childNode2->prunned = true;
                    node->num_after_prunned--;
//                    cout << "pruned!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << childNode2->move << " by " << childNode->move << endl;
//                    cout << "mean(" << childNode2->move << ")= " << mean2 << endl;
//                    cout << "mean(" << childNode->move << ")= " << mean << endl;
//                    cout << "mean_right(" << childNode2->move << ")= " << mean2 + RD*sd2 << endl;
//                    cout << "mean_left(" << childNode->move << ")= " << mean - RD*sd << endl;
//                    cout << "sd(" << childNode2->move << ")= " << sd2 << endl;
                }else if(mean + RD*sd < mean2 - RD*sd2){
                    childNode->prunned = true;
                    node->num_after_prunned--;
//                    cout << "pruned!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << childNode->move << " by " << childNode2->move << endl;
//                    cout << "mean(" << childNode->move << ")= " << mean << endl;
//                    cout << "mean(" << childNode2->move << ")= " << mean2 << endl;
//                    cout << "mean_right(" << childNode->move << ")= " << mean + RD*sd << endl;
//                    cout << "mean_left(" << childNode2->move << ")= " << mean2 - RD*sd2 << endl;
//                    cout << "sd(" << childNode->move << ")= " << sd << endl;
                }
            }
        }
    }
}

Node *select(Node *node, int turn, bool endFlag[2]){
//	cout << "selecting start" << endl;
	Node *retNode;
	if(!node->expended ){
		retNode = node;
		return retNode;
	}else if(node->num_legal_moves == 0){
        endFlag[0] = 1;
        retNode = node;
        return retNode;
    }

//	cout << "child not null" << endl;


//    prunning(node);

	//if node.turn == turn => find max child ucb, else find mix ucb
	double maxUcb = -1;
//	double minUcb = 10000;
	int selectNodeIndex = 0;
	for(int i=0; i<node->num_legal_moves; i++){
		Node *childNode = node->child[i];
        if(childNode->prunned){
            continue;
        }
//		cout << "child's move: " << childNode->move <<endl;
//		cout << "win: " << childNode->win <<endl;
//		cout << "sim_time: " << childNode->sim_time <<endl;
//		cout << "total_sim: " << node-> sim_time <<endl;
		if(node->turn == turn){
			childNode->ucb_score = calUcb(childNode->win, childNode->sim_time, node-> sim_time);
		}else{
			childNode->ucb_score = calUcb(childNode->sim_time - childNode->win, childNode->sim_time, node-> sim_time);
		}
		childNode->ucb_score = calUcb(childNode->win, childNode->sim_time, node-> sim_time);
//		cout << "ucb score: " << childNode->ucb_score <<endl;
		if(maxUcb < childNode->ucb_score){
			maxUcb = childNode->ucb_score;
			selectNodeIndex = i;
		}

//		if(node->turn == turn){
////			cout << "max node" << endl;
//			if(maxUcb < childNode->ucb_score){
//				maxUcb = childNode->ucb_score;
//				selectNodeIndex = i;
//			}
//		}else{
////			cout << "min node" << endl;
//			if(minUcb > childNode->ucb_score){
//				minUcb = childNode->ucb_score;
//				selectNodeIndex = i;
//			}
//		}
	}
	retNode = select(node->child[selectNodeIndex], turn, endFlag);
	return retNode;
}

void expand(Node *node){
	int MoveList[HISTORYLENGTH];
	int num_legal_moves = 0;

	num_legal_moves = gen_legal_move(node->Board, node->turn, node->game_length, node->GameRecord, MoveList);

//	cout << "expand node's move: " << node->move << endl;
//	cout << "expand child number: " << num_legal_moves << endl;
	node->num_legal_moves = num_legal_moves;
    node->num_after_prunned = num_legal_moves;
	//find children's Board and GameRecord AND MOVE!!, expanded to true;
	for(int i=0; i<num_legal_moves; i++){
		Node *childNode = addNode(node, i);
		childNode->move = MoveList[i];
		do_move(childNode->Board, node->turn, MoveList[i]);
		record(childNode->Board, childNode->GameRecord, childNode->game_length);
//		cout << "child" << i << "move =" << node->child[i]->move << endl;
	}
	node->expended = true;
}


int simulate(Node *node, int time){
//	cout << "simulate start " << endl;
	int wins = 0;
	for(int i=0; i<time; i++){
		bool finish = false;
		int t_Board[BOUNDARYSIZE][BOUNDARYSIZE];
		//turn will be changed in while, so change to wrong turn first.
		int t_turn = change_turn(node->turn);
		//same, length will be add in while.
		int t_game_length = node->game_length-1;
		int t_GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE];
		//set Board, GameRecord
		copyBoard(node->Board, t_Board);
		copyGameRecord(node->GameRecord, t_GameRecord, node->game_length);
		//flag of a player have passed
		bool pass1 = false;

		while(!finish){
			t_turn = change_turn(t_turn);
			t_game_length += 1;
			int MoveList[HISTORYLENGTH];
			int return_move = 0;
			int num_legal_moves = 0;
			int try_limit = 30;
			int length_limit = 70;
			int trytry = 0;
			bool try_result = false;

			while(trytry <= try_limit && t_game_length < length_limit){

				int rand_x = (rand()%9) + 1;
				int rand_y = (rand()%9) + 1;
//				cout << "rand_x = " << rand_x << endl;
//				cout << "rand_y = " << rand_y << endl;
				if(update_board_check(t_Board, rand_x, rand_y, t_turn) == 1){
					//add checking for last Board is same, use max limit now
//					return_move = rand_x*10 + rand_y;
					try_result = true;
					//for not to be pass.
					num_legal_moves = 1;
					break;
				}
				trytry++;
			}

			if(!try_result){
				num_legal_moves = gen_legal_move(t_Board, t_turn, t_game_length, t_GameRecord, MoveList);
				return_move = rand_pick_move(num_legal_moves, MoveList);
//			cout << "sim turn: " << t_turn << endl;
				do_move(t_Board, t_turn, return_move);
			}


			record(t_Board, t_GameRecord, t_game_length);
			if(num_legal_moves == 0){
				if(!pass1){
					pass1 = true;
				}else{
					finish = true;
				}
			}else{
				pass1 = false;
			}
		}
		//calculate result
		double result;
		result = final_score(t_Board);
		result -= _komi;  //maybe not use komi
		int win_turn = 0;
		if (result > 0.0) { // Black win
			win_turn = BLACK;
		}
		if (result < 0.0) { // White win
			win_turn = WHITE;
		}
		else { // draw

		}
		//node->turn is not the select node turn
		if(node->turn != win_turn){
			wins++;
//			cout << "sim win! ";
		}else{
//			cout << "sim lose! ";
		}
	}

//	if(root_turn != node->turn){
//		wins = time - wins;
//	}

	return wins;
}

void back_propagation(Node *node, int wins, int local_time){
	Node *p = node->parent;
	node->win += wins;
	node->sim_time += local_time;
//	node->total_sim = p->sim_time + local_time;
//	node->ucb_score = calUcb(node->win, node->sim_time, node-> total_sim);
//	cout << "node(" << node->move << ") win rate: " << node->win << "/" << node->sim_time <<endl;
	if(!node->parent){
		return;
	}
	back_propagation(p, wins, local_time);
}


void sim_all_children(Node *node, int root_turn, int time, chrono::system_clock::time_point end_t){
//	cout << "node children num: " << node->num_legal_moves << endl;
	for(int i=0; i < node->num_legal_moves; i++){
		if(chrono::duration_cast<chrono::microseconds>(end_t - chrono::system_clock::now()).count()<0){
//			cout << chrono::duration_cast<chrono::microseconds>(end_t - chrono::system_clock::now()).count() << endl;
			break;
		}
		Node *childNode = node->child[i];
		//sim
		int wins = simulate(childNode, time);
		if(childNode->turn == root_turn){
			wins = time - wins;
		}
		//back propagation
		back_propagation(childNode, wins, time);
	}
}


Node* init_root(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]){
//	cout << "init start" << endl;
	Node *root = newNode();
	copyBoard(Board, root->Board);
//	cout << "copy board success" << endl;
	copyGameRecord(GameRecord, root->GameRecord, game_length);
//	cout << "init success" << endl;
	root->game_length = game_length;
	root->turn = turn;
	return root;
}

void print_pv(Node* root){
//	cout << "start print pv" << endl;

	//selectChild's parent is root, win rate should be calculated by root's turn
	ofstream pv_file;
	pv_file.open("PV_path.txt");

	int rootTurn = root->turn;
	Node *parentNode = root;
	int childNum = parentNode->num_legal_moves;
	while(childNum != 0){
		Node *nextChild;
		double maxScore = -1;
//		double minScore = 10000;
		for(int i=0; i<childNum; i++){
			Node *temp = parentNode->child[i];
            if(temp->prunned){
                continue;
            }
            if(parentNode->turn == rootTurn){
                temp->ucb_score = calUcb(temp->win, temp->sim_time, parentNode-> sim_time);
            }else{
                temp->ucb_score = calUcb(temp->sim_time - temp->win, temp->sim_time, parentNode-> sim_time);
            }
			double ucb = temp->ucb_score;
            if(maxScore < ucb){
                maxScore = ucb;
                nextChild = temp;
            }

		}

        if(parentNode == nextChild){
            break;
        }

        double winRate;
        winRate = nextChild->win/nextChild->sim_time;
		pv_file << ((nextChild->turn!=BLACK)?"b ":"w ");
		pv_file << string(1, LabelX[(nextChild->move/10)]);
		pv_file << nextChild->move%10;
		pv_file << ", win rate = ";
		pv_file << winRate;
        pv_file << ", ucb score = ";
        pv_file << maxScore;
		pv_file << ", sim = ";
		pv_file << nextChild->sim_time;
		pv_file << "\n";

		parentNode = nextChild;
		childNum = parentNode->num_legal_moves;
	}

	pv_file << "\n";
	pv_file.close();
//	cout << "finish print pv" << endl;
}


int mcts_ucb_pick_move(Node *root, chrono::system_clock::time_point end_t){
//	int time = 0;
	end_t -= std::chrono::seconds(1);
//	time_t end_c = chrono::system_clock::to_time_t(end_t);
//	cout << ctime(&end_c) << endl;
    bool endFlag[2] = {0, 0};

    //		cout << "new tracing tree" << endl;
    Node *selectNode = select(root, root->turn, endFlag);
//		cout << "select success" << endl;
    if(endFlag[0] != 1){
        expand(selectNode);
        sim_all_children(selectNode, root->turn, FIRST_TIME_SIM, end_t);
    }


	while(chrono::duration_cast<chrono::microseconds>(end_t - chrono::system_clock::now()).count()>0){
        if(endFlag[0] == 1){
            break;
        }
//		cout << chrono::duration_cast<chrono::seconds>(end_t - chrono::system_clock::now()).count() << endl;
//		cout << "new tracing tree" << endl;
		Node *selectNode = select(root, root->turn, endFlag);
//		cout << "select success" << endl;
		expand(selectNode);
//		cout << "expand success" << endl;
		sim_all_children(selectNode, root->turn, SECOND_TIME_SIM, end_t);
//		cout << "sim success" << endl;
//		time++;
	}

	double maxScore = -1;
	int move = 0;
//	Node *nextNode;
	for(int i=0; i<root->num_legal_moves; i++){
		Node *selectChild = root->child[i];
        if(selectChild->sim_time < 100 || selectChild->prunned){
            continue;
        }
		double winRate = selectChild->win/selectChild->sim_time;
		if(maxScore < winRate){
			maxScore = winRate;
			move = selectChild->move;
//			nextNode = selectChild;
		}
	}
//	cout << "max win rate = " << maxScore << endl;
	print_pv(root);

    delete_tree(root);
	return move;
}


/* 
 * This function randomly generate one legal move (x, y) with return value x*10+y,
 * if there is no legal move the function will return 0.
 * */
int genmove(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int turn, int time_limit, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]) {
//    clock_t start_t, end_t, now_t;
	chrono::system_clock::time_point start_t, end_t;
    // record start time
    start_t = chrono::system_clock::now();
	chrono::seconds timeLimit(time_limit);
    // calculate the time bound
    end_t = start_t + timeLimit;
//	time_t start_c = chrono::system_clock::to_time_t(start_t);
//	cout << ctime(&start_c) << endl;
//	time_t end_c = chrono::system_clock::to_time_t(end_t);
//	cout << ctime(&end_c) << endl;

//    int MoveList[HISTORYLENGTH];
//    int num_legal_moves = 0;
    int return_move = 0;
//
//    num_legal_moves = gen_legal_move(Board, turn, game_length, GameRecord, MoveList);
//
//    return_move = rand_pick_move(num_legal_moves, MoveList);
//	cout << "gen start" << endl;

	Node *root = init_root(Board, turn, game_length, GameRecord);

//	cout << root->turn << endl;
//	cout << "pick move start" << endl;

	return_move = mcts_ucb_pick_move(root, end_t);

    //heuristic
    if(game_length == 0){
        return_move = 55;
//        cout << "by heuristic" << endl;
    }
    if(game_length == 1){
        if(Board[5][5] != EMPTY){
            return_move = 33;
        }
//        cout << "by heuristic" << endl;
    }

//	cout << "pick move success" << endl;

    do_move(Board, turn, return_move);

    return return_move % 100;
}

/* 
 * Following are commands for Go Text Protocol (GTP)
 *
 * */
const char *KnownCommands[]={
    "protocol_version",
    "name",
    "version",
    "known_command",
    "list_commands",
    "quit",
    "boardsize",
    "clear_board",
    "komi",
    "play",
    "genmove",
    "undo",
    "quit",
    "showboard",
    "final_score"
};

void gtp_final_score(int Board[BOUNDARYSIZE][BOUNDARYSIZE]) {
    double result;
    result = final_score(Board);
    result -= _komi;
    cout << "= ";
    if (result > 0.0) { // Black win
	cout << "B+" << result << endl << endl<< endl;;
    }
    if (result < 0.0) { // White win
	cout << "W+" << -result << endl << endl<< endl;;
    }
    else { // draw
	cout << "0" << endl << endl<< endl;;
    }
}
void gtp_undo(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]) {
    if (game_length!=0) {
	for (int i = 1; i <= BOARDSIZE; ++i) {
	    for (int j = 1; j <= BOARDSIZE; ++j) {
		Board[i][j] = GameRecord[game_length][i][j];
	    }
	}
    }
    cout << "= " << endl << endl;
}
void gtp_showboard(int Board[BOUNDARYSIZE][BOUNDARYSIZE]) {
    for (int i = 1; i <=BOARDSIZE; ++i) {
	cout << "#";
	cout <<10-i;
	for (int j = 1; j <=BOARDSIZE; ++j) {
	    switch(Board[i][j]) {
		case EMPTY: cout << " .";break;
		case BLACK: cout << " X";break;
		case WHITE: cout << " O";break;
	    }
	}
	cout << endl;
    }
    cout << "#  ";
    for (int i = 1; i <=BOARDSIZE; ++i) 
	cout << LabelX[i] <<" ";
    cout << endl;
    cout << endl;

}
void gtp_protocol_version() {
    cout <<"= 2"<<endl<< endl;
}
void gtp_name() {
    cout <<"= TCG-R05922107" << endl<< endl;
}
void gtp_version() {
    cout << "= 1.02" << endl << endl;
}
void gtp_list_commands(){
    cout <<"= ";
    for (int i = 0 ; i < NUMGTPCOMMANDS; ++i) {
	cout <<KnownCommands[i] << endl;
    }
    cout << endl;
}
void gtp_known_command(const char Input[]) {
    for (int i = 0 ; i < NUMGTPCOMMANDS; ++i) {
	if (strcmp(Input, KnownCommands[i])==0) {
	    cout << "= true" << endl<< endl;
	    return;
	}
    }
    cout << "= false" << endl<< endl;
}
void gtp_boardsize(int size) {
    if (size!=9) {
	cout << "? unacceptable size" << endl<< endl;
    }
    else {
	_board_size = size;
	cout << "= "<<endl<<endl;
    }
}
void gtp_clear_board(int Board[BOUNDARYSIZE][BOUNDARYSIZE], int NumCapture[]) {
    reset(Board);
    NumCapture[BLACK] = NumCapture[WHITE] = 0;
    cout << "= "<<endl<<endl;
}
void gtp_komi(double komi) {
    _komi = komi;
    cout << "= "<<endl<<endl;
}
void gtp_play(char Color[], char Move[], int Board[BOUNDARYSIZE][BOUNDARYSIZE], int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]) {
    int turn, move_i, move_j;
    if (Color[0] =='b' || Color[0] == 'B')
	turn = BLACK;
    else
	turn = WHITE;
    if (strcmp(Move, "PASS") == 0 || strcmp(Move, "pass")==0) {
	record(Board, GameRecord, game_length+1);
    }
    else {
	// [ABCDEFGHJ][1-9], there is no I in the index.
	Move[0] = toupper(Move[0]);
	move_j = Move[0]-'A'+1;
	if (move_j == 10) move_j = 9;
	move_i = 10-(Move[1]-'0');
	update_board(Board, move_i, move_j, turn);
	record(Board, GameRecord, game_length+1);
    }
    cout << "= "<<endl<<endl;
}
void gtp_genmove(int Board[BOUNDARYSIZE][BOUNDARYSIZE], char Color[], int time_limit, int game_length, int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]){
    int turn = (Color[0]=='b'||Color[0]=='B')?BLACK:WHITE;
    int move = genmove(Board, turn, time_limit, game_length, GameRecord);
    int move_i, move_j;
    record(Board, GameRecord, game_length+1);
    if (move==0) {
	cout << "= PASS" << endl<< endl<< endl;
    }
    else {
	move_i = (move%100)/10;
	move_j = (move%10);
//	cerr << "#turn("<<game_length<<"): (move, move_i,move_j)" << turn << ": " << move<< " " << move_i << " " << move_j << endl;
	cout << "= " << LabelX[move_j]<<10-move_i<<endl<< endl;
    }
}
/*
 * This main function is used of the gtp protocol
 * */
void gtp_main(int display) {
    char Input[COMMANDLENGTH]="";
    char Command[COMMANDLENGTH]="";
    char Parameter[COMMANDLENGTH]="";
    char Move[4]="";
    char Color[6]="";
    int ivalue;
    double dvalue;
    int Board[BOUNDARYSIZE][BOUNDARYSIZE]={{0}};
    int NumCapture[3]={0};// 1:Black, 2: White
    int time_limit = DEFAULTTIME;
    int GameRecord[MAXGAMELENGTH][BOUNDARYSIZE][BOUNDARYSIZE]={{{0}}};
    int game_length = 0;
    if (display==1) {
	gtp_list_commands();
	gtp_showboard(Board);
    }
    while (gets(Input) != 0) {
	sscanf(Input, "%s", Command);
	if (Command[0]== '#')
	    continue;

	if (strcmp(Command, "protocol_version")==0) {
	    gtp_protocol_version();
	}
	else if (strcmp(Command, "name")==0) {
	    gtp_name();
	}
	else if (strcmp(Command, "version")==0) {
	    gtp_version();
	}
	else if (strcmp(Command, "list_commands")==0) {
	    gtp_list_commands();
	}
	else if (strcmp(Command, "known_command")==0) {
	    sscanf(Input, "known_command %s", Parameter);
	    gtp_known_command(Parameter);
	}
	else if (strcmp(Command, "boardsize")==0) {
	    sscanf(Input, "boardsize %d", &ivalue);
	    gtp_boardsize(ivalue);
	}
	else if (strcmp(Command, "clear_board")==0) {
	    gtp_clear_board(Board, NumCapture);
	    game_length = 0;
	}
	else if (strcmp(Command, "komi")==0) {
	    sscanf(Input, "komi %lf", &dvalue);
	    gtp_komi(dvalue);
	}
	else if (strcmp(Command, "play")==0) {
	    sscanf(Input, "play %s %s", Color, Move);
	    gtp_play(Color, Move, Board, game_length, GameRecord);
	    game_length++;
	    if (display==1) {
		gtp_showboard(Board);
	    }
	}
	else if (strcmp(Command, "genmove")==0) {
	    sscanf(Input, "genmove %s", Color);
	    gtp_genmove(Board, Color, time_limit, game_length, GameRecord);
	    game_length++;
	    if (display==1) {
		gtp_showboard(Board);
	    }
	}
	else if (strcmp(Command, "quit")==0) {
	    break;
	}
	else if (strcmp(Command, "showboard")==0) {
	    gtp_showboard(Board);
	}
	else if (strcmp(Command, "undo")==0) {
	    game_length--;
	    gtp_undo(Board, game_length, GameRecord);
	    if (display==1) {
		gtp_showboard(Board);
	    }
	}
	else if (strcmp(Command, "final_score")==0) {
	    if (display==1) {
		gtp_showboard(Board);
	    }
	    gtp_final_score(Board);
	}
    }
}



int main(int argc, char* argv[]) {
//    int type = GTPVERSION;// 1: local version, 2: gtp version
//    cout << calSD(204, 850) << endl;
    int type = GTPVERSION;// 1: local version, 2: gtp version
    int display = 0; // 1: display, 2 nodisplay
    if (argc > 1) {
	if (strcmp(argv[1], "-display")==0) {
	    display = 1;
	}
	if (strcmp(argv[1], "-nodisplay")==0) {
	    display = 0;
	}
    }
    gtp_main(display);
    return 0;
}
