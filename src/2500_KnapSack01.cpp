// A Dynamic Programming based solution for 0-1 Knapsack problem
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <assert.h>
#include "util.h"
#include "Tracer.h"
using namespace std;

struct Pair {
	uint32_t a;
	uint32_t b;
};

static void fillPairArray(uint8_t* seedIn, int seedSize, Pair arr[], int n) {
	uint32_t* rand1024=new uint32_t[1024]; //1024 32-b values 
	expandRand4KB(seedIn, seedSize, (uint8_t*)rand1024);
	int counter=0;
	for(int i=0; i<1024; i++) {
		for(int j=0; j<1024; j+=2) {
			if(j==i) continue;
			arr[counter].a=mulxor(rand1024[i],rand1024[j]);
			arr[counter].b=mulxor(rand1024[i],rand1024[j+1]);
			//printf("%d-%d: a:%08x b:%08x %08x %08x \n",i,j,
			//		arr[counter].a,arr[counter].b,
			//		rand1024[i],rand1024[j]);
			counter++;
			if(counter==n) {
				delete[] rand1024;
				return;
			}
		}
	}
	assert(false);
}
 
 
// A utility function that returns maximum of two integers
inline uint32_t max(uint32_t a, uint32_t b) { return (a > b)? a : b; }
 
// Returns the maximum value that can be put in a knapsack of capacity W
uint32_t knapSack(uint32_t W, uint16_t wt[], uint16_t val[], uint32_t n) {
   uint32_t i, w;
   uint32_t U=W+1;
   //int K[n+1][W+1];
   uint32_t* K=(uint32_t*)malloc((n+1)*(W+1)*sizeof(int));
   assert(K!=NULL);
 
   // Build table K[][] in bottom up manner
   for (i = 0; i <= n; i++) {
       for (w = 0; w <= W; w++) {
           if (i==0 || w==0) {
               K[i*U+w] = 0;
		   }
           else if (wt[i-1] <= w) {
               K[i*U+w] = max(val[i-1] + K[(i-1)*U + w-wt[i-1]],  K[(i-1)*U+w]);
		   }
           else {
               K[i*U+w] = K[(i-1)*U+w];
			   Tracer::I()->meet(K[i*U+w]);
		   }
       }
   }
 
   uint32_t result=K[n*U+W];
   free(K);
   return result;
}
 
//int main() {
//    int val[] = {60, 100, 120};
//    int wt[] = {10, 20, 30};
//    int  W = 50;
//    int n = sizeof(val)/sizeof(val[0]);
//    printf("%d", knapSack(W, wt, val, n));
//    return 0;
//}

void run_KnapSack01(uint8_t* seedIn, int seedSize) {
	const int Count=4*1024;
	const int IterCount=2;
	Pair* tmp1=new Pair[Count/4];
	fillPairArray(seedIn, seedSize, tmp1, Count/4);
	uint16_t* val=(uint16_t*)tmp1;
	Pair* tmp2=new Pair[Count/4];
	fillPairArray(seedIn+8, seedSize-8, tmp2, Count/4);
	uint16_t* wt=(uint16_t*)tmp2;
	for(int i=0; i<Count; i++) {
		wt[i]%=32;
	}
	for(int i=0; i<IterCount; i++) {
		uint32_t W=i*32+38;
		int result=knapSack(W,wt+i,val,Count-i);
		Tracer::I()->meet(result);
		//printf("result is %d\n",result);
	}
	delete[] tmp1;
	delete[] tmp2;
}

#ifdef SELF_TEST
int main() {
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_KnapSack01((uint8_t*)hello,len);
	}
	return 0;
}
#endif
