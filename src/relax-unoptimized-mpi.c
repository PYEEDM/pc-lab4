#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>

#include <time.h>
#include "job-time.h"

//
// allocate an array of n elements
//
double *allocArray( size_t n)
{
   double *m;
   m = (double *)malloc( n*sizeof(double));
   if( m==NULL) {
      perror( "failed to allocate matrix; ");
   }
   return m;
}

//
// initialise the values of the given array "out" of length n with 0s
//
void init( double *out, size_t n)
{
   size_t i,j;

   for( i=0; i<n; i++) {
      out[i] = 0;
   }

}

//
// print the values of a given matrix "out" of size "nxn"
//
void print( double *out, size_t n)
{
   size_t i,j,maxn;

   maxn = (n < 20 ? n : 20);
   for( i=0; i<maxn; i++) {
      printf( "|");
      for( j=0; j<maxn; j++) {
         printf( " %7.2f", out[i*n+j]);
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
void relax( double *in, double *out, size_t n, int start_index, int length)
{
   size_t i,j;
   //TODO: if this ain't the messiest possible code to do this then I don't know what is
   size_t init_i = start_index/n, last_i = (start_index+length)/n, init_j = start_index%n, last_j = n-1;
   init_i = init_i > 0 ? init_i : 1;
   last_i = last_i < n-1 ? last_i : n-1;
   for( i=init_i; i<last_i; i++) {
      init_j = i == init_i ? init_j : 1;
      last_j = i < last_i - 1 ? last_j : (start_index+length)%n;
      //TODO: until here ^
      for( j=init_j; j<last_j; j++) {
         out[i*n+j] = 0.25*in[(i-1)*n+j]      // upper neighbour
                      + 0.25*in[i*n+j]        // center
                      + 0.125*in[(i+1)*n+j]   // lower neighbour
                      + 0.175*in[i*n+(j-1)]   // left neighbour
                      + 0.2*in[i*n+(j+1)];    // right neighbour
      }
   }
}

int main (int argc, char *argv[])
{
   int my_rank, num_ranks;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

   double *a,*b;
   size_t n=0;
   int i;
   int max_iter;

   if (my_rank == 0)
   {
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
   }

   MPI_Bcast(&n, 1, MPI_UINT64_T, 0, MPI_COMM_WORLD);
   MPI_Bcast(&max_iter, 1, MPI_INT, 0, MPI_COMM_WORLD);

   int elements = n*n;
   int elements_per_rank = (elements + num_ranks - 1)/num_ranks;
   int my_start = my_rank * elements_per_rank;

   int *lengths = (int *)malloc( num_ranks*sizeof(int));
   int *displs = (int *)malloc( num_ranks*sizeof(int));
   for (int j = 0; j < num_ranks; j++)
   {
      lengths[j] = (j < num_ranks-1 || (elements % elements_per_rank) == 0) ? elements_per_rank : elements % elements_per_rank;
      displs[j] = j*elements_per_rank;
   }

   int my_length = lengths[my_rank];

   a = allocArray( elements);
   b = allocArray( elements_per_rank);

   init( a, elements);
   init( b, elements_per_rank);

   a[n/4] = 100.0;
   a[(n*3)/4] = 1000.0;

   if (my_rank == 0)
   {
      printf( "size   : n = %zu => %d M elements (%d MB)\n",
              n, (int)(n*n/1000000), (int)(n*n*sizeof(double) / (1024*1024)));
      printf( "iter   : %d\n", max_iter);

      print(a, n);
   }

   struct timespec start, finish;
   start = time_gettime();

   for( i=0; i<max_iter; i++) {  

      if (my_rank == 0)
      {
         for (int r = 1; r < num_ranks; r++)
         {
            MPI_Send(a+displs[r], lengths[r]+(r < num_ranks - 1 ? n : 0), MPI_DOUBLE, r, 0, MPI_COMM_WORLD);
         }
      }
      else
      {
         MPI_Recv(a+displs[my_rank], lengths[my_rank]+(my_rank < num_ranks - 1 ? n : 0), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      }

      relax( a, b, n, my_start, my_length);

      MPI_Gatherv(b, my_length, MPI_DOUBLE, a, lengths, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      a[n/4] = 100.0;
      a[(n*3)/4] = 1000.0;
   }

   if (my_rank == 0)
   {
      finish = time_gettime();
      printf( "Matrix after %d iterations:\n", i);
      print( a, n); 
      time_print_elapsed(__FILE__, start, finish);
   }
 
   MPI_Finalize();

   return 0;
}
