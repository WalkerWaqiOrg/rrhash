/* Following program is a C implementation of Rabin Karp
Algorithm given in the CLRS book */
#include<stdio.h>
#include<string.h>
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
 
 
// d is the number of characters in input alphabet
#define d 256
 
/* pat -> pattern
    txt -> text
    q -> A prime number
*/
static void search(char pat[], char txt[], int q, int M, int N)
{
    int i, j;
    int p = 0; // hash value for pattern
    int t = 0; // hash value for txt
    int h = 1;
 
    // The value of h would be "pow(d, M-1)%q"
    for (i = 0; i < M-1; i++)
        h = (h*d)%q;
 
    // Calculate the hash value of pattern and first
    // window of text
    for (i = 0; i < M; i++)
    {
        p = (d*p + pat[i])%q;
        t = (d*t + txt[i])%q;
    }
 
    // Slide the pattern over text one by one
    for (i = 0; i <= N - M; i++)
    {
 
        // Check the hash values of current window of text
        // and pattern. If the hash values match then only
        // check for characters on by one
        if ( p == t )
        {
			TRACER->meet(p);
            /* Check for characters one by one */
            for (j = 0; j < M; j++)
            {
                if (txt[i+j] != pat[j])
                    break;
            }
 
            // if p == t and pat[0...M-1] = txt[i, i+1, ...i+M-1]
            if (j == M) {
                //printf("Pattern found at index %d \n", i);
				TRACER->meet(M);
			}
        }
 
        // Calculate hash value for next window of text: Remove
        // leading digit, add trailing digit
        if ( i < N-M )
        {
            t = (d*(t - txt[i]*h) + txt[i+M])%q;
 
            // We might get negative value of t, converting it
            // to positive
            if (t < 0)
            t = (t + q);
        }
    }
}
 
///* Driver program to test above function */
//int main()
//{
//    char txt[] = "GEEKS FOR GEEKS";
//    char pat[] = "GEEK";
//    int q = 101; // A prime number
//    search(pat, txt, q);
//    return 0;
//}


void run_RobinKarp(uint8_t* seedIn, int seedSize) {
    int q = 101; // A prime number
	const int Count=13*1024;
	Pair* tmp=new Pair[Count/8];
	fillPairArray(seedIn, seedSize, tmp, Count/8);
	char* arr=(char*)tmp;
	for(int step=128; step<=256; step+=4) {
		for(int i=0; i<Count-step-10; i+=step) {
			search(arr+i+step, arr+i, q, arr[i+step+1]+arr[i+step+2]+1, step);
		}
	}
    q = 103;
	for(int step=128; step<=256; step+=4) {
		for(int i=0; i<Count-step-10; i+=step) {
			search(arr+i+step, arr+i, q, arr[i+step+1]+arr[i+step+2]+1, step);
		}
	}
    q = 107;
	for(int step=128; step<=256; step+=4) {
		for(int i=0; i<Count-step-10; i+=step) {
			search(arr+i+step, arr+i, q, arr[i+step+1]+arr[i+step+2]+1, step);
		}
	}
    q = 109;
	for(int step=128; step<=256; step+=4) {
		for(int i=0; i<Count-step-10; i+=step) {
			search(arr+i+step, arr+i, q, arr[i+step+1]+arr[i+step+2]+1, step);
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
		run_RobinKarp((uint8_t*)hello,len);
	}
	delete TRACER;
	return 0;
}
#endif
