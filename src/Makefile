all: seq omp mpi

seq: relax-fix-original.c job-time.c
	gcc relax-fix-original.c job-time.c -O3 -march=native -fopt-info-vec -o relax-seq.o 

omp: relax-unoptimized-omp.c job-time.c 
	gcc -fopenmp relax-unoptimized-omp.c job-time.c -O3 -march=native -fopt-info-vec -o relax-omp.o

mpi: relax-unoptimized-mpi.c job-time.c 
	mpicc -DMPI relax-unoptimized-mpi.c job-time.c -O3 -march=native -fopt-info-vec -o relax-mpi.o 
