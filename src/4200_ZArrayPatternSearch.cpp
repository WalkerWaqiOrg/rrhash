// A C++ program that implements Z algorithm for pattern searching
#include<iostream>
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
 
 
static void getZarr(string str, int Z[]);
 
//  prints all occurrences of pattern in text using Z algo
static void search(string text, string pattern)
{
    // Create concatenated string "P$T"
    string concat = pattern + "$" + text;
    int l = concat.length();
 
    // Construct Z array
    int Z[l];
    getZarr(concat, Z);
 
    //  now looping through Z array for matching condition
    for (int i = 0; i < l; ++i)
    {
        // if Z[i] (matched region) is equal to pattern
        // length  we got the pattern
        if (Z[i] == int(pattern.length())) {
            //cout << "Pattern found at index "
            //     <<  i - pattern.length() -1 << endl;
			TRACER->meet(i - pattern.length() -1);
		}
    }
}
 
//  Fills Z array for given string str[]
static void getZarr(string str, int Z[])
{
    int n = str.length();
    int L, R, k;
 
    // [L,R] make a window which matches with prefix of s
    L = R = 0;
    for (int i = 1; i < n; ++i)
    {
        // if i>R nothing matches so we will calculate.
        // Z[i] using naive way.
        if (i > R)
        {
            L = R = i;
 
            // R-L = 0 in starting, so it will start
            // checking from 0'th index. For example,
            // for "ababab" and i = 1, the value of R
            // remains 0 and Z[i] becomes 0. For string
            // "aaaaaa" and i = 1, Z[i] and R become 5
            while (R<n && str[R-L] == str[R])
                R++;
            Z[i] = R-L;
            R--;
			TRACER->meet(R);
        }
        else
        {
            // k = i-L so k corresponds to number which
            // matches in [L,R] interval.
            k = i-L;
 
            // if Z[k] is less than remaining interval
            // then Z[i] will be equal to Z[k].
            // For example, str = "ababab", i = 3, R = 5
            // and L = 2
            if (Z[k] < R-i+1) {
                Z[i] = Z[k];
 
            // For example str = "aaaaaa" and i = 2, R is 5,
            // L is 0
				TRACER->meet(Z[i]);
			}
            else
            {
                //  else start from R  and check manually
                L = i;
                while (R<n && str[R-L] == str[R])
                    R++;
                Z[i] = R-L;
                R--;
            }
        }
    }
}
 
//// Driver program
//int main()
//{
//    string text = "GEEKS FOR GEEKS";
//    string pattern = "GEEK";
//    search(text, pattern);
//    return 0;
//}

void run_ZArrayPatternSearch(uint8_t* seedIn, int seedSize) {
	const int Count=72*1024;
	Pair* tmp=new Pair[Count/8];
	fillPairArray(seedIn, seedSize, tmp, Count/8);
	char* arr=(char*)tmp;
	for(int step=128; step<=256; step+=8) {
		for(int i=0; i<Count-step; i+=step) {
			string text(arr+i, step);
			string pattern(arr+i+step, arr[i+step+1]+arr[i+step+2]+1);
			search(text, pattern);
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
		run_ZArrayPatternSearch((uint8_t*)hello,len);
	}
	delete TRACER;
	return 0;
}
#endif

