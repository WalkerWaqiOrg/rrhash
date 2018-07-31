// C++ program for implementation of KMP pattern searching
// algorithm
#include<iostream>
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
 
 
static void computeLPSArray(char *pat, int M, int *lps);
 
// Prints occurrences of txt[] in pat[]
static void KMPSearch(char *pat, char *txt, int M, int N)
{
	static int tmp=0; 
    // create lps[] that will hold the longest prefix suffix
    // values for pattern
    int lps[M];
 
    // Preprocess the pattern (calculate lps[] array)
    computeLPSArray(pat, M, lps);
 
    int i = 0;  // index for txt[]
    int j  = 0;  // index for pat[]
    while (i < N)
    {
        if (pat[j] == txt[i])
        {
            j++;
            i++;
        }
 
        if (j == M)
        {
            //printf("Found pattern at index %d \n", i-j);
            j = lps[j-1];
			tmp = (tmp<<2)^j;
        }
 
        // mismatch after j matches
        else if (i < N && pat[j] != txt[i])
        {
            // Do not match lps[0..lps[j-1]] characters,
            // they will match anyway
			Tracer::I()->meet(tmp);
            if (j != 0)
                j = lps[j-1];
            else
                i = i+1;
        }
    }
}
 
// Fills lps[] for given patttern pat[0..M-1]
static void computeLPSArray(char *pat, int M, int *lps)
{
    // length of the previous longest prefix suffix
    int len = 0;
 
    lps[0] = 0; // lps[0] is always 0
 
    // the loop calculates lps[i] for i = 1 to M-1
    int i = 1;
    while (i < M)
    {
        if (pat[i] == pat[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else // (pat[i] != pat[len])
        {
            // This is tricky. Consider the example.
            // AAACAAAA and i = 7. The idea is similar 
            // to search step.
            if (len != 0)
            {
                len = lps[len-1];
 
                // Also, note that we do not increment
                // i here
            }
            else // if (len == 0)
            {
                lps[i] = 0;
                i++;
            }
        }
    }
}
 
//// Driver program to test above function
//int main()
//{
//    char *txt = "ABABDABACDABABCABAB";
//    char *pat = "ABABCABAB";
//    int M = strlen(pat);
//    int N = strlen(txt);
//    KMPSearch(pat, txt, M, N);
//    return 0;
//}


void run_KMPstring(uint8_t* seedIn, int seedSize) {
	const int Count=25*1024;
	Pair* tmp=new Pair[Count/8];
	fillPairArray(seedIn, seedSize, tmp, Count/8);
	char* arr=(char*)tmp;
	for(int step=128; step<=256; step+=4) {
		for(int i=0; i<Count-step-10; i+=step) {
			KMPSearch(arr+i+step, arr+i, arr[i+step+1]+arr[i+step+2]+1, step);
		}
	}
	delete[] tmp;
}

#ifdef SELF_TEST
int main() {
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_KMPstring((uint8_t*)hello,len);
	}
	return 0;
}
#endif
