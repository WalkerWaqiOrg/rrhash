#include "Tracer.h"
using namespace std;

void run_HeapSort(uint8_t* seedIn, int seedSize);
void run_QuickSort(uint8_t* seedIn, int seedSize);
void run_MergeSort(uint8_t* seedIn, int seedSize); 
void run_PrioQueue(uint8_t* seedIn, int seedSize);
void run_BinomialHeap(uint8_t* seedIn, int seedSize);
void run_FibonacciHeap(uint8_t* seedIn, int seedSize);
void run_Dijkstra(uint8_t* seedIn, int seedSize);
void run_Prim(uint8_t* seedIn, int seedSize);
void run_Kruskal(uint8_t* seedIn, int seedSize);
void run_Huffman(uint8_t* seedIn, int seedSize);
void run_JobSchedule(uint8_t* seedIn, int seedSize);
void run_IncreasingConsective(uint8_t* seedIn, int seedSize);
void run_BinarySearch(uint8_t* seedIn, int seedSize);
void run_KthSmallest(uint8_t* seedIn, int seedSize);
void run_KnapSack01(uint8_t* seedIn, int seedSize);
void run_LeastSQ(uint8_t* seedIn, int seedSize);
void run_CuckooHash(uint8_t* seedIn, int seedSize);
void run_BloomFilter(uint8_t* seedIn, int seedSize);
void run_SuffixArray(uint8_t* seedIn, int seedSize);
void run_SuffixTrie(uint8_t* seedIn, int seedSize);
void run_AVLTree(uint8_t* seedIn, int seedSize);
void run_BinarySearchTree(uint8_t* seedIn, int seedSize);
void run_AutomataPatternSearch(uint8_t* seedIn, int seedSize);
void run_BTree(uint8_t* seedIn, int seedSize);
void run_KMPstring(uint8_t* seedIn, int seedSize);
void run_MatrixInverse(uint8_t* seedIn, int seedSize);
void run_QuadTree(uint8_t* seedIn, int seedSize);
void run_RedBlackTree(uint8_t* seedIn, int seedSize);
void run_RerverPolishNotation(uint8_t* seedIn, int seedSize);
void run_RobinKarp(uint8_t* seedIn, int seedSize);
void run_Viterbi(uint8_t* seedIn, int seedSize);
void run_ZArrayPatternSearch(uint8_t* seedIn, int seedSize);

extern "C" void run_all(const char *data, size_t length, unsigned char *hash);

Tracer* TRACER;
void run_all(const char *data, size_t length, unsigned char *hash) {
	static bool TRACER_ready=false;
	if(!TRACER_ready) {
		TRACER=new Tracer;
		TRACER_ready=true;
	}
	else {
		TRACER->clear();
	}
	for(int i=0; i<length-8; i+=8) {
		uint64_t* d=(uint64_t*)(data+i);
		TRACER->meet(*d);
	}
	for(int counter=0; counter<8; counter++) {
		uint8_t* seedIn;
		int seedSize;
		if(counter==0) {
			seedSize=length;
			seedIn=(uint8_t*)malloc(length);
			memcpy(seedIn,data,length);
		}
		else {
			seedSize=64;
			seedIn=(uint8_t*)malloc(64);
			memcpy(seedIn,TRACER->fnvHistory,64);
		}
		switch(TRACER->historyCksum()%32) {
			case 0:
			run_HeapSort(seedIn, seedSize);
			break;
			case 1:
			run_QuickSort(seedIn, seedSize);
			break;
			case 2:
			run_MergeSort(seedIn, seedSize); 
			break;
			case 3:
			run_PrioQueue(seedIn, seedSize);
			break;
			case 4:
			run_BinomialHeap(seedIn, seedSize);
			break;
			case 5:
			run_FibonacciHeap(seedIn, seedSize);
			break;
			case 6:
			run_Dijkstra(seedIn, seedSize);
			break;
			case 7:
			run_Prim(seedIn, seedSize);
			break;
			case 8:
			run_Kruskal(seedIn, seedSize);
			break;
			case 9:
			run_Huffman(seedIn, seedSize);
			break;
			case 10:
			run_JobSchedule(seedIn, seedSize);
			break;
			case 11:
			run_IncreasingConsective(seedIn, seedSize);
			break;
			case 12:
			run_BinarySearch(seedIn, seedSize);
			break;
			case 13:
			run_KthSmallest(seedIn, seedSize);
			break;
			case 14:
			run_KnapSack01(seedIn, seedSize);
			break;
			case 15:
			run_LeastSQ(seedIn, seedSize);
			break;
			case 16:
			run_CuckooHash(seedIn, seedSize);
			break;
			case 17:
			run_BloomFilter(seedIn, seedSize);
			break;
			case 18:
			run_SuffixArray(seedIn, seedSize);
			break;
			case 19:
			run_SuffixTrie(seedIn, seedSize);
			break;
			case 20:
			run_AVLTree(seedIn, seedSize);
			break;
			case 21:
			run_BinarySearchTree(seedIn, seedSize);
			break;
			case 22:
			run_AutomataPatternSearch(seedIn, seedSize);
			break;
			case 23:
			run_BTree(seedIn, seedSize);
			break;
			case 24:
			run_KMPstring(seedIn, seedSize);
			break;
			case 25:
			run_MatrixInverse(seedIn, seedSize);
			break;
			case 26:
			run_QuadTree(seedIn, seedSize);
			break;
			case 27:
			run_RedBlackTree(seedIn, seedSize);
			break;
			case 28:
			run_RerverPolishNotation(seedIn, seedSize);
			break;
			case 29:
			run_RobinKarp(seedIn, seedSize);
			break;
			case 30:
			run_Viterbi(seedIn, seedSize);
			break;
			default:
			run_ZArrayPatternSearch(seedIn, seedSize);
			break;
		}
		free(seedIn);
	}
	TRACER->final_result(hash);
}
