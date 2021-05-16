## About the Implementation
### Choices
The only part of the algorithm that can be parallelized is the `relax` method, which is why I added a 
`#pragma omp parallel for collapse(2)` line there. It obviously parallelizes the for loop. The `collapse(2)` 
clause is used in order to parallelize both levels (the outer and the inner loop), since parallelizing all levels
of loops is more efficient. (For instance, running the code without the `collapse` clause is about 1.1
times slower when using a single thread and a full 2.5 times slower when using 2 threads.)

The call to `omp_set_num_threads()` is passed the variable `num_threads`, the value of which is given as a 
mandatory third argument, when the code is run.

### Programming and Debugging Effort
Working with OpenMP is quite fast and easy once you know what part of the code you need to parallelize. I had
no issues that needed debugging (when it comes to OpenMP; I did have typos in the other code I introduced that
needed debugging but I don't think that is relevant), but the programming did require some effort, especially
since at the beginning I mistakingly also tried to parallelize the `init` function, and wasn't aware of the 
`collapse` clause.

## About the Performance
The difference in execution time between the code run with 1 thread, 4, 8, 16 and 32 varies. Most of the time 
doubling the number of threads means more or less halving the execution time (with the matrix size being kept
constant). That's when the code is run locally/on the main node. On the cluster, there is almost no 
difference between the performance of 1 thread and 2 threads.

For tests made on the main node:   

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
