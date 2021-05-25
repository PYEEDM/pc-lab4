#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <mpi.h>

#include <time.h>
#include "job-time.h"

#define PRINT_EXTRA_INFO true

//
// allocate a flattened matrix of "rxc" elements
//
double *allocMatrix(size_t n, size_t c)
{
	double *m;
	m = (double *)malloc(n*c*sizeof(double));
	if(m==NULL) {
		perror("failed to allocate matrix");
	}
	return m;
}

//
// initialise the values of the given matrix "out" of size "rxc" with 0s
//
void init(double *out, size_t r, size_t c)
{
	size_t i,j;

	for(i=0; i<r; i++) {
		for(j=0; j<c; j++) {
			out[i*c+j] = 0;
		}
	}
}

//
// print the values of a given matrix "out" of size "nxn"
//
void print(double *out, size_t n)
{
	size_t i,j,maxn;

	maxn = (n < 20 ? n : 20);
	for(i=0; i<maxn; i++) {
		printf("|");
		for(j=0; j<maxn; j++) {
			printf(" %7.2f", out[i*n+j]);
		}
		if(maxn < n) {
			printf("...|\n");
		} else {
			printf("|\n");
		}
	}
	if(maxn < n) {
		printf("...\n");
	}
}

//
// individual step of the 5-point stencil
// computes values in matrix "out" from those in matrix "in"
// assuming both are of size "nxn"
//
void relax(double *in, double *out, size_t n, int displ, int count)
{
	size_t i,j;
	size_t last_i = count/n < n-1 ? count/n : n-1;
	for(i = 1; i<=last_i; i++) { 
		for(j=1; j<n-1; j++) {
				out[i*n+j] = 0.25*in[(i-1)*n+j]           // upper neighbour
							 + 0.25*in[i*n+j]             // center
							 + 0.125*in[(i+1)*n+j]        // lower neighbour
							 + 0.175*in[i*n+(j-1)]        // left neighbour
							 + 0.2*in[i*n+(j+1)];         // right neighbour
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

	double *master, *in, *out, *tmp;
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

	// initialize slicing setup
	int elements = n*n;
	int rows_per_rank = (n-2 + num_ranks - 1)/num_ranks;
	int elements_per_rank = rows_per_rank * n;
	int *out_counts = (int *)malloc(num_ranks*sizeof(int));
	int *out_displs = (int *)malloc(num_ranks*sizeof(int));
	for (int j = 0; j < num_ranks; j++)
	{
		out_displs[j] = j*elements_per_rank+n;
		out_counts[j] = j < num_ranks - 1 ? elements_per_rank : elements - n - out_displs[j];
	}

	in = allocMatrix(rows_per_rank+2, n);
	out = allocMatrix(rows_per_rank+2, n);
	init(in, rows_per_rank+2, n);
	init(out, rows_per_rank+2, n);
	if (my_rank == 0)
	{
		master = allocMatrix(n, n);
		init(master, n, n);
		master[n/4] = 100.0;
		master[(n*3)/4] = 1000.0;
		in[n/4] = 100.0;
		in[(n*3)/4] = 1000.0;
		out[n/4] = 100.0;
		out[(n*3)/4] = 1000.0;
	}
	else
	{
		// Workaround because MPI doesn't like passing an unallocated array to Gatherv even on non-root ranks that don't need it...
		master = (double*) malloc(sizeof(double));
	}

	if (my_rank == 0)
	{  
		if(PRINT_EXTRA_INFO)
		{
			printf("size    : n = %zu => %d M elements (%d MB)\n",
						n, (int)(n*n/1000000), (int)(n*n*sizeof(double) / (1024*1024)));
			printf("iter    : %d\n", max_iter);

			print(master, n);
		}
		
		// start time measurement
		start = time_gettime();
	}

	// execute the algorithm
	for(i=0; i<max_iter; i++) { 

		relax(in, out, n, out_displs[my_rank], out_counts[my_rank]);

		tmp = in;
		in = out;
		out = tmp;

		// send relevant rows to relevant ranks
		if (i < max_iter - 1)
		{
			if (my_rank > 0)
			{
				MPI_Recv(&(in[0]), n, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(&(in[0])+n, n, MPI_DOUBLE, my_rank-1, 0, MPI_COMM_WORLD);
			}
			if (my_rank < num_ranks - 1)
			{
				MPI_Send(&(in[0])+elements_per_rank, n, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD);
				MPI_Recv(&(in[0])+elements_per_rank+n, n, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	}

	// collect the resulting matrix
	MPI_Gatherv(&(in[0])+n, out_counts[my_rank], MPI_DOUBLE, &(master[0]), out_counts, out_displs,MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (my_rank == 0)
	{
		// end time measurement
		finish = time_gettime();

		if (PRINT_EXTRA_INFO)
		{
			printf("Matrix after %d iterations:\n", i);
			print(master, n);
		}

		time_print_elapsed(__FILE__, start, finish);
	}
 
	MPI_Finalize();

	return 0;
}
