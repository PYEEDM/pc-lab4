#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>
#include "job-time.h"
#include <time.h>


//
// allocate a a flattened matrix of "nxn" elements
//
double *allocMatrix( size_t n)
{
   double *m;
   m = (double *)malloc( n*n*sizeof(double));
   if( m==NULL) {
      perror( "failed to allocate matrix; ");
   }
   return m;
}

//
// initialise the values of the given matrix "out" of size "nxn" with 0s
//
void init( double *out, size_t n)
{
   size_t i,j;

   #pragma omp parallel for collapse(2)
   for( i=0; i<n; i++) {
      for( j=0; j<n; j++) {
         out[i*n+j] = 0;
      }
   }
}

//
// print the values of a given matrix "out" of size "nxn"
//
void print( double *out, size_t n)
{
   size_t i,j,maxn;

   maxn = (n < 20 ? n : 20);

   // this might not be doing anything, might still be sequential ?
   #pragma omp parallel for ordered
   for( i=0; i<maxn; i++) {
      #pragma omp ordered
      printf( "|");
      for( j=0; j<maxn; j++) {
         #pragma omp ordered
         printf( " %7.2f", out[i*n+j]);
      }
      if( maxn < n) {
         #pragma omp ordered
         printf( "...|\n");
      } else {
         #pragma omp ordered
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
void relax( double *in, double *out, size_t n)
{
   size_t i,j;

   #pragma omp for collapse(2)
   for( i=1; i<n-1; i++) {
      for( j=1; j<n-1; j++) {
         #pragma omp critical
         out[i*n+j] = 0.25*in[(i-1)*n+j] + 0.25*in[i*n+j] + 0.125*in[(i+1)*n+j] + 0.175*in[i*n+(j-1)] + 0.2*in[i*n+(j+1)];
      }
   }
}

int main (int argc, char *argv[])
{
   double *a,*b, *tmp;
   size_t n=0;
   int i;
   int max_iter;

   if( argc < 3) {
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

   a = allocMatrix( n);
   b = allocMatrix( n);

   omp_set_num_threads(2); // todo: make it not hard coded

   init( a, n);
   init( b, n);

   a[n/4] = 100.0;;
   b[n/4] = 100.0;;

   a[(n*3)/4] = 1000.0;;
   b[(n*3)/4] = 1000.0;;

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

   printf( "Matrix after %d iterations:\n", i);
   print( b, n);
   finish = time_gettime();
   time_print_elapsed(__FILE__, start, finish);

   return 0;
}
