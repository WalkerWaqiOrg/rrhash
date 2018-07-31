#include <iostream>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "util.h"
#include "Tracer.h"
 
#define N (211*1024/8)
#define ITER_COUNT 1
//#define N 480
//#define ITER_COUNT 4

using namespace std;

struct Pair {
	uint64_t a;
	uint64_t b;
};
struct PairA: public Pair {
	bool operator< (const PairA& other) {return a <other.a;}
	bool operator<=(const PairA& other) {return a<=other.a;}
	bool operator>=(const PairA& other) {return a>=other.a;}
	bool operator> (const PairA& other) {return a >other.a;}
	bool operator==(const PairA& other) {return a==other.a;}
	void mix(const PairA& other) {b^=other.b;}
	uint64_t v() {return b;}
};
struct PairB: public Pair {
	bool operator< (const PairB& other) {return b <other.b;}
	bool operator<=(const PairB& other) {return b<=other.b;}
	bool operator>=(const PairB& other) {return b>=other.b;}
	bool operator> (const PairB& other) {return b >other.b;}
	bool operator==(const PairB& other) {return b==other.b;}
	void mix(const PairB& other) {a^=other.a;}
	uint64_t v() {return a;}
};
template <typename T>
static void mixArray(T a[], int n) {
	for(int i=1; i<n; i++) {
		a[i].mix(a[i-1]);
	}
}

static void fillPairArray(uint8_t* seedIn, int seedSize, Pair* arr, int n) {
	const int C=512;
	uint64_t* rand512=(uint64_t*)malloc(512*8); //512 64-b values 
	assert(rand512!=NULL);
	expandRand4KB(seedIn, seedSize, (uint8_t*)rand512);
	int counter=0;
	for(int i=0; i<C; i++) {
		for(int j=0; j<C; j+=2) {
			if(j==i) continue;
			arr[counter].a=mulxor(rand512[i],rand512[j]);
			arr[counter].b=mulxor(rand512[i],rand512[j+1]);
			//printf("%d-%d: a:%08x b:%08x %016llx %016llx \n",i,j,
			//		arr[counter].a,arr[counter].b,
			//		rand512[i],rand512[j]);
			counter++;
			if(counter==n) {
				free(rand512);
				return;
			}
		}
	}
	assert(false);
}
 
// A function to heapify the array.
template <typename T>
static void maxHeapify(T a[], int i, int n)
{
	int j;
	T temp;
	temp = a[i];
	j = 2*i;
 
 	while (j <= n)
	{
		if (j < n && a[j+1] > a[j])
		j = j+1;
		// Break if parent value is already greater than child value.
		if (temp > a[j])
			break;
		// Switching value with the parent node if temp < a[j].
		else if (temp <= a[j])
		{
			a[j/2] = a[j];
			Tracer::I()->meet(a[j].v());
			j = 2*j;
		}
	}
	a[j/2] = temp;
	return;
}

template <typename T>
static void heapSortCore(T a[], int n)
{
	int i;
	T temp;
	for (i = n; i >= 2; i--)
	{
		// Storing maximum value at the end.
		temp = a[i];
		a[i] = a[1];
		a[1] = temp;
		// Building max heap of remaining element.
		maxHeapify(a, 1, i - 1);
	}
}

template <typename T>
static void buildMaxHeap(T a[], int n)
{
	int i;
	for(i = n/2; i >= 1; i--)
		maxHeapify(a, i, n);
}

template <typename T>
static void heapSort(T arr[], int n) {
	buildMaxHeap<T>(arr, n-1);
	heapSortCore<T>(arr, n-1);
}

void run_HeapSort(uint8_t* seedIn, int seedSize) {
	Pair* arr=(Pair*)malloc(N*sizeof(Pair));
	assert(arr!=NULL);
	PairA* arrA=(PairA*)arr;
	PairB* arrB=(PairB*)arr;
	fillPairArray(seedIn, seedSize, arr, N);
	//for(int j=0; j<N; j++) printf("init %d: %016llx %016llx\n",j,arr[j].a,arr[j].b);
	for(int i=0; i<ITER_COUNT; i++) {
		heapSort<PairA>(arrA, N);
		mixArray<PairA>(arrA, N);
		//for(int j=0; j<N; j++) printf("%d-%d: %016llx %016llx\n",i,j,arr[j].a,arr[j].b);
		heapSort<PairB>(arrB, N);
		mixArray<PairB>(arrB, N);
		//for(int j=0; j<N; j++) printf("%d-%d: %016llx %016llx\n",i,j,arr[j].a,arr[j].b);
	}
	Tracer::I()->sha3_update((unsigned char*)arr, 1024);
	free(arr);
}

#undef N
#undef ITER_COUNT

#ifdef SELF_TEST
int main() {
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_HeapSort((uint8_t*)hello,len);
	}
	return 0;
}
#endif
