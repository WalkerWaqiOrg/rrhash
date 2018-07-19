// C program for Finite Automata Pattern searching
// Algorithm
#include<stdio.h>
#include<string.h>
#define NO_OF_CHARS 256
#include "util.h"
#include "Tracer.h"
using namespace std;

extern Tracer* TRACER;

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
			arr[counter].a=0x00030003&mulxor(rand1024[i],rand1024[j]);
			arr[counter].b=0x00030003&mulxor(rand1024[i],rand1024[j+1]);
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
 
 
static int getNextState(char *pat, int M, int state, int x)
{
    // If the character c is same as next character
    // in pattern,then simply increment state
    if (state < M && x == pat[state])
        return state+1;
 
    // ns stores the result which is next state
    int ns, i;
 
    // ns finally contains the longest prefix
    // which is also suffix in "pat[0..state-1]c"
 
    // Start from the largest possible value
    // and stop when you find a prefix which
    // is also suffix
    for (ns = state; ns > 0; ns--)
    {
        if (pat[ns-1] == x)
        {
            for (i = 0; i < ns-1; i++)
                if (pat[i] != pat[state-ns+1+i])
                    break;
            if (i == ns-1)
                return ns;
        }
    }
 
    return 0;
}
 
/* This function builds the TF table which represents4
    Finite Automata for a given pattern */
static void computeTF(char *pat, int M, int TF[][NO_OF_CHARS])
{
    int state, x;
    for (state = 0; state <= M; ++state)
        for (x = 0; x < NO_OF_CHARS; ++x)
            TF[state][x] = getNextState(pat, M, state, x);
}
 
/* Prints all occurrences of pat in txt */
static void search(char *pat, char *txt, int M, int N)
{
 
    int TF[M+1][NO_OF_CHARS];
 
    computeTF(pat, M, TF);
 
    // Process txt over FA.
    int i, state=0;
	int tmp=0;
    for (i = 0; i < N; i++)
    {
        state = TF[state][int(txt[i])];
		tmp=(tmp<<1)^state;
        if (state == M) {
			TRACER->meet(tmp);
            //printf ("\n Pattern found at index %d", i-M+1);
		}
    }
}
 
//// Driver program to test above function
//int main()
//{
//    char *txt = "AABAACAADAABAAABAA";
//    char *pat = "AABA";
//    int M = strlen(pat);
//    int N = strlen(txt);
//    search(pat, txt, M, N);
//    return 0;
//}


void run_AutomataPatternSearch(uint8_t* seedIn, int seedSize) {
	const int Count=140*1024;
	Pair* tmp=new Pair[Count/8];
	fillPairArray(seedIn, seedSize, tmp, Count/8);
	char* arr=(char*)tmp;
	for(int step=128; step<=256; step+=16) {
		for(int i=0; i<Count-step-10; i+=step) {
			search(arr+i+step, arr+i, arr[i+step+1]+arr[i+step+2]+1, step);
		}
	}
	delete[] tmp;
}

#ifdef SELF_TEST
Tracer* TRACER;
int main() {
	TRACER=new Tracer;
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_AutomataPatternSearch((uint8_t*)hello,len);
	}
	delete TRACER;
	return 0;
}
#endif
