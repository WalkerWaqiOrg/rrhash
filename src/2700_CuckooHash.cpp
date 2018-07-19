// C++ program to demonstrate working of Cuckoo
// hashing.
#include<iostream>
#include<limits.h>
#include "util.h"
#include "Tracer.h"
using namespace std;

#define MAXN (9*1024)
#define ITER_COUNT 1

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
 
 
// upper bound on number of elements in our set
 
// choices for position
#define ver 2
 
// Auxiliary space bounded by a small multiple
// of MAXN, minimizing wastage
int hashtable[ver][MAXN];
 
// Array to store possible positions for a key
uint32_t pos[ver];
 
/* function to fill hash table with dummy value
 * dummy value: INT_MIN
 * number of hashtables: ver */
void initTable()
{
    for (int j=0; j<MAXN; j++)
        for (int i=0; i<ver; i++)
            hashtable[i][j] = INT_MIN;
}
 
/* return hashed value for a key
 * function: ID of hash function according to which
    key has to hashed
 * key: item to be hashed */
uint32_t myhash(uint32_t function, uint32_t key)
{
    switch (function)
    {
        case 1: return key%MAXN;
        case 2: return (key/MAXN)%MAXN;
		default: return (key/MAXN)%MAXN;
    }
}
 
/* function to place a key in one of its possible positions
 * tableID: table in which key has to be placed, also equal
   to function according to which key must be hashed
 * cnt: number of times function has already been called
   in order to place the first input key
 * n: maximum number of times function can be recursively
   called before stopping and declaring presence of cycle */
bool place(int key, int tableID, int cnt, int n)
{
    /* if function has been recursively called max number
       of times, stop and declare cycle. Rehash. */
    if (cnt==n)
    {
        //printf("%d unpositioned\n", key);
        //printf("Cycle present. REHASH.\n");
        return false;
    }
 
    /* calculate and store possible positions for the key.
     * check if key already present at any of the positions.
      If YES, return. */
    for (int i=0; i<ver; i++)
    {
        pos[i] = myhash(i+1, key);
        if (hashtable[i][pos[i]] == key)
           return true;
    }
 
    /* check if another key is already present at the
       position for the new key in the table
     * If YES: place the new key in its position
     * and place the older key in an alternate position
       for it in the next table */
    if (hashtable[tableID][pos[tableID]]!=INT_MIN)
    {
        int dis = hashtable[tableID][pos[tableID]];
		TRACER->meet(dis);
        hashtable[tableID][pos[tableID]] = key;
        place(dis, (tableID+1)%ver, cnt+1, n);
    }
    else {//else: place the new key in its position
       hashtable[tableID][pos[tableID]] = key;
    }
	return true;
}
 
/* function to print hash table contents */
void printTable()
{
    printf("Final hash tables:\n");
 
    for (int i=0; i<ver; i++, printf("\n"))
        for (int j=0; j<MAXN; j++)
            (hashtable[i][j]==INT_MIN)? printf("- "):
                     printf("%d ", hashtable[i][j]);
 
    printf("\n");
}
 
/* function for Cuckoo-hashing keys
 * keys[]: input array of keys
 * n: size of input array */
void cuckoo(int keys[], int n)
{
    // initialize hash tables to a dummy value (INT-MIN)
    // indicating empty position
    initTable();
 
    // start with placing every key at its position in
    // the first hash table according to first hash
    // function
    for (int i=0, cnt=0; i<n; i++, cnt=0) {
        bool ok=place(keys[i], 0, cnt, n);
		if(!ok) initTable();
	}
 
    //print the final hash tables
    //printTable();
}
 
///* driver function */
//int main()
//{
//    /* following array doesn't have any cycles and
//       hence  all keys will be inserted without any
//       rehashing */
//    int keys_1[] = {20, 50, 53, 75, 100, 67, 105,
//                    3, 36, 39};
// 
//    int n = sizeof(keys_1)/sizeof(int);
// 
//    cuckoo(keys_1, n);
// 
//    /* following array has a cycle and hence we will
//       have to rehash to position every key */
//    int keys_2[] = {20, 50, 53, 75, 100, 67, 105,
//                    3, 36, 39, 6};
// 
//    int m = sizeof(keys_2)/sizeof(int);
// 
//    cuckoo(keys_2, m);
// 
//    return 0;
//}

void run_CuckooHash(uint8_t* seedIn, int seedSize) {
	Pair* tmp=new Pair[MAXN/2];
	int* arr=(int*)tmp;
	for(int i=0; i<ITER_COUNT; i++) {
		for(int j=0; j<seedSize; j++) seedIn[j]++;
		fillPairArray(seedIn, seedSize, tmp, MAXN/2);
		cuckoo(arr, MAXN);
	}
	delete[] tmp;
}

#ifdef SELF_TEST
Tracer* TRACER;
int main() {
	TRACER=new Tracer;
	char hello[100]="aer3:nvqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_CuckooHash((uint8_t*)hello,len);
	}
	delete TRACER;
	return 0;
}
#endif
