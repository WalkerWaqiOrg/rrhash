#include <iostream>
#include <stack>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>
#include <cctype>
#include "util.h"
#include "Tracer.h"
using namespace std;

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
 
const int Count=76*1024;
template<int A, int B, int C, int D>
void run_RerverPolishNotation_help(int8_t* arr) {
	stack <int> s;
	
	int x=0;
	for(int i=0; i<Count; i++) {
		//cout<<" why: "<<int(arr[i])<<endl;
		if(i%256==0) {
			Tracer::I()->meet(x);
			//cout<<x<<endl;
		}
		if(s.size()<2) {
			s.push(arr[i]);
		}
		else {
			//cout<<" now: "<<s.size()<<" "<<s.top()<<" "<<int(arr[i])<<endl;
			int a,b;
			x=(x<<8)^(x>>8)^s.top();
			switch(arr[i]&3) {
			case A:
				b = s.top();
				s.pop();
				a = s.top();
				s.pop();
				s.push(a+b);
				break;	
			case B:
				b = s.top();
				s.pop();
				a = s.top();
				s.pop();
				s.push(a-b);
				break;	
			
			case C:
				b = s.top();
				s.pop();
				a = s.top();
				s.pop();
				s.push(a*b);
				break;	
		
			case D:
				b = s.top();
				s.pop();
				if (b != 0) {
					a = s.top();
					s.pop();
					s.push(a / b);
				}
				break;	
			}
		}
	}
}
void run_RerverPolishNotation(uint8_t* seedIn, int seedSize) {
	Pair* tmp=new Pair[Count/8];
	fillPairArray(seedIn, seedSize, tmp, Count/8);
	int8_t* arr=(int8_t*)tmp;
	run_RerverPolishNotation_help<0,1,2,3>(arr);
	run_RerverPolishNotation_help<3,0,1,2>(arr);
	run_RerverPolishNotation_help<2,3,0,1>(arr);
	run_RerverPolishNotation_help<1,2,3,0>(arr);

	run_RerverPolishNotation_help<3,2,1,0>(arr);
	run_RerverPolishNotation_help<0,3,2,1>(arr);
	run_RerverPolishNotation_help<1,0,3,2>(arr);
	run_RerverPolishNotation_help<2,1,0,3>(arr);

	run_RerverPolishNotation_help<1,0,2,3>(arr);
	run_RerverPolishNotation_help<3,1,0,2>(arr);
	run_RerverPolishNotation_help<2,3,1,0>(arr);
	run_RerverPolishNotation_help<0,2,3,1>(arr);

	run_RerverPolishNotation_help<1,2,0,3>(arr);
	run_RerverPolishNotation_help<3,1,2,0>(arr);
	run_RerverPolishNotation_help<0,3,1,2>(arr);
	run_RerverPolishNotation_help<2,0,3,1>(arr);

	run_RerverPolishNotation_help<3,1,2,0>(arr);
	run_RerverPolishNotation_help<0,3,1,2>(arr);
	run_RerverPolishNotation_help<2,0,3,1>(arr);
	run_RerverPolishNotation_help<1,2,0,3>(arr);

	run_RerverPolishNotation_help<3,0,2,1>(arr);
	run_RerverPolishNotation_help<1,3,0,2>(arr);
	run_RerverPolishNotation_help<2,1,3,0>(arr);
	run_RerverPolishNotation_help<0,2,1,3>(arr);
}

#ifdef SELF_TEST
int main() {
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_RerverPolishNotation((uint8_t*)hello,len);
	}
	return 0;
}
#endif

