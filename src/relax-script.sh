#!/usr/bin/env bash

#SBATCH -p csedu
#SBATCH -n 32
#SBATCH --time=3:00:00

echo "Compiling..."
make

echo "Running experiment for variable matrix size..."
n=256
while (( $n <= 32768 ))
do
	echo "Matrix size: $n * $n."
    c=1
	while (( $c <= 32 ))
	do
		echo "Number of cores: $c"
		for i in `seq 1 5`
		do
			echo "Measurement: $i"
			if (( c==1 ))
			then
				./relax-seq.o $n 1
				./relax-seq-vec.o $n 1
			fi
			./relax-omp.o $n 1 $c
			./relax-omp-vec.o $n 1 $c
			mpirun -n $c ./relax-mpi.o $n 1 
			mpirun -n $c ./relax-mpi-vec.o $n 1 
				
		done
		c=$(( 2*c ))
	done	
	n=$(( 2*n ))
done

echo "Running experiment for variable number of iterations..."
itr=100
while (( $itr <= 1000 ))
do	
	echo "Iterations: $itr"
    c=1
	while (( $c <= 32 ))
	do
		echo "Number of cores: $c"
		for i in `seq 1 5`
		do
            echo "Measurement: $i"
            if (( c==1 ))
            then
                ./relax-seq.o 2048 $itr
                ./relax-seq-vec.o 2048 $itr
            fi
            ./relax-omp.o 2048 $itr $c
            ./relax-omp-vec.o 2048 $itr $c
            mpirun -n $c relax-mpi.o 2048 $itr
            mpirun -n $c relax-mpi-vec.o 2048 $itr 
		done
		c=$(( 2*c ))
	done
	itr=$(( 100+itr ))	
done
