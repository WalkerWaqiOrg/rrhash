// C++ program to implement different operations
// on Binomial Heap
#include <iostream>
#include <list>
#include "util.h"
#include "Tracer.h"
using namespace std;
#define N (400*3)
#define ITER_COUNT 1
//#define N 480
//#define ITER_COUNT 4

#define THRES 2048

extern Tracer* TRACER;

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

 
// A Binomial Tree node.
template <typename T>
struct Node
{
    T data;
	int degree;
    Node *child, *sibling, *parent;
};
 
template <typename T>
Node<T>* newNode(T key)
{
    Node<T> *temp = new Node<T>;
    temp->data = key;
    temp->degree = 0;
    temp->child = temp->parent = temp->sibling = NULL;
    return temp;
}
 
// This function merge two Binomial Trees.
template <typename T>
Node<T>* mergeBinomialTrees(Node<T> *b1, Node<T> *b2)
{
    // Make sure b1 is smaller
    if (b1->data > b2->data)
        swap(b1, b2);
 
    // We basically make larger valued tree
    // a child of smaller valued tree
    b2->parent = b1;
    b2->sibling = b1->child;
    b1->child = b2;
    b1->degree++;
 
    return b1;
}
 
// This function perform union operation on two
// binomial heap i.e. l1 & l2
template <typename T>
list<Node<T>*> unionBionomialHeap(list<Node<T>*> l1,
                               list<Node<T>*> l2)
{
    // _new to another binomial heap which contain
    // new heap after merging l1 & l2
    list<Node<T>*> _new;
    typename list<Node<T>*>::iterator it = l1.begin();
    typename list<Node<T>*>::iterator ot = l2.begin();
    while (it!=l1.end() && ot!=l2.end())
    {
        // if D(l1) <= D(l2)
        if((*it)->degree <= (*ot)->degree)
        {
            _new.push_back(*it);
            it++;
			TRACER->meet( (*it)->data );
        }
        // if D(l1) > D(l2)
        else
        {
            _new.push_back(*ot);
            ot++;
        }
    }
 
    // if there remains some elements in l1
    // binomial heap
    while (it != l1.end())
    {
        _new.push_back(*it);
        it++;
    }
 
    // if there remains some elements in l2
    // binomial heap
    while (ot!=l2.end())
    {
        _new.push_back(*ot);
        ot++;
    }
    return _new;
}
 
// adjust function rearranges the heap so that
// heap is in increasing order of degree and
// no two binomial trees have same degree in this heap
template <typename T>
list<Node<T>*> adjust(list<Node<T>*> _heap)
{
    if (_heap.size() <= 1)
        return _heap;
    list<Node<T>*> new_heap;
    typename list<Node<T>*>::iterator it1,it2,it3;
    it1 = it2 = it3 = _heap.begin();
 
    if (_heap.size() == 2)
    {
        it2 = it1;
        it2++;
        it3 = _heap.end();
    }
    else
    {
        it2++;
        it3=it2;
        it3++;
    }
    while (it1 != _heap.end())
    {
        // if only one element remains to be processed
        if (it2 == _heap.end())
            it1++;
 
        // If D(it1) < D(it2) i.e. merging of Binomial
        // Tree pointed by it1 & it2 is not possible
        // then move next in heap
        else if ((*it1)->degree < (*it2)->degree)
        {
            it1++;
            it2++;
            if(it3!=_heap.end())
                it3++;
        }
 
        // if D(it1),D(it2) & D(it3) are same i.e.
        // degree of three consecutive Binomial Tree are same
        // in heap
        else if (it3!=_heap.end() &&
                (*it1)->degree == (*it2)->degree &&
                (*it1)->degree == (*it3)->degree)
        {
            it1++;
            it2++;
            it3++;
        }
 
        // if degree of two Binomial Tree are same in heap
        else if ((*it1)->degree == (*it2)->degree)
        {
            *it1 = mergeBinomialTrees(*it1,*it2);
            it2 = _heap.erase(it2);
            if(it3 != _heap.end())
                it3++;
        }
    }
    return _heap;
}
 
// inserting a Binomial Tree into binomial heap
template <typename T>
list<Node<T>*> insertATreeInHeap(list<Node<T>*> _heap,
                              Node<T> *tree)
{
    // creating a new heap i.e temp
    list<Node<T>*> temp;
 
    // inserting Binomial Tree into heap
    temp.push_back(tree);
 
    // perform union operation to finally insert
    // Binomial Tree in original heap
    temp = unionBionomialHeap(_heap,temp);
 
    return adjust(temp);
}
 
// removing minimum key element from binomial heap
// this function take Binomial Tree as input and return
// binomial heap after
// removing head of that tree i.e. minimum element
template <typename T>
list<Node<T>*> removeMinFromTreeReturnBHeap(Node<T> *tree)
{
    list<Node<T>*> heap;
    Node<T> *temp = tree->child;
    Node<T> *lo;
 
    // making a binomial heap from Binomial Tree
    while (temp)
    {
        lo = temp;
        temp = temp->sibling;
        lo->sibling = NULL;
        heap.push_front(lo);
    }
    return heap;
}
 
// inserting a key into the binomial heap
template <typename T>
list<Node<T>*> insert(list<Node<T>*> _head, const T& key)
{
    Node<T> *temp = newNode<T>(key);
    return insertATreeInHeap<T>(_head,temp);
}
 
// return pointer of minimum value Node
// present in the binomial heap
template <typename T>
Node<T>* getMin(list<Node<T>*> _heap)
{
    typename list<Node<T>*>::iterator it = _heap.begin();
    Node<T> *temp = *it;
    while (it != _heap.end())
    {
        if ((*it)->data < temp->data)
            temp = *it;
        it++;
    }
    return temp;
}
 
template <typename T>
list<Node<T>*> extractMin(list<Node<T>*> _heap)
{
    list<Node<T>*> new_heap,lo;
    Node<T> *temp;
 
    // temp contains the pointer of minimum value
    // element in heap
    temp = getMin(_heap);
    typename list<Node<T>*>::iterator it;
    it = _heap.begin();
    while (it != _heap.end())
    {
        if (*it != temp)
        {
            // inserting all Binomial Tree into new
            // binomial heap except the Binomial Tree
            // contains minimum element
            new_heap.push_back(*it);
        }
        it++;
    }
    lo = removeMinFromTreeReturnBHeap(temp);
    new_heap = unionBionomialHeap(new_heap,lo);
    new_heap = adjust(new_heap);
    return new_heap;
}
 
// print function for Binomial Tree
template <typename T>
void printTree(Node<T> *h)
{
    while (h)
    {
        cout << h->data << " ";
        printTree(h->child);
        h = h->sibling;
    }
}
 
// print function for binomial heap
template <typename T>
void printHeap(list<Node<T>*> _heap)
{
    typename list<Node<T>*>::iterator it = _heap.begin();
    while (it != _heap.end())
    {
        printTree(*it);
        it++;
    }
}
 
 
// Driver program to test above functions
typedef Node<uint64_t> IntNode;
typedef list<IntNode*> Heap;
//int main()
//{
//    int ch,key;
//    Heap _heap;
// 
//    // Insert data in the heap
//    _heap = insert<int>(_heap,10);
//    _heap = insert<int>(_heap,20);
//    _heap = insert<int>(_heap,30);
// 
//    cout << "Heap elements after insertion:\n";
//    printHeap(_heap);
// 
//    IntNode *temp = getMin(_heap);
//    cout << "\nMinimum element of heap "
//         << temp->data << "\n";
// 
//    // Delete minimum element of heap
//    _heap = extractMin<int>(_heap);
//    cout << "Heap after deletion of minimum element\n";
//    printHeap(_heap);
// 
//    return 0;
//}

//    _heap = insert<int>(_heap,30);
//    IntNode *temp = getMin(_heap);
//    _heap = extractMin<int>(_heap);

void run_BinomialHeap(uint8_t* seedIn, int seedSize) {
	Pair* arr=new Pair[N];
	fillPairArray(seedIn, seedSize, arr, N);
	//for(int j=0; j<N; j++) printf("init %d: %016lx %016lx\n",j,arr[j].a,arr[j].b);
    Heap _heap;
	int totCount=0;
	for(unsigned int i=0; i<1024&&i<N; i++) {
	    _heap = insert<uint64_t>(_heap,arr[i].a^arr[i].b);
		totCount++;
	}
	for(int j=0; j<ITER_COUNT; j++) {
		for(int i=0; i<N; i++) {
			uint64_t x=0;
		    IntNode *temp = getMin(_heap);
			if(temp!=NULL) {
			    _heap = extractMin<uint64_t>(_heap);
				x^=temp->data;
				//printf("haha %016lx\n",x);
				totCount--;
				if(temp->data%16==0) TRACER->meet(x);
			}
			if(x%2==0&&totCount<THRES) {
				_heap = insert<uint64_t>(_heap,arr[i].a);
				_heap = insert<uint64_t>(_heap,arr[i].b);
				totCount+=2;
			}
		}
	}
	delete[] arr;
}

#undef N
#undef THRES
#undef ITER_COUNT

#ifdef SELF_TEST
Tracer* TRACER;
int main() {
	TRACER=new Tracer;
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_BinomialHeap((uint8_t*)hello,len);
	}
	delete TRACER;
	return 0;
}
#endif
