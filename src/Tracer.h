#ifndef TRACER_H
#define TRACER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "ExpandRand/sha3.h"

class Tracer {
private:
	Tracer() {
		smallBuf=(uint64_t*)malloc(smallBufSize);
		bigBuf=(uint64_t*)malloc(bigBufSize);
		clear();
	}

public:
	static Tracer* I() {
		thread_local static Tracer Inst;
		return &Inst;
	}
	// small Buf will be read and written
	uint64_t* smallBuf;
	const int smallBufSize=128*1024; //128KB
	const int smallBufMask=(smallBufSize/8-1);
	// big Buf will only be written during program execution
	uint64_t* bigBuf;
	const int bigBufSize=32*1024*1024; //32MB
	const int bigBufMask=(bigBufSize/8-1)>>3;
	const uint64_t FNV_PRIME=0x100000001b3ULL;
	const uint64_t FNV_INIT=0xcbf29ce484222325ULL;
	int counter;
	uint64_t fnvKey;
	uint64_t fnvHistory[8];
	uint32_t historyCksum() {
		uint8_t res=0;
		uint8_t* ptr=(uint8_t*)this->fnvHistory;
		for(int i=0; i<64; i++) {
			res+=*ptr;
		}
		return res;
	}
	sha3_ctx ctx;
	void sha3_update(const unsigned char* msg, size_t size) {
		rhash_sha3_update(&ctx, msg, size);
	}
	void sha3_final(unsigned char* result) {
		rhash_sha3_final(&ctx, result);
	}
	void final_result(unsigned char* result) {
		uint64_t temp[32];
		for(int i=0; i<32; i++) temp[i]=FNV_INIT;
		for(int j=0; j<bigBufSize/8; j+=32) {
			for(int i=0; i<32; i++) {
				temp[i]*=FNV_PRIME;
				temp[i]^=bigBuf[j+i];
			}
		}
		sha3_update((unsigned char*)temp, 8*32);
		sha3_final(result);
	}
	void clear() {
		rhash_sha3_256_init(&ctx);
		memset((char*)smallBuf,0,smallBufSize);
		memset((char*)bigBuf,0,bigBufSize);
		fnvKey=FNV_INIT;
		counter=0;
		for(int i=0; i<8; i++) fnvHistory[i]=0;
	}
	~Tracer() {
		free(smallBuf);
		free(bigBuf);
	}
	// 128bit multiplication
	void extmul(uint64_t& a, uint64_t& b) {
		__int128 result = (__int128)a * (__int128)b;
		a = result >> 64;
		b = (int64_t)result;
	}
	void meet(uint64_t value) {
		uint64_t oldKey=fnvKey;
		fnvKey*=FNV_PRIME;
		fnvKey^=value;
		counter++;
		// when normal fnv runs for 32 times, update smallBuf and 
		// append new entry to fnvHistory
		if(counter%32==0) {
			int idx=oldKey&smallBufMask;
			assert(idx*8<smallBufSize);
			if(smallBuf[idx]==0) {
				smallBuf[idx]=fnvKey;
			}
			else {
				extmul(smallBuf[idx], fnvKey);
				fnvKey^=oldKey;
			}
			fnvHistory[counter/32-1]=fnvKey;
		}
		// when fnvHistory has 8 entries, flush it out to bigBuf
		if(counter==256) {
			int base=8*(oldKey&bigBufMask);
			counter=0;
			for(int i=0; i<8; i++) bigBuf[base+i]=fnvHistory[i]; 
		}
	}
};

#endif
