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
    bitset<128> hashValue;
    Entry(){
        valid = false;
    }

};

class HashTable {
public:
    bitset<128> hashKey[15][32];  //15:bright and dark piece  32:position
    bitset<128> whoKey[2];

    Entry hashTable[TABLE_SIZE];
    Entry missEntry;

    HashTable();

    int modifyTableSize(bitset<128> &bit);
    bool update(BOARD &board, int value, int depth, bool exact);
    Entry *get(BOARD &board);
    bitset<128> getHashValue(BOARD &board);
    bitset<128> pieceXOR(bitset<128> &originValue, int fin, int pos);
    bitset<128> whoXOR(bitset<128> &originValue);

};
extern HashTable HT;