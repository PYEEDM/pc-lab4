all: seq omp mpi seqop ompop mpiop

seq: relax-unoptimized-sequential.c job-time.c
	gcc relax-unoptimized-sequential.c job-time.c -O3 -march=native -fopt-info-vec -o relax-seq.o 

omp: relax-unoptimized-omp.c job-time.c 
	gcc -fopenmp relax-unoptimized-omp.c job-time.c -O3 -march=native -fopt-info-vec -o relax-omp.o

mpi: relax-unoptimized-mpi.c job-time.c 
	mpicc -DMPI relax-unoptimized-mpi.c job-time.c -O3 -march=native -fopt-info-vec -o relax-mpi.o 

seqop: relax-optimized-sequential.c job-time.c
	gcc relax-optimized-sequential.c job-time.c -O3 -march=native -fopt-info-vec -o relax-seq-op.o

ompop: relax-optimized-omp.c job-time.c
	gcc -fopenmp relax-optimized-omp.c job-time.c -O3 -march=native -fopt-info-vec -o relax-omp-op.o

mpiop: relax-unoptimized-mpi.c job-time.c 
	mpicc -DMPI relax-optimized-mpi.c job-time.c -O3 -march=native -fopt-info-vec -o relax-mpi-op.o 