#include <iostream>
#include <algorithm>
#include <stdint.h>
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
 
using namespace std;
 
static bool binarySearch(int32_t* array, int n, int32_t search, int32_t* value) {
	int first = 0;
	int last = n - 1;
	int middle = (first+last)/2;
	
	while (first <= last) {
		if (array[middle] < search)
			first = middle + 1;    
		else if (array[middle] == search) {
			*value=array[middle];
			//printf("%d found at location %d.\n", search, middle+1);
			break;
		}
		else {
			last = middle - 1;
		}
		middle = (first + last)/2;
	}
	if (first > last) {
		*value=array[middle];
		return false;
	}
	else {
		return true;
	}
}

void run_BinarySearch(uint8_t* seedIn, int seedSize) {
	const int Count=64*1024;
	const int IterCount=180*1024;
	Pair* arr=new Pair[Count/2];
	int32_t* iarr=(int32_t*)arr;
	fillPairArray(seedIn, seedSize, arr, Count/2);
	sort(iarr, iarr+Count);
	//for(int i=0; i<Count; i++) {
	//	cout<<"sort "<<iarr[i]<<endl;
	//}
	Pair* startend=new Pair[IterCount];
	fillPairArray(seedIn, seedSize, startend, IterCount);
	for(int i=0; i<IterCount; i++) {
		int32_t start=startend[i].a%Count;
		int32_t end=startend[i].b%Count;
		if(start>end) {
			int32_t temp;
			temp=start; start=end; end=temp;
		}
		int32_t avg=(int64_t(iarr[start])+int64_t(iarr[end]))/2;
		int32_t nearest;
		bool found=binarySearch(iarr+start, end-start, avg, &nearest);
		if(found) {
			TRACER->meet(avg);
			//cout<<"found "<<avg<<endl;
		}
		else {
			TRACER->meet(~nearest);
			//cout<<"not found "<<avg<<" nearest "<<nearest<<endl;
		}
	}
	delete[] arr;
	delete[] startend;
}

#ifdef SELF_TEST
Tracer* TRACER;
int main() {
	TRACER=new Tracer;
	char hello[100]="aer39invqbj3to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_BinarySearch((uint8_t*)hello,len);
	}
	delete TRACER;
	return 0;
}
#endif
