#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>

#include <time.h>
#include "job-time.h"

double **allocMatrixVectorized(size_t n)
{
    double * data = (double*) malloc(n*n*sizeof(double*));
    double ** m = (double**) malloc(n * sizeof(double*));
    int i;
    for(i = 0;i < n; i++)
    {
        m[i] = &(data[n*i]);
    }
    if(!m) perror("failed to allocate matrix; ");
    return m;
}

//
// initialise the values of the given matrix "out" of size "nxn" with 0s
//
void init(double **out, size_t n)
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
void relax(double **in, double **out, size_t n, int displ, int count)
{
   size_t i,j,init_i,init_j;
   init_i = displ/n > 0 ? displ/n : 1;
   for(i = init_i; i<n-1; i++) {
      init_j = i == 0 && displ%n > 0 ? displ%n : 1; 
      for(j=init_j; j<n-1; j++) {
         int this_index = i*n+j;
         if (this_index >= displ + count)
         {
            return;
         }
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
   struct timespec start, finish;
   int my_rank, num_ranks;

   // MPI initialization
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

   double **a,**b;
   size_t n=0;
   int i;
   int max_iter;

   if (my_rank == 0)
   {
      if(argc < 3) {
         printf("call should have two arguments \"%s <n> <iter>\"\n", argv[0]);
         exit(1);
      }
      if(sscanf(argv[1], "%zu", &n) != 1) {
         printf("non size_t value for matrix size\n");
         exit(1);
      }
  
      if(sscanf(argv[2], "%d", &max_iter) != 1) {
         printf("non int value for # iterations\n");
         exit(1);
      }
   }

   // broadcast arguments
   MPI_Bcast(&n, 1, MPI_UINT64_T, 0, MPI_COMM_WORLD);
   MPI_Bcast(&max_iter, 1, MPI_INT, 0, MPI_COMM_WORLD);

   int elements = n*n;
   int elements_per_rank = (elements + num_ranks - 1)/num_ranks;

   // initialize slicing setup
   int *in_counts = (int *)malloc(num_ranks*sizeof(int));
   int *in_displs = (int *)malloc(num_ranks*sizeof(int));
   int *out_counts = (int *)malloc(num_ranks*sizeof(int));
   int *out_displs = (int *)malloc(num_ranks*sizeof(int));
   for (int j = 0; j < num_ranks; j++)
   {
      in_displs[j] = j > 0 ? j*elements_per_rank - n : j*elements_per_rank;   
      in_counts[j] = j < num_ranks-1 ? elements_per_rank + (j > 0 ? 2 : 1) * n : elements_per_rank;
      out_displs[j] = j*elements_per_rank;
      out_counts[j] = (j < num_ranks-1 || (elements % elements_per_rank) == 0) ? elements_per_rank : elements % elements_per_rank;
   }

   // allocate and initialize matrices
   a = allocMatrixVectorized(n);
   b = allocMatrixVectorized(n);

   init(a, n);
   init(b, n);

   // add initial data
   b[0][n/4] = 100.0;
   b[0][(n*3)/4] = 1000.0;

   if (my_rank == 0)
   {  
      printf("size   : n = %zu => %d M elements (%d MB)\n",
              n, (int)(n*n/1000000), (int)(n*n*sizeof(double) / (1024*1024)));
      printf("iter   : %d\n", max_iter);

      print(b, n);
   }

   // start time measurement
   start = time_gettime();

   // execute the algorithm
   for(i=0; i<max_iter; i++) {  
      MPI_Scatterv(&(b[0][0]), in_counts, in_displs, MPI_DOUBLE, &(a[0][0]) + in_displs[my_rank], in_counts[my_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

      relax(a, b, n, out_displs[my_rank], out_counts[my_rank]);

      MPI_Gatherv(&(b[0][0])+ out_displs[my_rank], out_counts[my_rank], MPI_DOUBLE, &(b[0][0]), out_counts, out_displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   }

   if (my_rank == 0)
   {
      // end time measurement
      finish = time_gettime();

      printf("Matrix after %d iterations:\n", i);
      print(b, n); 

      time_print_elapsed(__FILE__, start, finish);
   }
 
   MPI_Finalize();

   return 0;
}
