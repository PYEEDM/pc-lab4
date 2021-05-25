all: seq omp mpi seqop ompop mpiop

seq: relax-sequential.c job-time.c
	gcc relax-sequential.c job-time.c -fopt-info-vec -o relax-seq.o 

omp: relax-omp.c job-time.c 
	gcc -fopenmp relax-omp.c job-time.c -fopt-info-vec -o relax-omp.o

mpi: relax-mpi.c job-time.c 
	mpicc -DMPI relax-mpi.c job-time.c -fopt-info-vec -o relax-mpi.o 

seqop: relax-sequential-vec.c job-time.c
	gcc relax-sequential-vec.c job-time.c -O3 -march=native -fopt-info-vec -o relax-seq-vec.o

ompop: relax-omp-vec.c job-time.c
	gcc -fopenmp relax-omp-vec.c job-time.c -O3 -march=native -fopt-info-vec -o relax-omp-vec.o

mpiop: relax-mpi-vec.c job-time.c 
	mpicc -DMPI relax-mpi-vec.c job-time.c -O3 -march=native -fopt-info-vec -o relax-mpi-vec.o 