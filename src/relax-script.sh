#!/usr/bin/env bash

#SBATCH -p csedu
echo "compiling..."

gcc relax-optimized-sequential.c job-time.c -O3 -march=native -fopt-info-vec -o relax-seq-op.o
gcc -fopenmp relax-optimized-omp.c job-time.c -O3 -march=native -fopt-info-vec -o relax-omp-op.o
mpicc -DMPI relax-optimized-mpi.c job-time.c -O3 -march=native -fopt-info-vec -o relax-mpi-op.o 

echo "!!!!!!!size of matrix!!!!!!!!"

n=100
while (( $n <= 35000 ))
do
	echo "matrix $n * $n."
    ./relax-seq-op.o $n 1
	./relax-omp-op.o $n 1 1
	mpirun relax-mpi-op.o $n 1 
	n=$(( 2*n ))	
done

echo "!!!!!!!iteration number!!!!!!!!"
itr=100
while (( $itr <= 35000 ))
do
	echo "iterations: $itr"
    ./relax-seq-op.o 64 $itr
	./relax-omp-op.o 64 $itr 1
	mpirun relax-mpi-op.o 64 $utr 
	itr=$(( 2*itr ))	
done