## About the Implementation
The only part of the algorithm that can be parallelized is the `relax` method, which is why I added a 
`#pragma omp parallel for collapse(2)` line there. It obviously parallelizes the for loop. The `collapse(2)` 
clause is used in order to parallelize both levels (the outer and the inner loop), since parallelizing all levels
of loops is more efficient. (For instance, running the code without the `collapse` clause is about 1.1
times slower when using a single thread and a full 2.5 times slower when using 2 threads.)

The call to `omp_set_num_threads()` is passed the variable `num_threads`, the value of which is given as a 
mandatory third argument, when the code is run.

## About the Performance
The difference in execution time between the code run with 1 thread, 4, 8, 16 and 32 varies. Most of the time 
doubling the number of threads means more or less halving the execution time (with the matrix size being kept
constant). To be precise, when the number of threads doubles, the performance is increased between 1.3 and 
2.8 times.

"Strong scaling is tested by running the code with different number of threads while keeping the [size of the
matrix constant]" -> since the time is more or less halved, this somehow potentially has strong scaling, I think.

"Weak scaling is tested by running the code with different number of threads and with correspondingly scaled 
[size of the matrix]" -> running it in this way (n=1000, threads=1; n=2000, threads=2, etc.) and plotting the 
results as a scatter plot shows that it has more or less weak scaling, but not perfect.

## GFLOPS/s
Since we are only asked to present the absolute performance in terms of the size of the matrix (and not the number
of iterations), I am taking into account only the `relax` method. It has two nested for-loops with a single statement
in the center, which does 9 floating point operations. This makes the performance `((n-2)*(n-2)*9)/num_threads` 
GFLOPS/s.
