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
| |num cores            |size=256|size=512|size=1024|size=2048|size=4096|size=8192|size=16384|size=32768|
|---|---|---|---|---|---|---|---|---|---|
| (size in MB)|         | 0MB     | 2MB   | 8 MB    | 32 MB   |  128 MB | 512 MB  | 2048 MB  | 8192 MB|      
|  seq  without compiler optimization   | 1        | 1 ms     | 4 ms   | 9 ms    | 39 ms   |  159 ms | 633 ms  | 2580 ms  | 10322 ms|
| seq with compiler optimization|1| 0 ms  | 0 ms   | 2 ms    | 9 ms    |   39 ms |  154 ms | 635 ms   | 2537 ms |

Size: 1024 (8MB)
|algorithm| num cores | iter=1 | iter=100 | iter=200|iter=300|iter=400|iter=500|iter=600|iter=700|iter=800|iter=900|iter=1000|
|---|---|---|---|---|---|---|---|---|---|---|---|---|   
|seq| 1 |             9 ms     | 987ms    | 2003 ms| 2950 ms|3930 ms |4981 ms | 5912 ms|6940 ms |8064 ms | 9350 ms| 10642 ma |

Size: 512 (2MB)
|algorithm| num cores | iter=1 | iter=100 | iter=200|iter=300|iter=400|iter=500|iter=600|iter=700|iter=800|iter=900|iter=1000|
|---|---|---|---|---|---|---|---|---|---|---|---|---|   
|seq| 1 |               5 ms     | 251 ms    | 508 ms| 767 ms|980 ms |1268 ms | 1471 ms|1716 ms |1961 ms | 2206 ms| 2445 ma |
