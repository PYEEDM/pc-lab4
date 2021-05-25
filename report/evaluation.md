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

not formated yet...
!!!!!!!size of matrix!!!!!!!!
matrix 256 * 256.
--- [relax-sequential-vec.c] time elapsed 000 ms
--- [relax-omp-vec.c] time elapsed 000 ms
--- [relax-mpi-vec.c] time elapsed 000 ms
matrix 512 * 512.
--- [relax-sequential-vec.c] time elapsed 000 ms
--- [relax-omp-vec.c] time elapsed 000 ms
--- [relax-mpi-vec.c] time elapsed 000 ms
matrix 1024 * 1024.
--- [relax-sequential-vec.c] time elapsed 001 ms
--- [relax-omp-vec.c] time elapsed 002 ms
--- [relax-mpi-vec.c] time elapsed 003 ms
matrix 2048 * 2048.
--- [relax-sequential-vec.c] time elapsed 007 ms
--- [relax-omp-vec.c] time elapsed 009 ms
--- [relax-mpi-vec.c] time elapsed 014 ms
matrix 4096 * 4096.
--- [relax-sequential-vec.c] time elapsed 028 ms
--- [relax-omp-vec.c] time elapsed 038 ms
--- [relax-mpi-vec.c] time elapsed 056 ms
matrix 8192 * 8192.
--- [relax-sequential-vec.c] time elapsed 113 ms
--- [relax-omp-vec.c] time elapsed 125 ms
--- [relax-mpi-vec.c] time elapsed 219 ms
matrix 16384 * 16384.
--- [relax-sequential-vec.c] time elapsed 459 ms
--- [relax-omp-vec.c] time elapsed 529 ms
--- [relax-mpi-vec.c] time elapsed 2296 ms
matrix 32768 * 32768.
--- [relax-sequential-vec.c] time elapsed 2124 ms
--- [relax-omp-vec.c] time elapsed 2420 ms
--- [relax-mpi-vec.c] time elapsed 10235 ms
!!!!!!!iteration number!!!!!!!!
iterations: 100
--- [relax-sequential-vec.c] time elapsed 143 ms
--- [relax-omp-vec.c] time elapsed 192 ms
--- [relax-mpi-vec.c] time elapsed 140 ms
iterations: 200
--- [relax-sequential-vec.c] time elapsed 275 ms
--- [relax-omp-vec.c] time elapsed 375 ms
--- [relax-mpi-vec.c] time elapsed 272 ms
iterations: 300
--- [relax-sequential-vec.c] time elapsed 402 ms
--- [relax-omp-vec.c] time elapsed 560 ms
--- [relax-mpi-vec.c] time elapsed 415 ms
iterations: 400
--- [relax-sequential-vec.c] time elapsed 547 ms
--- [relax-omp-vec.c] time elapsed 748 ms
--- [relax-mpi-vec.c] time elapsed 547 ms
iterations: 500
--- [relax-sequential-vec.c] time elapsed 682 ms
--- [relax-omp-vec.c] time elapsed 934 ms
--- [relax-mpi-vec.c] time elapsed 683 ms
iterations: 600
--- [relax-sequential-vec.c] time elapsed 820 ms
--- [relax-omp-vec.c] time elapsed 1124 ms
--- [relax-mpi-vec.c] time elapsed 813 ms
iterations: 700
--- [relax-sequential-vec.c] time elapsed 971 ms
--- [relax-omp-vec.c] time elapsed 1373 ms
--- [relax-mpi-vec.c] time elapsed 985 ms
iterations: 800
--- [relax-sequential-vec.c] time elapsed 1153 ms
--- [relax-omp-vec.c] time elapsed 1708 ms
--- [relax-mpi-vec.c] time elapsed 1127 ms
iterations: 900
--- [relax-sequential-vec.c] time elapsed 1320 ms
--- [relax-omp-vec.c] time elapsed 2124 ms
--- [relax-mpi-vec.c] time elapsed 1341 ms
iterations: 1000
--- [relax-sequential-vec.c] time elapsed 1550 ms
--- [relax-omp-vec.c] time elapsed 2673 ms
--- [relax-mpi-vec.c] time elapsed 1531 ms
