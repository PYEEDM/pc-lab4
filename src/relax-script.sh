#!/usr/bin/env bash

#SBATCH -p csedu
echo "compiling..."

gcc relax-sequential-vec.c job-time.c -O3 -march=native -fopt-info-vec -o relax-seq-vec.o
gcc -fopenmp relax-omp-vec.c job-time.c -O3 -march=native -fopt-info-vec -o relax-omp-vec.o
mpicc -DMPI relax-mpi-vec.c job-time.c -O3 -march=native -fopt-info-vec -o relax-mpi-vec.o 
echo "!!!!!!!size of matrix!!!!!!!!"

n=256
while (( $n <= 35000 ))
do
	echo "matrix $n * $n."
    ./relax-seq-vec.o $n 1
	./relax-omp-vec.o $n 1 1
	mpirun -N 1 relax-mpi-vec.o $n 1 
	n=$(( 2*n ))	
done

echo "!!!!!!!iteration number!!!!!!!!"
itr=100
while (( $itr <= 1000 ))
do
	echo "iterations: $itr"
    ./relax-seq-vec.o 1024 $itr
	./relax-omp-vec.o 1024 $itr 1
	mpirun -N 1 relax-mpi-vec.o 1024 $itr 
	itr=$(( 100+itr ))	
done