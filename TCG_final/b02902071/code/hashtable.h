#ifndef __hashtable__
#define __hashtable__
#endif

// #ifndef __bitboard_hh__
// #include "bitboard.hh"
// #endif

#ifndef ONE
#define ONE
typedef unsigned int u32;
u32 One = 0x00000001;
#endif

#ifndef ANQI
#include"anqi.hh"
#endif


#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <bitset>


#define TABLE_SIZE (1<<HASH_N)
const int HASH_N = 16;




struct Entry{
	int value;
	int depth;
	int who;
	bool valid;
	bool exact;
	bitset<128> hashval;
	Entry(){
		valid=false;
	};
};



class HashTable{
public:
	bitset<128> hashKey[15][32];  // 15: except space piece,  32: position
	bitset<128> whoKey[2];
	Entry hash_table[TABLE_SIZE];
	Entry MissEntry;

	HashTable(){
		for(int i = 0; i < TABLE_SIZE; i++) {
			hash_table[i]=Entry();
		}

		for(int i=0;i<15;i++){
			for(int j=0;j<32;j++){ //128
				hashKey[i][j]=(bitset<128>(rand()) << 96) ^ (bitset<128>(rand()) << 64) ^ (bitset<128>(rand()) << 32) ^ bitset<128>(rand());
			}
		}
		for(int i = 0; i<2; i++){
			whoKey[i] = (bitset<128>(rand()) << 96) ^ (bitset<128>(rand()) << 64) ^ (bitset<128>(rand()) << 32) ^ bitset<128>(rand());
		}
	}

	bitset<128> getHashVal(Bitboard &quickboard){
		// color[next] XOR s[q1][l1] XOR · · · XOR s[qx][lx]
		bitset<128> hash_val = 0;
		// return hash_val;
		for(int i = 0; i < 15; i++){
			u32 p = quickboard.piece[i];
			while(p){
				u32 mask = LS1B(p);
				p ^= mask;
				hash_val ^= hashKey[i][GetIndex(mask)];
				// cout << bitset<32>(p) << endl;
			}
		}
		hash_val ^= whoKey[quickboard.who]; 
		return hash_val;
	}

	bitset<128> exclusivePiece(bitset<128> &ori_val, int fin, int pos){
		return ori_val ^ hashKey[fin][pos];
	}

	bitset<128> exclusiveWho(bitset<128> &ori_val){
		return ori_val ^ whoKey[0] ^ whoKey[1];
	}	


	int modTableSize(bitset<128> &bit){
		int mask = 1;
		int result = 0;
		for (size_t i = 0; i < HASH_N; ++i) {
    		if (bit.test(i))
       			result |= mask;
    			mask <<= 1;
		}
		return result % TABLE_SIZE;
	}

	// return ture if update
	bool update(Bitboard &quickboard, int val, int depth, bool exact){
		
		int index = modTableSize(quickboard.hashval);

		Entry *ori_entry= &hash_table[index];

		if(ori_entry->valid==true){
			if(ori_entry->hashval == quickboard.hashval && 
				ori_entry->who == quickboard.who){
					if(ori_entry->depth > depth || 
						(ori_entry->depth == depth && ori_entry->exact==true) ) return false;
			}
		}
		ori_entry->value=val;
		ori_entry->depth=depth;
		ori_entry->who=quickboard.who;
		ori_entry->valid=true;
		ori_entry->exact=exact;
		ori_entry->hashval=quickboard.hashval;
		return true;
	}

	// return pointer to Entry
	Entry *get(Bitboard &quickboard){
		// if(isEndFlag){
		// 	return &MissEntry;
		// }

		int index = modTableSize(quickboard.hashval);

		Entry* ori_entry = &(hash_table[index]);
		if(ori_entry->valid==true && 
				ori_entry->who==quickboard.who && 
				ori_entry->hashval == quickboard.hashval) 
			return ori_entry;
		return &MissEntry; // valid = false
	}

};

HashTable HT;

