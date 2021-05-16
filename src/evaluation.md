## seq



<!--
Iteration: 1
|algorithm |num cores |size=1000|size=6000|size=11000|size=16000|size=21000|size=26000|
|---|---|---|---|---|---|---|---|
|  seq | 1  |  9ms |328ms |1171ms  | 2457ms  |4257ms|6466ms|
| seq_optimized |1   |   |   |   |
| omp  | 1  |   |   |   |
| mpi|||||)-->


Iteration 1:
|algorithm |num cores |size=256|size=512|size=1024|size=2048|size=4096|size=8192|size=16384|size=32768
|---|---|---|---|---|---|---|---|---|---|
| size in MB|         | 0MB     | 2MB   | 8 MB    | 32 MB   |  128 MB | 512 MB  | 2048 MB  | 8192 MB      
|  seq     | 1        | 1ms     | 4ms   | 9ms     | 39 ms   |  159 ms | 633 ms  | 2580 ms  | 10322 ms

Iteration: 1-1000 100
