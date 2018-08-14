
#include <stdint.h>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "byte_order.c"
#include "sha3.c"
#include "blake2b-ref.c"

//sha3's output is 256 bits (32 bytes), blake2b consumes 128 bytes and outputs 64 bytes
// 4096/64=8*8 we need 8 128-bytes blocks 8*128=32*32
void expandRand4KB(uint8_t* in, int inSize, uint8_t* out) {
	const int bufSize=8*128;
	uint8_t* tempBuf=(uint8_t*)malloc(8*128+128);
	sha3_ctx ctx;
	rhash_sha3_512_init(&ctx);
	rhash_sha3_update(&ctx, in, inSize);
	rhash_sha3_final(&ctx, tempBuf);
	rhash_sha3_512_init(&ctx);
	rhash_sha3_update(&ctx, tempBuf, 32);
	rhash_sha3_final(&ctx, tempBuf+32);
	for(int start=64; start<bufSize; start+=32) {
		rhash_sha3_512_init(&ctx);
		rhash_sha3_update(&ctx, tempBuf+start-64, 64);
		rhash_sha3_final(&ctx, tempBuf+start);
	}
	//uint32_t* i32buf=(uint32_t*)tempBuf;
	//for(int i=0; i<bufSize/4; i++) printf("here %d %x\n",i*4,i32buf[i]);

	for(int i=0; i<8; i++) {
		for(int j=0; j<8; j++) {
			int pos=i*8+j;
			blake2b(out+64*pos, 64, tempBuf+128*i, 128, tempBuf+128*j, 64);
		}
	}
	free(tempBuf);
	//uint32_t* i32buf=(uint32_t*)out;
	//for(int i=0; i<1024; i++) printf("here %d %x\n",i*4,i32buf[i]);
}

#ifdef  SELF_TEST
static void test_expandRand4KB() {
	uint32_t* outBuf=(uint32_t*)malloc(4096);
	char mystr[100]="aeiqfcnq3i0$%$@$%		^&*jdIHIHF:JKWF";
	int l=strlen(mystr);
	expandRand4KB((uint8_t*)mystr, l, (uint8_t*)outBuf);
	for(int i=0; i<4096/4; i+=4) {
		printf("%x %x %x %x\n", outBuf[i],outBuf[i+1],outBuf[i+2],outBuf[i+3]);
	}
	free(outBuf);
}

int main( void ) {
	test_expandRand4KB();
	return 0;
}
#endif

