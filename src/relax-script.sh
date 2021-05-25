#!/usr/bin/env bash

#SBATCH -p csedu
#SBATCH --cpus-per-task 32
echo "compiling..."

make

echo "!!!!!!!size of matrix!!!!!!!!"

n=256
m=1
while (( $n <= 35000 ))
do
	echo "matrix $n * $n."
	while (($m <= 2))
	do
		echo "cores: $m"
		for i in `seq 1 5`
		do
			echo "measurement: $i"
			if ((m==1))
			then
				./relax-seq.o $n 1
				./relax-seq-vec.o $n 1
			fi
			./relax-omp.o $n 1 $m
			./relax-omp-vec.o $n 1 $m
			mpirun -N $m ./relax-mpi.o $n 1 
			mpirun -N $m ./relax-mpi-vec.o $n 1 
				
		done
		m=$((2*m))
	done	
	n=$(( 2*n ))
done

echo "!!!!!!!iteration number!!!!!!!!"
itr=100
m=1
while (( $itr <= 1000 ))
do	
	echo "iterations: $itr"
	while (($m <= 2))
	do
		echo "number of cores: $m"
		for i in `seq 1 5`
		do
		echo "measurement: $i"
		if ((m==1))
		then
			./relax-seq.o 2048 $itr
			./relax-seq-vec.o 2048 $itr
		fi
		./relax-omp.o 2048 $itr $m
		./relax-omp-vec.o 2048 $itr $m
		mpirun -N $m relax-mpi.o 2048 $itr
		mpirun -N $m relax-mpi-vec.o 2048 $itr 
		
		done
		m=$((2*m))
	done
	itr=$(( 100+itr ))	
done