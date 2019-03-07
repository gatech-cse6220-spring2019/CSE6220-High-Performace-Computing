#ifndef SOLVER_H
#define SOLVER_H

#include <cstdio>
#include <vector>
#include<set>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <random>
#include <mpi.h>
#include <unistd.h>



/************ Sequential solving function for nqueen*****************
 *
 * Takes the board size as input and creates all solutions to the n-queen problem. All solutions are put in the vector all_solns. Each solution is a vector of length n. Position i in a solution represents the row number of queen in column i. Columns and rows are numbered from 0 to n-1.
 *
 * Parameters:
 * n : The board size
 * all_solns: A vector of all solutions, each solution being an n sized vector.
 *
 * *****************************************************************/
void seq_solver(unsigned int n, std::vector<std::vector<unsigned int> >& all_solns);

bool is_valid(std::vector<unsigned int>& arr, unsigned int row);
void dfs(unsigned int n, std::vector<unsigned int>& partial, std::set<unsigned int>&hashset, std::vector<std::vector<unsigned int> >& all_solns);




/************ Master function for parallel nqueen*****************
 *
 * The master repeatedly produces a k length incomplete solution and sends this partial solution to an available worker processors. The worker will get occupied in completing this partial solution. (more on worker's job later). All workers are available in the beginning. If at some stage all workers are occupied, master waits till a worker returns with finished job. The master then sends a new partial solution to this newly freed worker. The master collects all these solutions in the vector all_solns. If there are no more partial solutions to be sent and all jobs sent to workers have been completed and returned to master, master will send a kill signal to all workers and quit.
 *
 *
 * Parameters:
 * n : The board size
 * k: The length of solution to be completed by master before handing the job to worker
 * all_solns: A vector containing all solutions. Each solution is a vector of length n. Position i in a solution represents the row number of queen in column i. Columns and rows are numbered from 0 to n-1.
 *
 * *****************************************************************/
void nqueen_master(	unsigned int n,
					unsigned int k,
					std::vector<std::vector<unsigned int> >& all_solns);







/************ Worker function for parallel nqueen*****************
 *
 * The worker will wait for a message from master. If this message is a partial k length solution to n-queen problem, the worker will finish it and return the complete solutions to master. Note that more than one complete solutions can be obtained from a partial k length solution. If the message received from master is a kill signal, the worker will quit.
 *
 *
 * Parameters:
 * n : The board size
 * k: The length of solution completed by master before handing the job to worker
 *
 * *****************************************************************/
void nqueen_worker(	unsigned int n,
					unsigned int k);



#endif // SOLVER_H
