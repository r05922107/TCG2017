#ifndef __bitboard__
#define __bitboard__

#endif

#include <cctype>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <list>
#include <bitset>
#include <cstdlib>
#include <unistd.h>
using namespace std;

#ifndef ANQI
#include"anqi.hh"
#endif



#ifndef __bitboard_hh__
#include "bitboard.hh"
#endif

int MAXINT = 1000000;
int WIN = 999990;

#ifndef ONE
#define ONE
typedef unsigned int u32;
u32 One = 0x00000001;
#endif



int INIT_COUNT[14] = {1,2,2,2,2,2,5,1,2,2,2,2,2,5};

int static_val[16] = {
        11000, 4900, 2400, 1100, 500, 3000, 50,
        11000, 4900, 2400, 1100, 500, 3000, 50,
        0, 0
};

int dyn_val[16] = {
        11000, 4900, 2400, 1100, 500, 3000, 50,
        11000, 4900, 2400, 1100, 500, 3000, 50,
        0, 0
};

int K_enemy_val[6] = {11000, 6000, 4000, 3000, 2300, 2000};
int enemy_val[9] = {11000, 4900, 2400, 1100, 500, 240, 110, 50, 20};


typedef struct bitboard{
    /*
    0  'K' 帥  ,     7  'k' 將  ,     14  'X' 未翻  ,
    1  'G' 仕  ,     8  'g' 士  ,     15  '-' 空格  ,
    2  'M' 相  ,     9  'm' 象  ,
    3  'R' 硨  ,    10  'r' 車  ,
    4  'N' 傌  ,    11  'n' 馬  ,
    5  'C' 炮  ,    12  'c' 砲  ,
    6  'P' 兵  ,    13  'p' 卒 
    */

    u32 piece[16];                                
        
    u32 red, black, occupied;  

    int dcount[14];

    CLR who; 
        // // (color)
        //  0 = 紅方 (大寫字母)
        //  1 = 黑方 (小寫字母)
        // -1 = 都不是 

    bool end_game_mode;
    bitset<128> hashval;


}Bitboard;


#ifndef __hashtable__
#include "hashtable.h"
#endif


class BitboardController{
public:

    
    void init_board(struct bitboard &quickboard){
        for(int i = 0; i < 16; i++){
            quickboard.piece[i] = 0;
        }

        for(int i = 0; i < 14; i++){
            quickboard.dcount[i] = INIT_COUNT[i];
        }
        quickboard.red = 0;
        quickboard.black = 0;
        quickboard.occupied = 0;
        quickboard.who = -1;
        quickboard.end_game_mode = false;

    }

    void generate_board(struct bitboard &quickboard, BOARD &slowboard){

        // who
        quickboard.who = slowboard.who;

        // piece
        for(int i = 0; i <32; i++){
            quickboard.piece[slowboard.fin[i]] |= One << i;
        }

        // dcount, not flipped count
        for(int i = 0; i<14; i++){
            quickboard.dcount[i] = slowboard.cnt[i];
        }

        // red board & black board
        for(int i = 0; i<7; i++){
            quickboard.red |= quickboard.piece[i];
            quickboard.black |= quickboard.piece[i+7];
        }

        // occupied board
        quickboard.occupied |= quickboard.red;
        quickboard.occupied |= quickboard.black;
        quickboard.occupied |= quickboard.piece[14];

        quickboard.hashval = HT.getHashVal(quickboard);

    }

    struct bitboard copy(struct bitboard &quickboard){
        Bitboard new_bitboard = quickboard;
        return new_bitboard;
    }

    // return number of genmoves
    int get_valid_moves(struct bitboard &quickboard, struct MOVLST &lst, bool justeat = false){
        lst.num = 0; // Init the lst
        for(int i = 0; i <= 6; i++){
            int i_plus = 0;
            int p_minus = 0;
            u32 op = quickboard.black;

            if(quickboard.who == 0){ // red turn
                i_plus = 0;
                p_minus = 0;
                op = quickboard.black;
            }
            else{ // black turn
                i_plus = 7;
                p_minus = 7;
                op = quickboard.red;
            }

            u32 p = quickboard.piece[i + i_plus];
            while(p){
                u32 mask = LS1B(p);
                u32 dest = 0;
                p ^= mask;
                int src = GetIndex(mask); 
                if(i == 0){  // K
                    dest=pMoves[src] & (op ^ quickboard.piece[13-p_minus]);
                }
                else if(i == 1){ // G 
                    dest=pMoves[src] & (op ^ quickboard.piece[7-p_minus]);
                }
                else if(i == 2){ // M
                    dest=pMoves[src] & (op ^ quickboard.piece[7-p_minus] ^ quickboard.piece[8-p_minus]);
                }
                else if(i == 3){ // R
                    dest=pMoves[src] & (quickboard.piece[10-p_minus] | quickboard.piece[11-p_minus] | quickboard.piece[12-p_minus] | quickboard.piece[13-p_minus]);
                }
                else if(i == 4){ // N
                    dest=pMoves[src] & (quickboard.piece[11-p_minus] | quickboard.piece[12-p_minus] | quickboard.piece[13-p_minus]);
                }
                else if(i == 5){ // C  
                    // cout << "here\n";
                    dest=CGEN(quickboard, src) & op;
                }
                else if(i == 6){ // P
                    dest= pMoves[src] & (quickboard.piece[7-p_minus] | quickboard.piece[13-p_minus]);
                }
                else{
                    dest = 0;
                }

                if(justeat == false){// Space to go
                    dest |= pMoves[src] & quickboard.piece[15];
                }

                while(dest){
                    u32 mask2 = LS1B(dest);
                    dest ^= mask2;
                    u32 result = GetIndex(mask2);
                    MOV mov(src, result);
                    mov.st_fin = FIN(i + i_plus);
                    lst.mov[lst.num++] = mov;
                }
            }
        }

        return lst.num;
    }

    bool check_eat_move(struct bitboard &quickboard, MOV mov){
        if(mov.st == mov.ed){
            return false;
        }
        if ((One << mov.ed) & quickboard.occupied){
            return true;
        }
        return false;
    }

    void get_reveal_moves(struct bitboard &quickboard, struct MOVLST &lst){
        lst.num = 0; // Init the lst
        u32 toflipboard = quickboard.piece[14];
        while(toflipboard){
            u32 mask = LS1B(toflipboard);
            toflipboard ^= mask;
            int pos = GetIndex(mask);
            MOV mov(pos, pos);
            lst.mov[lst.num++] = mov;
        }
        return;
    }

    void get_unflip_prob(struct bitboard &quickboard, vector<FLIP_PROB> &toflip_lst){
        double unflipCnt = getUnflipCount(quickboard);
        for(int i = 0; i < 14; i++){
            if(quickboard.dcount[i] > 0){
                FLIP_PROB f;
                f.fin = FIN(i);
                f.prob = (double)quickboard.dcount[i] / (double)unflipCnt;
                toflip_lst.push_back(f);
            }
        }
    }

    int getTotalChess(Bitboard &quickboard){
        int total = 0;
        for(int i = 0; i < 15; i++){
            total += __builtin_popcountll(quickboard.piece[i]);
        }
        return total;
    }

    int evaluate(struct bitboard &quickboard){

        // WIN or LOSE
        if(__builtin_popcountll(quickboard.red) == 0 &&
            getTotalChess(quickboard) == __builtin_popcountll(quickboard.black) ){
            if(quickboard.who == 0){
                return -WIN;
            }else{
                return WIN;
            }
            
        }
        if(__builtin_popcountll(quickboard.black) == 0 &&
            getTotalChess(quickboard) == __builtin_popcountll(quickboard.red) ){
            if(quickboard.who == 0){
                return WIN;
            }else{
                return -WIN;
            }
        }



        int VAL[2] = {0,0}; // 0: red, 1: black

        // count alive
        int Live[14] = {0};
        for(int i = 0; i < 14; i++){
            Live[i] = __builtin_popcountll(quickboard.piece[i]) + quickboard.dcount[i];
        }

        // dynamic piece value
        bool isDynFlag = false;
        if( (Live[0] > 0 && Live[13] > 0) ||
                (Live[6] > 0 && Live[7] > 0)){
            bool isDynFlag = true;

            // King
            VAL[0] += K_enemy_val[Live[13]];
            VAL[1] += K_enemy_val[Live[6]];

            if(isEndFlag){
                dyn_val[0] = K_enemy_val[Live[13]];
                dyn_val[7] = K_enemy_val[Live[6]];
            }


            // G, M, R, N
            int Enemies[2] = {0};
            for(int i = 0; i < 5; i++){
                Enemies[0] += Live[i+7];
                Enemies[1] += Live[i];
                if(i < 4){
                    VAL[0] += enemy_val[Enemies[0]] * Live[i+1];
                    VAL[1] += enemy_val[Enemies[1]] * Live[i+8];

                    if(isEndFlag){
                        dyn_val[i+1] = enemy_val[Enemies[0]];
                        dyn_val[i+8] = enemy_val[Enemies[1]];
                    }
                }
            }
            // C
            VAL[0] += Live[5] * static_val[5];
            VAL[1] += Live[12] * static_val[12];
            // cout << "ii:" << VAL[0] << " " << VAL[1] << endl;
            
            //Pawn
            VAL[0] += enemy_val[Enemies[0] - Live[7]] * Live[6];
            VAL[1] += enemy_val[Enemies[1] - Live[0]] * Live[13];

            if(isEndFlag){
                dyn_val[6] = enemy_val[Enemies[0] - Live[7]];
                dyn_val[13] = enemy_val[Enemies[1] - Live[0]];
            }

            // if(DEBUG){
            //     Display(quickboard);
            //     cout << "p_v:" << enemy_val[Enemies[0] - Live[7]] << " " << enemy_val[Enemies[1] - Live[0]] << endl;
            //     cout << "VAL:" << VAL[0] << " " << VAL[1] << endl;
            //     exit(-1);
            // }

        }else{ //static value

            for(int i = 0; i < 7; i++){
                VAL[0] += Live[i] * static_val[i];
                VAL[1] += Live[i+7] * static_val[i+7];
            }

            // edge reduce score
            int idx;
            for(int i = 0; i < 14; i++){
                u32 p = quickboard.piece[i];
                while(p){
                    u32 mask = LS1B(p);
                    p ^= mask;
                    idx = GetIndex(mask);
                    if(idx/4==0 || idx/4==7){
                        if(i < 7){
                            VAL[0] -= 5;
                        }else{
                            VAL[1] -= 5;
                        }
                    }
                    if(idx%4==0 || idx%4==3){
                        if(i < 7){
                            VAL[0] -= 5;
                        }else{
                            VAL[1] -= 5;
                        }
                    }
                }
            }

        }

        // endgame
        if(isEndFlag){
            int *arr;
            if(isDynFlag){
                arr = dyn_val;
            }
            else{
                arr = static_val;
            }

            for(int i = 0; i < 7; i++){
                u32 p = quickboard.piece[i];
                while(p){
                    u32 mask = LS1B(p);
                    p ^= mask;
                    for(int j = 7; j < 14; j++){
                        u32 p2 =quickboard.piece[j];
                        while(p2){
                            u32 mask2 = LS1B(p2);
                            p2 ^= mask2;
                            int d = getDist(mask, mask2);
                            if( i!= 5 && ChkEats(FIN(i), FIN(j))){
                                VAL[0] += (10 - d) * arr[j] / 50;
                            }
                            if( j!= 5 && ChkEats(FIN(j), FIN(i))){
                                VAL[1] += (10 - d) * arr[i] / 50;
                            }
                        }
                    }
                }  
            }

            // edge reduce score
            int idx;
            for(int i = 0; i < 14; i++){
                u32 p = quickboard.piece[i];
                while(p){
                    u32 mask = LS1B(p);
                    p ^= mask;
                    idx = GetIndex(mask);
                    if(idx/4==0 || idx/4==7){
                        if(i < 7){
                            VAL[0] -= 20;
                        }else{
                            VAL[1] -= 20;
                        }
                    }
                    if(idx%4==0 || idx%4==3){
                        if(i < 7){
                            VAL[0] -= 20;
                        }else{
                            VAL[1] -= 20;
                        }
                    }
                }
            }

        }



        // return
        if(quickboard.who == 0){
            return VAL[0] - VAL[1];
        }else{
            return VAL[1] - VAL[0];
        }
    }

    int getDist(u32 &a, u32 &b){
        int aa = GetIndex(a);
        int bb = GetIndex(b);
        return abs( (aa/4) - (bb/4) ) + abs( (aa%4) - (bb%4) );
    }

    void reveal(struct bitboard &quickboard, MOV mov, FIN fin){
        int pos = mov.st;

        u32 upos = One << pos;
        quickboard.piece[fin] |= upos;
        quickboard.hashval = HT.exclusivePiece(quickboard.hashval, fin, pos);
        quickboard.piece[14] &= ~upos;
        quickboard.hashval = HT.exclusivePiece(quickboard.hashval, 14, pos);

        if(fin < 7){ // red 
            quickboard.red |= upos;
        }else{
            quickboard.black |= upos;
        }
        quickboard.dcount[fin] --;
        quickboard.who = 1 - quickboard.who;
        quickboard.hashval = HT.exclusiveWho(quickboard.hashval);
        return;
    }

    void update(struct bitboard &quickboard, MOV mov){
        
        if(mov.st == -1){ // null step
            quickboard.who = 1 - quickboard.who;
            quickboard.hashval = HT.exclusiveWho(quickboard.hashval);
            return;
        }

        // take the st fin
        quickboard.piece[mov.st_fin] &= ~(One << mov.st);
        quickboard.piece[15] |= One << mov.st;
        quickboard.occupied &= ~(One << mov.st);
        quickboard.red &= ~(One << mov.st);
        quickboard.black &= ~(One << mov.st);
        quickboard.hashval = HT.exclusivePiece(quickboard.hashval, mov.st_fin, mov.st);

        // take the ed fin
        if(quickboard.occupied & (One << mov.ed)){ // if eat
            int i_plus;
            if(quickboard.who == 0){ // red eat black
                i_plus = 7;
                quickboard.black &= ~(One << mov.ed);
            }
            else{ //else black eat red
                i_plus = 0;
                quickboard.red &= ~(One << mov.ed);
            }
            for(int i = 0; i < 7; i++){
                if(quickboard.piece[i+i_plus] & (One << mov.ed)){
                    quickboard.piece[i+i_plus] &= ~(One << mov.ed);
                    quickboard.hashval = HT.exclusivePiece(quickboard.hashval, i+i_plus, mov.ed);
                    break;
                }
            }
        } 

        // put fin on the ed
        quickboard.piece[mov.st_fin] |= One << mov.ed;
        quickboard.hashval = HT.exclusivePiece(quickboard.hashval, mov.st_fin, mov.ed);
        quickboard.occupied |= One << mov.ed;
        quickboard.piece[15] &= ~(One << mov.ed);
        if (quickboard.who == 0){
            quickboard.red |= One << mov.ed;
        }
        else{
            quickboard.black |= One << mov.ed;
        }

        // change turn
        quickboard.who = 1 - quickboard.who;
        quickboard.hashval = HT.exclusiveWho(quickboard.hashval);

        return;
    }


    // void flip(struct bitboard &quickboard, u32 upos, FIN fin=FIN_X){ // 翻子
    //     quickboard.piece[fin] |= upos;
    //     if(fin < 7){ // red
    //         quickboard.red |= upos;
    //     }else{
    //         quickboard.black |= upos;
    //     }
    //     quickboard.dcount[fin] --;
    //     quickboard.who = 1 - quickboard.who;
    // };

    void print_string(u32 x){
        cout << std::bitset<32>(x) << endl;
    }

    void Display(struct bitboard &quickboard){

        BOARD tmpboard;
        tmpboard.who = quickboard.who;
        for(int i = 0; i < 14; i++){
            tmpboard.cnt[i] = quickboard.dcount[i];
        }
        for(int i = 0; i < 16; i++){
            u32 p = quickboard.piece[i];
            cout << "i:" << i << " ";
            print_string(p);
            while(p){
                u32 mask = LS1B(p);
                p ^= mask;
                // cout << "GetIndex(mask):" << GetIndex(mask) << " FIN(i):" << FIN(i) << endl;;
                tmpboard.fin[GetIndex(mask)] = FIN(i);
            }   
        }
        sleep(1);
        tmpboard.Display();
        sleep(1);

        // FIN fin[32]; // 各個位置上面擺了啥
        // int cnt[14]; // 各種棋子的未翻開數量
    }


    // void isEndGame(struct bitboard &quickboard){
    //     int dark_count = getUnflipCount(quickboard);
    //     if(dark_count==0 || board.totalDark+board.totalBright<12) {
    //         quickboard.end_game_mode=true;
    //         printf("END GAME START\n");
    //     }
    // }


    int getUnflipCount(struct bitboard &quickboard){
        return __builtin_popcountll(quickboard.occupied & (~quickboard.red) & (~quickboard.black));
    }

    /******** IMPLEMENTATION ***********/
    // Implement CGEN
     
    u32 CGEN(struct bitboard &quickboard, int src){
        u32 resultR = 0;
        u32 resultC = 0;


        int r = src / 4;
        int c = src % 4;

        // cout << "src:" << src << " r:" << r << " c:" << c << endl;

        // ROW
        u32 x = ((rank[r] & quickboard.occupied) ^ (One << src )) >> (4 * r);
        
        // cout << "ROW X:";
        // print_string(x);

        if(c == 0){ // leftest
            resultR |= CGEN_R0(x);
        }
        else if (c == 1){ // leftest 2nd
            resultR |= CGEN_R1(x);
        }
        else if(c == 2){ // left
            resultR |= CGEN_R2(x);
        }
        else if(c == 3){
            resultR |= CGEN_R3(x);
        }
        else{
            fprintf(stderr, "c is:%d, sth. wrong", c);
            exit(-1);
        }

        
        // COL
        x = ((file[c] & quickboard.occupied) ^ (One << src )) >> c;

        // cout << "COL X:";
        // print_string(x);

        if(r == 0){
            resultC |= CGEN_C0(x);    
        }
        else if(r == 1){
            resultC |= CGEN_C1(x);    
        }
        else if(r == 2){
            resultC |= CGEN_C2(x);    
        }
        else if(r == 3){
            resultC |= CGEN_C3(x);    
        }
        else if(r == 4){
            resultC |= CGEN_C4(x);    
        }
        else if(r == 5){
            resultC |= CGEN_C5(x);    
        }
        else if(r == 6){
            resultC |= CGEN_C6(x);    
        }
        else if(r == 7){
            resultC |= CGEN_C7(x);    
        }       

        // cout << "resultC:";
        // print_string(resultC);

        return (resultR << (4 * r)) | (resultC << c);

    }


    /* CGEN_R */
    u32 CGEN_R0(u32 x){
        if(x){
            u32 mask = LS1B(x);
            return (x ^ mask)? LS1B(x ^ mask) : 0;
        }
        return 0;
    }

    u32 CGEN_R1(u32 x){
        return ((x & 12)==12)? 8 : 0;
    }

    u32 CGEN_R2(u32 x){
        return ((x & 3) == 3) ? 1 : 0;
    }

    u32 CGEN_R3(u32 x){
        if(x){
            u32 mask = MS1B(x);
            return (x ^ mask)? MS1B(x ^ mask) : 0;
        }
        return 0;
    }


    /* CGEN_C */
    u32 CGEN_C0(u32 x){
        return CGENCL(x);
    }
    u32 CGEN_C1(u32 x){
        return CGENCL(x&0x11111100);
    }
    u32 CGEN_C2(u32 x){
        u32 part = 0;
        if((x&0x00000011)==0x00000011){
            part |= 1;
        }
        return part | CGENCL(x&0x11111000);
    }
    u32 CGEN_C3(u32 x){
        u32 part = 0;
        part = CGENCR(x&0x00000111);
        return part | CGENCL(x&0x11110000);
    }
    u32 CGEN_C4(u32 x){
        u32 part = 0;
        part = CGENCR(x&0x00001111);
        return part | CGENCL(x&0x11100000);
    }
    u32 CGEN_C5(u32 x){
        u32 part = 0;
        part = CGENCR(x&0x00011111);
        return part | CGENCL(x&0x11000000);
    }
    u32 CGEN_C6(u32 x){
        return CGENCR(x&0x00111111);
    }
    u32 CGEN_C7(u32 x){
        return CGENCR(x);
    }


    u32 CGENCL(u32 x){
        if(x){
            u32 mask = LS1B(x);
            return (x ^ mask)? LS1B(x ^ mask) : 0;
        }
        return 0;
    }

    u32 CGENCR(u32 x){
        if(x){
            u32 mask = MS1B(x);
            return (x ^ mask)? MS1B(x ^ mask) : 0;
        }
        return 0;
    }

};
