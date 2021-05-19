#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

#include <time.h>
#include "job-time.h"

double **allocMatrixVectorized(size_t n)
{
    double ** m = (double**) malloc(n * sizeof(double*));
    int i;
    for(i = 0;i < n; i++)
    {
        m[i] = (double*) malloc(n * sizeof(double));
    }
    if(!m) perror("failed to allocate matrix; ");
    return m;
}

//
// initialise the values of the given matrix "out" of size "nxn" with 0s
//
void init( double **out, size_t n)
{
    size_t i,j;
    
    for( i=0; i<n; i++) {
        for( j=0; j<n; j++) {
            out[i][j] = 0;
        }
    }   
}

//
// print the values of a given matrix "out" of size "nxn"
//
void print( double **out, size_t n)
{
    size_t i,j,maxn;
    
    maxn = (n < 20 ? n : 20);
    for( i=0; i<maxn; i++) {
        printf( "|");
        for( j=0; j<maxn; j++) {
            printf( " %7.2f",out[i][j]);
        }
        if( maxn < n) {
            printf( "...|\n");
        } else {
            printf( "|\n");
        }
    }
    if( maxn < n) {
        printf( "...\n");
    }
}

//
// individual step of the 5-point stencil
// computes values in matrix "out" from those in matrix "in"
// assuming both are of size "nxn"
//
void relax( double **in, double **out, size_t n)
{
    size_t i,j;

   #pragma omp parallel for collapse(2)
    for( i=1; i<n-1; i++) {
        for( j=1; j<n-1; j++) {
            out[i][j] = 0.25*in[(i-1)][j]      // upper neighbour
            + 0.25*in[i][j]        // center
            + 0.125*in[(i+1)][j]   // lower neighbour
            + 0.175*in[i][(j-1)]   // left neighbour
            + 0.2*in[i][(j+1)];    // right neighbour
        }
    }
}

int main (int argc, char *argv[])
{
    double **a,**b, **tmp;
    size_t n=0;
    int i;
   int max_iter, num_threads = 1;

   if( argc < 4) {
      printf("call should have two arguments \"%s <n> <iter>\"\n", argv[0]);
      exit(1);
   }
   if( sscanf( argv[1], "%zu", &n) != 1) {
      printf("non size_t value for matrix size\n");
      exit(1);
   }

   if( sscanf( argv[2], "%d", &max_iter) != 1) {
      printf("non int value for # iterations\n");
      exit(1);
   }
   if(sscanf(argv[3], "%d", &num_threads) != 1) {
      printf("specify number of threads\n");
      exit(1);
   }
    
//       a = allocMatrix( n);
//       b = allocMatrix( n);
    
    a = allocMatrixVectorized(n);
    b = allocMatrixVectorized(n);

   omp_set_num_threads(num_threads);

    init( a, n);
    init( b, n);
    
    a[0][n/4] = 100.0;
    b[0][n/4] = 100.0;
    
    a[0][(n*3)/4] = 1000.0;
    b[0][(n*3)/4] = 1000.0;
    
    printf( "size   : n = %zu => %d M elements (%d MB)\n",
           n, (int)(n*n/1000000), (int)(n*n*sizeof(double) / (1024*1024)));
    printf( "iter   : %d\n", max_iter);
    
    print(a, n);
    
   struct timespec start, finish;
   start = time_gettime();

    for( i=0; i<max_iter; i++) {
        tmp = a;
        a = b;
        b = tmp;
        relax( a, b, n);
    }

   finish = time_gettime();
    
    printf( "Matrix after %d iterations:\n", i);
    print( b, n);
    
   time_print_elapsed(__FILE__, start, finish);

    return 0;
}
