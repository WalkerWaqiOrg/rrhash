// C++ Program to find the maximum profit job sequence 
// from a given array of jobs with deadlines and profits
#include <iostream>
#include <algorithm>
#include <limits.h>
using namespace std;
#include "util.h"
#include "Tracer.h"

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
 
 
// A structure to represent various attributes of a Job
struct Job
{
    // Each job has id, deadline and profit
    int id;
    int deadLine, profit;
};
 
// A Simple Disjoint Set Data Structure
struct DisjointSet
{
    int *parent;
 
    // Constructor
    DisjointSet(int n)
    {
        parent = new int[n+1];
 
        // Every node is a parent of itself
        for (int i = 0; i <= n; i++)
            parent[i] = i;
    }
 
    // Path Compression
    int find(int s)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (s == parent[s])
            return s;
        return parent[s] = find(parent[s]);
    }
 
    // Makes u as parent of v.
    void merge(int u, int v)
    {
        //update the greatest available
        //free slot to u
        parent[v] = u;
    }
};
 
// Used to sort in descending order on the basis
// of profit for each job
bool cmp(Job a, Job b)
{
    return (a.profit > b.profit);
}
 
// Functions returns the maximum deadline from the set
// of jobs
int findMaxDeadline(struct Job arr[], int n)
{
    int ans = INT_MIN;
    for (int i = 0; i < n; i++)
        ans = max(ans, arr[i].deadLine);
    return ans;
}
 
void printJobScheduling(Job arr[], int n)
{
    // Sort Jobs in descending order on the basis
    // of their profit
    sort(arr, arr + n, cmp);
 
    // Find the maximum deadline among all jobs and
    // create a disjoint set data structure with
    // maxDeadline disjoint sets initially.
    int maxDeadline = findMaxDeadline(arr, n);
    DisjointSet ds(maxDeadline);
 
    // Traverse through all the jobs
    for (int i = 0; i < n; i++)
    {
        // Find the maximum available free slot for
        // this job (corresponding to its deadline)
        int availableSlot = ds.find(arr[i].deadLine);
 
        // If maximum available free slot is greater
        // than 0, then free slot available
        if (availableSlot > 0)
        {
            // This slot is taken by this job 'i'
            // so we need to update the greatest 
            // free slot. Note that, in merge, we 
            // make first parameter as parent of 
            // second parameter. So future queries
            // for availableSlot will return maximum
            // available slot in set of 
            // "availableSlot - 1"
            ds.merge(ds.find(availableSlot - 1), 
                             availableSlot);
 
            //cout << arr[i].id << " "<<endl;
			Tracer::I()->meet(arr[i].id);
        }
    }
}
 
//// Driver program to test above function
//int main()
//{
//    Job arr[] =  { { 'a', 2, 100 }, { 'b', 1, 19 }, 
//                   { 'c', 2, 27 },  { 'd', 1, 25 }, 
//                   { 'e', 3, 15 } };
//    int n = sizeof(arr) / sizeof(arr[0]);
//    cout << "Following jobs need to be "
//         << "executed for maximum profit\n";
//    printJobScheduling(arr, n);
//    return 0;
//}

void run_JobSchedule(uint8_t* seedIn, int seedSize) {
	const int Count=1024*128;
    Job* jobList=new Job[Count];
	Pair* arr=new Pair[Count];
	int deadLine=1;
	fillPairArray(seedIn, seedSize, arr, Count);
	for(int i=0; i<Count; i++) {
		jobList[i].id=arr[i].a;
		jobList[i].deadLine=deadLine;
		jobList[i].profit=arr[i].b;
		deadLine+=arr[i].a&0xF;
	}
	printJobScheduling(jobList, Count);
	delete[] arr;
	delete[] jobList;
}

#ifdef SELF_TEST
int main() {
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_JobSchedule((uint8_t*)hello,len);
	}
	return 0;
}
#endif
