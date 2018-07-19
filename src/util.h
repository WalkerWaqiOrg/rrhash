#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void expandRand4KB(uint8_t* in, int inSize, uint8_t* out);

inline uint64_t mulxor(uint64_t a, uint64_t b) {
	unsigned __int128 result = (unsigned __int128)a * (unsigned __int128)b;
	a = result >> 64;
	b = (int64_t)result;
	return a^b;
}


#endif
