#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void expandRand4KB(uint8_t* in, int inSize, uint8_t* out);

// inline uint64_t mulxor(uint64_t a, uint64_t b) {
// 	unsigned __int128 result = (unsigned __int128)a * (unsigned __int128)b;
// 	a = result >> 64;
// 	b = (int64_t)result;
// 	return a^b;
// }


inline uint64_t mulxor(uint64_t a, uint64_t b) {
	uint64_t x1,x2,y1,y2;
	x1=a; y1=b;
	x2=(a<<32)|(b>>32);
	y2=(b<<32)|(a>>32);
	a = x1*y1;
	b = x2*y2;
	return a^b;
}

#endif