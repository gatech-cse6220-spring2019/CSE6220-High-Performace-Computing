CX4220/CSE 6220 Programming Assignment 2
=================================

## Code structure

All the code is located at the root level of the project.

There are multiple header and .cpp files, your implementation will go
into the following file:

- `solver.cpp`: Implement the sequential algorithm for nqueen problem, the master and worker functions for parallel nqueen according
  to the function declarations in `solver.h`


Other files containing code that you should not change are:
- `solver.h`: Declares the nqueen functions.
- `utils.h` and `utils.cpp`: Implements common utility functions.
- `main.cpp`: Implements code for the main executable `nqueen`. This does
  input/output reading and calling of the actual functions.


## Compiling

In order to compile everything, simply run
```sh
make
```


## Running
For running on local system, do:
```sh
mpirun -np <num_procs> ./nqueen <n> <k>
```


For running on the PACE cluster, do:
```sh
qsub -v p=<num_procs>,n=<number_of_queens>,k=<k> pbs_script.pbs
```
