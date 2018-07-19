/*
 * C++ Program to Implement Red Black Tree
 */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <cassert>
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
 
#define INDENT_STEP  4
using namespace std;
enum color { RED, BLACK };
/*
 * Node RBTree Declaration
 */

typedef int KeyType;
typedef int ValueType;

typedef struct rbtree_node
{
    enum color color;
    KeyType key;
    ValueType value;
    rbtree_node *left, *right, *parent;
}*node;
 
typedef struct rbtree_t
{
    node root;
}*rbtree;
 
/*
 * Compare two nodes
 */
int compare_int(int leftp, int rightp)
{
    int left = (int)leftp;
    int right = (int)rightp;
    if (left < right)
        return -1;
    else if (left > right)
        return 1;
    else
    {
        assert (left == right);
        return 0;
    }
}
/*
 * Class RBTree Declaration
 */
class RBTree
{
    public:
        rbtree rbtree_create();
        ValueType rbtree_lookup(rbtree t, KeyType, bool* foundIt);
        void rbtree_insert(rbtree t, KeyType , ValueType);
        void rbtree_delete(rbtree t, KeyType);
        node grandparent(node n);
        node sibling(node n);
        node uncle(node n);
        void verify_properties(rbtree t);
        void verify_property_1(node root);
        void verify_property_2(node root);
        color node_color(node n);
        void verify_property_4(node root);
        void verify_property_5(node root);
        void verify_property_5_helper(node n, int , int*);
        node new_node(KeyType key, ValueType , color , node , node);
        node lookup_node(rbtree t, KeyType);
        void rotate_left(rbtree t, node n);
        void rotate_right(rbtree t, node n);
        void replace_node(rbtree t, node oldn, node newn);
        void insert_case1(rbtree t, node n);
        void insert_case2(rbtree t, node n);
        void insert_case3(rbtree t, node n);
        void insert_case4(rbtree t, node n);
        void insert_case5(rbtree t, node n);
        node maximum_node(node root);
        void delete_case1(rbtree t, node n);
        void delete_case2(rbtree t, node n);
        void delete_case3(rbtree t, node n);
        void delete_case4(rbtree t, node n);
        void delete_case5(rbtree t, node n);
        void delete_case6(rbtree t, node n);
};
/*
 * Return Grandparent of Node 
 */
node RBTree::grandparent(node n)
{
    assert (n != NULL);
    assert (n->parent != NULL);
    assert (n->parent->parent != NULL);
    return n->parent->parent;
}
 
/*
 * Return Sibling of Node 
 */
node RBTree::sibling(node n)
{
    assert (n != NULL);
    assert (n->parent != NULL);
    if (n == n->parent->left)
        return n->parent->right;
    else
        return n->parent->left;
}
 
/*
 * Return Uncle of Node 
 */
node RBTree::uncle(node n)
{
    assert (n != NULL);
    assert (n->parent != NULL);
    assert (n->parent->parent != NULL);
    return sibling(n->parent);
}
 
/*
 * Verifying Properties of Red black Tree
 */
void RBTree::verify_properties(rbtree t)
{
    verify_property_1 (t->root);
    verify_property_2 (t->root);
    verify_property_4 (t->root);
    verify_property_5 (t->root);
}
/*
 * Verifying Property 1
 */
void RBTree::verify_property_1(node n)
{
    assert (node_color(n) == RED || node_color(n) == BLACK);
    if (n == NULL)
        return;
    verify_property_1(n->left);
    verify_property_1(n->right);
}
/*
 * Verifying Property 2
 */
void RBTree::verify_property_2(node root)
{
    assert (node_color(root) == BLACK);
}
/*
 * Returns color of a node
 */
color RBTree::node_color(node n)
{
    return n == NULL ? BLACK : n->color;
}
/*
 * Verifying Property 4
 */
void RBTree::verify_property_4(node n)
{
    if (node_color(n) == RED)
    {
        assert (node_color(n->left) == BLACK);
        assert (node_color(n->right) == BLACK);
        assert (node_color(n->parent) == BLACK);
    }
    if (n == NULL)
        return;
    verify_property_4(n->left);
    verify_property_4(n->right);
}
/*
 * Verifying Property 5
 */
void RBTree::verify_property_5(node root)
{
    int black_count_path = -1;
    verify_property_5_helper(root, 0, &black_count_path);
}
 
void RBTree::verify_property_5_helper(node n, int black_count, int* path_black_count)
{
    if (node_color(n) == BLACK)
    {
        black_count++;
    }
    if (n == NULL)
    {
        if (*path_black_count == -1)
        {
            *path_black_count = black_count;
        }
        else
        {
            assert (black_count == *path_black_count);
        }
        return;
    }
    verify_property_5_helper(n->left,  black_count, path_black_count);
    verify_property_5_helper(n->right, black_count, path_black_count);
}
 
/*
 * Create Red Black Tree 
 */
rbtree RBTree::rbtree_create()
{
    rbtree t = new rbtree_t;
    t->root = NULL;
    verify_properties(t);
    return t;
}
 
/*
 * Creating New Node of Reb Black Tree
 */
node RBTree::new_node(KeyType k, ValueType v, color n_color, node left, node right)
{
    node result = new rbtree_node;
    result->key = k;
    result->value = v;
    result->color = n_color;
    result->left = left;
    result->right = right;
    if (left  != NULL)
        left->parent = result;
    if (right != NULL)
        right->parent = result;
    result->parent = NULL;
    return result;
}
/*
 * Look Up through Node
 */
node RBTree::lookup_node(rbtree t, KeyType key)
{
    node n = t->root;
    while (n != NULL)
    {
        int comp_result = compare_int(key, n->key);
        if (comp_result == 0)
        {
            return n;
        }
        else if (comp_result < 0)
        {
            n = n->left;
        }
        else
        {
            assert(comp_result > 0);
            n = n->right;
        }
    }
    return n;
}
/*
 * RbTree Look Up
 */
ValueType RBTree::rbtree_lookup(rbtree t, KeyType key, bool* foundIt)
{
    node n = lookup_node(t, key);
    *foundIt=(n != NULL );
	if(*foundIt) return n->value;
	else return 0;
}
 
/*
 * Rotate left
 */
void RBTree::rotate_left(rbtree t, node n)
{
    node r = n->right;
    replace_node(t, n, r);
    n->right = r->left;
    if (r->left != NULL)
    {
        r->left->parent = n;
    }
    r->left = n;
    n->parent = r;
}
/*
 * Rotate right
 */
void RBTree::rotate_right(rbtree t, node n)
{
    node L = n->left;
    replace_node(t, n, L);
    n->left = L->right;
    if (L->right != NULL)
    {
        L->right->parent = n;
    }
    L->right = n;
    n->parent = L;
}
/*
 * Replace a node
 */
void RBTree::replace_node(rbtree t, node oldn, node newn)
{
    if (oldn->parent == NULL)
    {
        t->root = newn;
    }
    else
    {
        if (oldn == oldn->parent->left)
            oldn->parent->left = newn;
        else
            oldn->parent->right = newn;
    }
    if (newn != NULL)
    {
        newn->parent = oldn->parent;
    }
}
/*
 * Insert node into RBTree
 */
void RBTree::rbtree_insert(rbtree t, KeyType key, ValueType value)
{
    node inserted_node = new_node(key, value, RED, NULL, NULL);
    if (t->root == NULL)
    {
        t->root = inserted_node;
    }
    else
    {
        node n = t->root;
        while (1)
        {
            int comp_result = compare_int(key, n->key);
            if (comp_result == 0)
            {
                n->value = value;
                return;
            }
            else if (comp_result < 0)
            {
                if (n->left == NULL)
                {
                    n->left = inserted_node;
                    break;
                }
                else
                {
                    n = n->left;
                }
            }
            else
            {
                assert (comp_result > 0);
                if (n->right == NULL)
                {
                    n->right = inserted_node;
                    break;
                }
                else
                {
                    n = n->right;
                }
            }
        }
        inserted_node->parent = n;
    }
    insert_case1(t, inserted_node);
    verify_properties(t);
}
 
/*
 * Inserting Case 1
 */
void RBTree::insert_case1(rbtree t, node n)
{
    if (n->parent == NULL)
        n->color = BLACK;
    else
        insert_case2(t, n);
}
 
/*
 * Inserting Case 2
 */
void RBTree::insert_case2(rbtree t, node n)
{
    if (node_color(n->parent) == BLACK)
        return;
    else
        insert_case3(t, n);
}
 
/*
 * Inserting Case 3
 */
void RBTree::insert_case3(rbtree t, node n)
{
    if (node_color(uncle(n)) == RED)
    {
        n->parent->color = BLACK;
        uncle(n)->color = BLACK;
        grandparent(n)->color = RED;
        insert_case1(t, grandparent(n));
    }
    else
    {
        insert_case4(t, n);
    }
}
 
/*
 * Inserting Case 4
 */
void RBTree::insert_case4(rbtree t, node n)
{
    if (n == n->parent->right && n->parent == grandparent(n)->left)
    {
        rotate_left(t, n->parent);
        n = n->left;
    }
    else if (n == n->parent->left && n->parent == grandparent(n)->right)
    {
        rotate_right(t, n->parent);
        n = n->right;
    }
    insert_case5(t, n);
}
 
/*
 * Inserting Case 5
 */
void RBTree::insert_case5(rbtree t, node n)
{
    n->parent->color = BLACK;
    grandparent(n)->color = RED;
    if (n == n->parent->left && n->parent == grandparent(n)->left)
    {
        rotate_right(t, grandparent(n));
    }
    else
    {
        assert (n == n->parent->right && n->parent == grandparent(n)->right);
        rotate_left(t, grandparent(n));
    }
}
 
/*
 * Delete Node from RBTree
 */
void RBTree::rbtree_delete(rbtree t, KeyType key)
{
    node child;
    node n = lookup_node(t, key);
    if (n == NULL)
        return;
    if (n->left != NULL && n->right != NULL)
    {
        node pred = maximum_node(n->left);
        n->key   = pred->key;
        n->value = pred->value;
        n = pred;
    }
    assert(n->left == NULL || n->right == NULL);
    child = n->right == NULL ? n->left  : n->right;
    if (node_color(n) == BLACK)
    {
        n->color = node_color(child);
		TRACER->meet(n->value);
        delete_case1(t, n);
    }
    replace_node(t, n, child);
    free(n);
    verify_properties(t);
}
 
/*
 * Returns Maximum node
 */
node RBTree::maximum_node(node n)
{
    assert (n != NULL);
    while (n->right != NULL)
    {
        n = n->right;
    }
    return n;
}
 
/*
 * Deleting Case 1
 */
void RBTree::delete_case1(rbtree t, node n)
{
    if (n->parent == NULL)
        return;
    else
        delete_case2(t, n);
}
 
/*
 * Deleting Case 2
 */
void RBTree::delete_case2(rbtree t, node n)
{
    if (node_color(sibling(n)) == RED)
    {
        n->parent->color = RED;
        sibling(n)->color = BLACK;
        if (n == n->parent->left)
            rotate_left(t, n->parent);
        else
            rotate_right(t, n->parent);
    }
    delete_case3(t, n);
}
 
/*
 * Deleting Case 3
 */
void RBTree::delete_case3(rbtree t, node n)
{
    if (node_color(n->parent) == BLACK && node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK && node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        delete_case1(t, n->parent);
    }
    else
        delete_case4(t, n);
}
 
/*
 * Deleting Case 4
 */
void RBTree::delete_case4(rbtree t, node n)
{
    if (node_color(n->parent) == RED && node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == BLACK && node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        n->parent->color = BLACK;
    }
    else
        delete_case5(t, n);
}
 
/*
 * Deleting Case 5
 */
void RBTree::delete_case5(rbtree t, node n)
{
    if (n == n->parent->left && node_color(sibling(n)) == BLACK &&
        node_color(sibling(n)->left) == RED && node_color(sibling(n)->right) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->left->color = BLACK;
        rotate_right(t, sibling(n));
    }
    else if (n == n->parent->right && node_color(sibling(n)) == BLACK &&
             node_color(sibling(n)->right) == RED && node_color(sibling(n)->left) == BLACK)
    {
        sibling(n)->color = RED;
        sibling(n)->right->color = BLACK;
        rotate_left(t, sibling(n));
    }
    delete_case6(t, n);
}
 
/*
 * Deleting Case 6
 */
void RBTree::delete_case6(rbtree t, node n)
{
    sibling(n)->color = node_color(n->parent);
    n->parent->color = BLACK;
    if (n == n->parent->left)
    {
        assert (node_color(sibling(n)->right) == RED);
        sibling(n)->right->color = BLACK;
        rotate_left(t, n->parent);
    }
    else
    {
        assert (node_color(sibling(n)->left) == RED);
        sibling(n)->left->color = BLACK;
        rotate_right(t, n->parent);
    }
}
 
/*
 * Print RBTRee
 */
void print_tree_helper(node n, int indent)
{
    int i;
    if (n == NULL)
    {
        fputs("<empty tree>", stdout);
        return;
    }
    if (n->right != NULL)
    {
        print_tree_helper(n->right, indent + INDENT_STEP);
    }
    for(i = 0; i < indent; i++)
        fputs(" ", stdout);
    //if (n->color == BLACK)
    //    cout<<(int)n->key<<endl;
    //else
    //    cout<<"<"<<(int)n->key<<">"<<endl;
    if (n->left != NULL)
    {
        print_tree_helper(n->left, indent + INDENT_STEP);
    }
}
 
void print_tree(rbtree t)
{
    print_tree_helper(t->root, 0);
    puts("");
}
 
/*
 * Main Contains Menu
 */
//int main()
//{
//    int i;
//    RBTree rbt;
//    rbtree t = rbt.rbtree_create();
//    for (i = 0; i < 12; i++)
//    {
//        int x = rand() % 10;
//        int y = rand() % 10;
//        print_tree(t);
//        cout<<"Inserting "<<x<<" -> "<<y<<endl<<endl;
//        rbt.rbtree_insert(t, x, y);
//		bool foundIt=false;
//        ValueType res=rbt.rbtree_lookup(t, x, &foundIt);
//        assert(foundIt && res == y);
//    }
//    for (i = 0; i < 15; i++)
//    {
//        int x = rand() % 10;
//        print_tree(t);
//        cout<<"Deleting key "<<x<<endl<<endl;
//        rbt.rbtree_delete(t, x);
//    }
//    return 0;
//}

void run_RedBlackTree(uint8_t* seedIn, int seedSize) {
	const int Count=3*1024;
	Pair* tmp=new Pair[Count/4];
	fillPairArray(seedIn, seedSize, tmp, Count/4);
	int16_t* arr=(int16_t*)tmp;
    RBTree rbt;
    rbtree t = rbt.rbtree_create();
	for(int i=0; i<Count; i+=2) {
		bool foundIt=false;
		ValueType res=rbt.rbtree_lookup(t, arr[i], &foundIt);
		if(foundIt) {
			//cout<<"found "<<arr[i]<<endl;
			TRACER->meet(res);
	        rbt.rbtree_delete(t, arr[i]);
		}
		else {
	        rbt.rbtree_insert(t, arr[i], arr[i+1]);
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
		run_RedBlackTree((uint8_t*)hello,len);
	}
	delete TRACER;
	return 0;
}
#endif
