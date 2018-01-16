#include "HashTable.h"

using namespace std;

HashTable::HashTable(){
    //initialize
    for(int i=0; i<TABLE_SIZE; i++){
        hashTable[i] = Entry();
    }

    for(int i=0; i<15; i++){
        for(int j=0; j<32; j++){
            hashKey[i][j] = (bitset<128>(rand()) << 96) ^ (bitset<128>(rand()) << 64) ^ (bitset<128>(rand()) << 32) ^ bitset<128>(rand());
        }
    }

    for(int i=0; i<2; i++){
        whoKey[i] = (bitset<128>(rand()) << 96) ^ (bitset<128>(rand()) << 64) ^ (bitset<128>(rand()) << 32) ^ bitset<128>(rand());
    }
}

int HashTable::modifyTableSize(bitset<128> &bit){
    int mask = 1;
    int result = 0;
    for(size_t i=0; i<HASH_N; i++){
        if(bit.test(i)){
            result |= mask;
            mask <<= 1;
        }
    }
    return result % TABLE_SIZE;
}

bool HashTable::update(BOARD &board, int value, int depth, bool exact){
    int index = modifyTableSize(board.hashValue);

    Entry *origin_entry = &hashTable[index];

    //no need to update
    if(origin_entry->valid){
        if(origin_entry->hashValue == board.hashValue && origin_entry->who == board.who){
            if(origin_entry->depth > depth ||
               (origin_entry->depth == depth && origin_entry->exact)){
                return false;
            }
        }
    }

    origin_entry->value = value;
    origin_entry->depth = depth;
    origin_entry->who = board.who;
    origin_entry->valid = true;
    origin_entry->exact = exact;
    origin_entry->hashValue = board.hashValue;
    return true;
}

Entry *HashTable::get(BOARD &board){
    int index = modifyTableSize(board.hashValue);
    Entry* origin_Entry = &(hashTable[index]);
    if(origin_Entry->valid && origin_Entry->who == board.who &&
       origin_Entry->hashValue == board.hashValue){
        return origin_Entry;
    }
    return &missEntry;
}

//color[next] XOR s[q1][l1] XOR ... XOR s[qx][lx]
bitset<128> HashTable::getHashValue(BOARD &board){
    bitset<128> hashValue = 0;

    for(int i=0; i<32; i++){
        if(board.fin[i] != FIN_E){
            hashValue ^= hashKey[board.fin[i]][i];
        }
    }
    hashValue ^= whoKey[board.who];
    return hashValue;
}

bitset<128> HashTable::pieceXOR(bitset<128> &originValue, int fin, int pos){
    return originValue ^ hashKey[fin][pos];
}

bitset<128> HashTable::whoXOR(bitset<128> &originValue){
    return originValue ^ whoKey[0] ^ whoKey[1];
}

HashTable HT;