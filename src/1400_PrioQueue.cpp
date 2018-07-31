#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "Tracer.h"

/**
* Debugging macro .
*
* Checks for a NULL pointer, and prints the error message, source file and
* line via 'stderr' .
* If the check fails the program exits with error code (-1) .
*/
#define NP_CHECK(ptr)                                                           \
    {                                                                           \
        if (NULL == (ptr)) {                                                    \
            fprintf(stderr, "%s:%d NULL POINTER: %s n",                         \
                __FILE__, __LINE__, #ptr);                                      \
            exit(-1);                                                           \
        }                                                                       \
    }                                                                           \

#define DEBUG(msg) fprintf(stderr, "%s:%d %s", __FILE__, __LINE__, (msg))

 
using namespace std;

#define N (511*1024/16)
#define ITER_COUNT 3
//#define N 480
//#define ITER_COUNT 4


struct Pair {
	uint64_t a;
	uint64_t b;
};

static void fillPairArray(uint8_t* seedIn, int seedSize, Pair arr[], int n) {
	uint64_t* rand512=new uint64_t[512]; //512 64-b values 
	expandRand4KB(seedIn, seedSize, (uint8_t*)rand512);
	int counter=0;
	for(int i=0; i<512; i++) {
		for(int j=0; j<512; j+=2) {
			if(j==i) continue;
			arr[counter].a=mulxor(rand512[i],rand512[j]);
			arr[counter].b=mulxor(rand512[i],rand512[j+1]);
			//printf("%d-%d: a:%016lx b:%016lx %016lx %016lx \n",i,j,
			//		arr[counter].a,arr[counter].b,
			//		rand512[i],rand512[j]);
			counter++;
			if(counter==n) {
				delete[] rand512;
				return;
			}
		}
	}
	assert(false);
}


/**
* Priority Queue Structure
*/
template<typename T>
struct PQueue {
    /* The actual size of heap at a certain time */
    size_t size;
    /* The amount of allocated memory for the heap */
    size_t capacity;
    /* An array of (void*), the actual max-heap */
    T *data;

	/** Allocates memory for a new Priority Queue .
	Needs a pointer to a comparator function, thus establishing priorities .
	*/
	PQueue(size_t capacity) {
		/* The inner representation of data inside the queue is an array of void* */
		this->data = new T[capacity];
		NP_CHECK(this->data);
		this->size = 0;
		this->capacity = capacity;
	}

	/** De-allocates memory for a given Priority Queue */
	~PQueue() {
		delete[] this->data;
	}

	/** Add an element inside the Priority Queue */
	void enqueue(const T& data);

	/** Removes the element with the greatest priority from within the Queue */
	T dequeue();

/* Util macros */
#define LEFT(x) (2 * (x) + 1)
#define RIGHT(x) (2 * (x) + 2)
#define PARENT(x) ((x) / 2)

	void heapify(size_t idx);
};


/**
* Adds a new element to the Priority Queue .
*/
template<typename T>
void PQueue<T>::enqueue(const T& data) {
    size_t i;
    T tmp;
    if (this->size >= this->capacity) {
        DEBUG("Priority Queue is full. Cannot add another element .");
        return;
    }
    /* Adds element last */
    this->data[this->size] = data;
    i = this->size;
    this->size++;
    /* The new element is swapped with its parent as long as its
    precedence is higher */
    while(i > 0 && this->data[i] > this->data[PARENT(i)]) {
        tmp = this->data[i];
        this->data[i] = this->data[PARENT(i)];
        this->data[PARENT(i)] = tmp;
        i = PARENT(i);
    }
}

/**
* Returns the element with the biggest priority from the queue .
*/
template<typename T>
T PQueue<T>::dequeue() {
    T data;
    if (this->size < 1) {         
         /* Priority Queue is empty */         
         DEBUG("Priority Queue underflow . Cannot remove another element .");         
    }     
    data = this->data[0];
    this->data[0] = this->data[this->size-1];
    this->size--;
    /* Restore heap property */
    heapify(0);
    return (data);
}

/**
* Turn an "almost-heap" into a heap .
*/
template<typename T>
void PQueue<T>::heapify(size_t idx) {
    /* left index, right index, largest */
    T tmp;
    size_t l_idx, r_idx, lrg_idx;

    l_idx = LEFT(idx);
    r_idx = RIGHT(idx);

    /* Left child exists, compare left child with its parent */
    if (l_idx < this->size && this->data[l_idx] > this->data[idx]) {
        lrg_idx = l_idx;
    } else {
        lrg_idx = idx;
    }

    /* Right child exists, compare right child with the largest element */
    if (r_idx < this->size && this->data[r_idx] > this->data[lrg_idx]) {
        lrg_idx = r_idx;
    }

    /* At this point largest element was determined */
    if (lrg_idx != idx) {
        /* Swap between the index at the largest element */
        tmp = this->data[lrg_idx];
        this->data[lrg_idx] = this->data[idx];
        this->data[idx] = tmp;
		Tracer::I()->meet(tmp);
        /* Heapify again */
        heapify(lrg_idx);
    }
}

//int main(int argc, char** argv) {
//    
//    PQueue<int> pq(200);
//    
//    int x = 100, y = 50, z = 300, k = 100, w = 1000;
//    
//    pq.enqueue(x);
//    pq.enqueue(y);
//    pq.enqueue(z);
//    pq.enqueue(k);
//    pq.enqueue(w);
//    
//    int i = 0;
//    for(;i<5;++i)
//        printf("%d\n", pq.dequeue());
//    
//    return (EXIT_SUCCESS);
//}

void run_PrioQueue(uint8_t* seedIn, int seedSize) {
	Pair* arr=new Pair[N];
	fillPairArray(seedIn, seedSize, arr, N);
	//for(int j=0; j<N; j++) printf("init %d: %016lx %016lx\n",j,arr[j].a,arr[j].b);
	PQueue<uint64_t> pq(32*1024/8);
	for(unsigned int i=0; i<pq.capacity/2&&i<N; i++) {
		pq.enqueue(arr[i].a^arr[i].b);
	}
	for(int j=0; j<ITER_COUNT; j++) {
		for(int i=0; i<N; i++) {
			uint64_t x;
			if(pq.size>=1) {
				x=pq.dequeue();
				//printf("haha %016lx\n",x);
			}
			if(x%2==0&&pq.size<pq.capacity-2) {
				pq.enqueue(arr[i].a);
				pq.enqueue(arr[i].b);
			}
		}
	}
	delete[] arr;
}

#undef N
#undef ITER_COUNT

#ifdef SELF_TEST
int main() {
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_PrioQueue((uint8_t*)hello,len);
	}
	return 0;
}
#endif
