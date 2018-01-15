#ifndef __bitboard_hh__
#define __bitboard_hh__
#endif



#ifndef ONE
#define ONE
typedef unsigned int u32;
u32 One = 0x00000001;
#endif

u32 LS1B(u32 x){ // Least Significant 1 Bit (LS1B)
    return x & (-x);
}

u32 MS1B(u32 x){ // Most Significant 1 Bit (MS1B)
	 x |= x >> 32;
	 x |= x >> 16;
	 x |= x >> 8;
	 x |= x >> 4;
	 x |= x >> 2;
	 x |= x >> 1;
	 return (x >> 1) + 1;
}


u32 pMoves[32] = {
	0x00000012,
	0x00000025,
	0x0000004A,
	0x00000084,
	0x00000121,
	0x00000252,
	0x000004A4,
	0x00000848,

	0x00001210,
	0x00002520,
	0x00004A40,
	0x00008480,
	0x00012100,
	0x00025200,
	0x0004A400,
	0x00084800,

	0x00121000,
	0x00252000,
	0x004A4000,
	0x00848000,
	0x01210000,
	0x02520000,
	0x04A40000,
	0x08480000,

	0x12100000,
	0x25200000,
	0x4A400000,
	0x84800000,
	0x21000000,
	0x52000000,
	0xA4000000,
	0x48000000,
};


u32 file[4] = {
	0x11111111,
	0x22222222,
	0x44444444,
	0x88888888,
};

u32 rank[8] = {
	0x0000000F,
	0x000000F0,
	0x00000F00,
	0x0000F000,
	0x000F0000,
	0x00F00000,
	0x0F000000,
	0xF0000000,
};


int index32[32] = {
	31, 0, 1, 5,
	2, 16, 27, 6,
	3, 14, 17, 19,
	28, 11, 7, 21,
	30, 4, 15, 26,
	13, 18, 10, 20,
	29, 25, 12, 9,
	24, 8, 23, 22,
};

int BitsHash(u32 x){
	return (x * 0x08ED2BE6) >> 27;
}

int GetIndex(u32 mask){
	return index32[BitsHash(mask)];
}


