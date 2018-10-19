Multicore CPU with OpenMP and OpenMPI parallel strategy.

Slurm Method:
Step 1: Load modules in Spartan:
*module load GCC/4.9.2
*module load OpenMPI/1.10.2-intel-2017.u2
*module load GMP/5.1.3-GCC-4.9.2

Step 2: Use Makefile to compile 
command:make

Step 3: Input OpenMP Environment Variables
export OMP_NUM_THREAD=8
export KMP_STACKSIZE="16M"

Step 4: Input parameter
command: mpiexec ./main keyfile keyNum
e.g. time mpiexec ./main 20K-keys.txt 20480

Edit slurm in the directory, and assign different number of nodes and OMP_NUM_THREADS. The sample slurm is in this repo for
your reference.

In the Data Set:
256-keys.txt: 256 keys
2048-keys.txt: 2048 keys
4096-keys.txt: 4096 keys
20K-keys.txt: 20480 keys
