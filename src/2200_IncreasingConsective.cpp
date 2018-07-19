#include <iostream>
#include <map>
#include <unordered_map>
#include <limits.h>
using namespace std;
#include "util.h"
#include "Tracer.h"

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
 
 
// function that returns the length of the
// longest increasing subsequence
// whose adjacent element differ by 1
int longestSubsequence(int a[], int n)
{
    // stores the index of elements
    unordered_map<int, int> mp;
 
    // stores the length of the longest 
    // subsequence that ends with a[i]
    int dp[n];
    memset(dp, 0, sizeof(dp)); 
 
    int maximum = INT_MIN;
 
    // iterate for all element
    for (int i = 0; i < n; i++) {
 
        // if a[i]-1 is present before i-th index
        if (mp.find(a[i] - 1) != mp.end()) {
 
            // last index of a[i]-1
            int lastIndex = mp[a[i] - 1] - 1;
 
            // relation
            dp[i] = 1 + dp[lastIndex];
			TRACER->meet(dp[i]);
        }
        else
            dp[i] = 1;
 
        // stores the index as 1-index as we need to
        // check for occurrence, hence 0-th index
        // will not be possible to check
        mp[a[i]] = i + 1;
 
        // stores the longest length
        maximum = max(maximum, dp[i]);
    }
 
    return maximum;
}
 
//// Driver Code
//int main()
//{
//    int a[] = { 3, 10, 3, 11, 4, 5, 6, 7, 8, 12 };
//    int n = sizeof(a) / sizeof(a[0]);
//    cout << longestSubsequence(a, n);
//    return 0;
//}


void run_IncreasingConsective(uint8_t* seedIn, int seedSize) {
	const int Count=180*1024;
	const int IterCount=40;
	Pair* arr=new Pair[Count/2];
	int32_t* iarr=(int32_t*)arr;
	fillPairArray(seedIn, seedSize, arr, Count/2);
	int32_t tmp=0;
	for(int i=0; i<Count; i++) {
		if((iarr[i]&0xF)==0) tmp=iarr[i];
		else {
			tmp+=iarr[i]&0x3;
			iarr[i]=tmp;
		}
	}
	Pair* startend=arr;
	for(int i=0; i<IterCount; i++) {
		int32_t start=(startend[i].a>>16)%Count;
		int32_t end=(startend[i].b>>16)%Count;
		if(start>end) {
			int32_t temp;
			temp=start; start=end; end=temp;
		}
		int len=longestSubsequence(iarr+start, end-start);
		//cout<<"len "<<len<<endl;
		TRACER->meet(len);
	}
	delete[] arr;
}

#ifdef SELF_TEST
Tracer* TRACER;
int main() {
	TRACER=new Tracer;
	char hello[100]="aer9invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_IncreasingConsective((uint8_t*)hello,len);
	}
	delete TRACER;
	return 0;
}
#endif
