#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <stdint.h>
#include "util.h"
#include "Tracer.h"
using namespace std;

extern Tracer* TRACER;

static void fillDoubleArray(uint8_t* seedIn, int seedSize, double arr[], int n) {
	uint32_t* rand1024=new uint32_t[1024]; //1024 32-b values 
	expandRand4KB(seedIn, seedSize, (uint8_t*)rand1024);
	int counter=0;
	for(int i=0; i<1024; i++) {
		for(int j=0; j<1024; j++) {
			if(j==i) continue;
			arr[counter]=double(mulxor(rand1024[i],rand1024[j]));
			//cout<<"counter:"<<counter<<" val:"<<arr[counter]<<endl;
			counter++;
			if(counter==n) {
				delete[] rand1024;
				return;
			}
		}
	}
	assert(false);
}
 

void llsq ( int n, double x[], double y[], double &a, double &b )
//
//  Purpose:
//    LLSQ solves a linear least squares problem matching a line to data.
//    A formula for a line of the form Y = A * X + B is sought, which
//    will minimize the root-mean-square error to N data points ( X[I], Y[I] );
//  Parameters:
//    Input, int N, the number of data values.
//    Input, double X[N], Y[N], the coordinates of the data points.
//    Output, double &A, &B, the slope and Y-intercept of the least-squares
//    approximant to the data.
{
  double bot;
  int i;
  double top;
  double xbar;
  double ybar;
//
//  Special case.
//
  if ( n == 1 )
  {
    a = 0.0;
    b = y[0];
    return;
  }
//
//  Average X and Y.
//
  xbar = 0.0;
  ybar = 0.0;
  for ( i = 0; i < n; i++ )
  {
    xbar = xbar + x[i];
    ybar = ybar + y[i];
  }
  xbar = xbar / ( double ) n;
  ybar = ybar / ( double ) n;
//
//  Compute Beta.
//
  top = 0.0;
  bot = 0.0;
  for ( i = 0; i < n; i++ )
  {
    top = top + ( x[i] - xbar ) * ( y[i] - ybar );
    bot = bot + ( x[i] - xbar ) * ( x[i] - xbar );
  }
  a = top / bot;
  //for(int i=0; i<n; i++) cout<<x[i]<<" "<<y[i]<<" ";
  //cout<<endl<<"top:"<<top<<" bot:"<<bot<<" a:"<<a<<endl;

  b = ybar - a * xbar;
  //cout<<"ybar:"<<ybar<<" xbar:"<<xbar<<" b:"<<b<<endl;

  return;
}

//void test01 ( ) {
//  double a;
//  double b;
//  double error;
//  int i;
//  int n = 15;
//  double x[15] = { 
//    1.47, 1.50, 1.52, 1.55, 1.57, 1.60, 1.63, 1.65, 1.68, 1.70, 
//    1.73, 1.75, 1.78, 1.80, 1.83 };
//  double y[15] = {
//    52.21, 53.12, 54.48, 55.84, 57.20, 58.57, 59.93, 61.29, 63.11, 64.47,
//    66.28, 68.10, 69.92, 72.19, 74.46 };
//
//  cout << "\n";
//  cout << "TEST01\n";
//  cout << "  LLSQ can compute the formula for a line of the form\n";
//  cout << "    y = A * x + B\n";
//  cout << "  which minimizes the RMS error to a set of N data values.\n";
//
//  llsq ( n, x, y, a, b );
//
//  cout << "\n";
//  cout << "  Estimated relationship is y = " << a << " * x + " << b << "\n";
//  cout << "  Expected value is         y = 61.272 * x - 39.062\n";
//  cout << "\n";
//  cout << "     I      X       Y      B+A*X    |error|\n";
//  cout << "\n";
//  error = 0.0;
//  for ( i = 0; i < n; i++ )
//  {
//    cout << "  " << setw(4) << i
//         << "  " << setw(7) << x[i]
//         << "  " << setw(7) << y[i]
//         << "  " << setw(7) << b + a * x[i]
//         << "  " << setw(7) << b + a * x[i] - y[i] << "\n";
//    error = error + pow ( b + a * x[i] - y[i], 2 );
//  }
//  error = sqrt ( error / ( double ) n );
//  cout << "\n";
//  cout << "  RMS error =                      " << error << "\n";
//
//  return;
//}
//int main (void) {
//  cout << "\n";
//  cout << "LLSQ_PRB\n";
//  cout << "  C++ version\n";
//  cout << "  Test the LLSQ library.\n";
//
//  test01 ( );
//  cout << "\n";
//  cout << "LLSQ_PRB\n";
//  cout << "  Normal end of execution.\n";
//  cout << "\n";
//
//  return 0;
//}

void run_LeastSQ(uint8_t* seedIn, int seedSize) {
	const int Count=32*1024;
	double* arr=new double[Count];
	fillDoubleArray(seedIn, seedSize, arr, Count);
	const int step=256;
	for(int i=0; i<Count-step; i+=1) {
		double a,b;
		llsq(step, arr+i, arr+i+step, a, b);
		int idx=int(a*10000);
		arr[uint32_t(idx)%Count]=b;
		TRACER->meet(idx);
		//printf("result of %d is %d\n",i,idx);
	}
	delete[] arr;
}

#ifdef SELF_TEST
Tracer* TRACER;
int main() {
	TRACER=new Tracer;
	char hello[100]="aer39iqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_LeastSQ((uint8_t*)hello,len);
	}
	delete TRACER;
	return 0;
}
#endif
