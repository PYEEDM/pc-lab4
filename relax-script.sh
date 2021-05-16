#!/bin/bash

#compile file
gcc relax-fix-original.c -o orginal.o #orginal relax
gcc -fopenmp relax-unoptimized_omp.c job-time.c -o openmp.o #openmp
mpicc -DMPI -o mpi.o relax-unoptimized_mpi.c job-time.c



for i in $(seq 1 10 20) #size
do
    for j in $(seq 0 10 20) #iter nr
    do 
        for t in $(seq 1 2)#threads nr
        do
        echo "size: $i, iter: $j  , thread= $t "
        ./original.o $i $j
        ./openmp.o $i $j $t
        mpirun -N $t ./mpi.o $i $j
        
        done
    done
done




# SIZE="${1:-10}"

# ITER_NR="${2:-100}"

# THREAD_NR="${3:-1}"
