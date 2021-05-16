## About the Implementation
The only part of the algorithm that can be parallelized is the `relax` method, which is why I added a 
`#pragma omp parallel for collapse(2)` line there. It obviously parallelizes the for loop. The `collapse(2)` 
clause is used in order to parallelize both levels (the outer and the inner loop), since parallelizing all levels
of loops is more efficient.

The call to `omp_set_num_threads()` is passed the variable `num_threads`, the value of which is passed as a 
mandatory third argument to the algorithm, when the code is run.

## About the Performance
The execution time isn't much different between 1 thread and 2 threads. But when run with 4 threads, it performes
twice as fast as it does with 2 (same for 8 threads in comparison to 4, 16 comapred to 8, 32 compared to 16).

## GFLOPS/s
Since we are only asked to present the absolute performance in terms of the size of the matrix (and not the number
of iterations), I am taking into account only the `relax` method. It has two nested for-loops with a single statement
in the center, which does 9 floating point operations. This makes the performance `((n-2)*(n-2)*9)/num_threads` 
GFLOPS/s.
