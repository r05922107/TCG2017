#ifndef __hashtable__
#define __hashtable__
#endif

#ifndef ANQI
#include"anqi.hh"
#endif

#include <cstdlib>
#include <iostream>
#include <cstdio>
//#include <bitset>

using namespace std;

#define TABLE_SIZE (1<<HASH_N)
const int HASH_N = 16;


struct Entry{
    int value;
    int depth;
    int who;
    bool valid;
    bool exact;
    bitset<1024> hashValue;
    Entry(){
        valid = false;
    }

};

class HashTable {
public:
    bitset<1024> hashKey[15][32];  //15:bright and dark piece  32:position
    bitset<1024> whoKey[2];

    Entry hashTable[TABLE_SIZE];
    Entry missEntry;

    HashTable();

    int modifyTableSize(bitset<1024> &bit);
    bool update(BOARD &board, int value, int depth, bool exact);
    Entry *get(BOARD &board);
    bitset<1024> getHashValue(BOARD &board);
    bitset<1024> pieceXOR(bitset<1024> &originValue, int fin, int pos);
    bitset<1024> whoXOR(bitset<1024> &originValue);

};
extern HashTable HT;