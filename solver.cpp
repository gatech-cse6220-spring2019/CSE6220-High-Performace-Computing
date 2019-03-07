#include "solver.h"
#include<map>
#include<set>

/*************************** DECLARE YOUR HELPER FUNCTIONS HERE ************************/





/*************************** solver.h functions ************************/


void seq_solver(unsigned int n, std::vector<std::vector<unsigned int> >& all_solns) {

	// TODO: Implement this function
    std::set<unsigned int>hashset;
    for(unsigned int i=0; i<n; i++)
        hashset.insert(i);
    std::vector<unsigned int>partial;
    dfs(n, partial, hashset, all_solns);
}

bool is_valid(std::vector<unsigned int>& arr, unsigned int row){
    for(unsigned int i = 0; i < arr.size(); i++){
        unsigned int a = arr[i]>row? arr[i]-row : row-arr[i];
        unsigned int b = arr.size()>i? arr.size()-i: i-arr.size();
        if(a==b){
            return false;
        }
    }
    return true;
}

void dfs(unsigned int n, std::vector<unsigned int>& partial, std::set<unsigned int>&hashset, std::vector<std::vector<unsigned int> >& all_solns){
    if (partial.size() >= n){
        all_solns.push_back(partial);
        return;
    }
    std::set<unsigned int>temp = hashset;
    for(auto it = temp.begin(); it != temp.end(); it++){
        if(is_valid(partial, *it)){
            hashset.erase(*it);
            partial.push_back(*it);
            dfs(n, partial, hashset, all_solns);
            partial.pop_back();
            hashset.insert(*it);
        }
    }
}


void nqueen_master(	unsigned int n,
					unsigned int k,
					std::vector<std::vector<unsigned int> >& all_solns) {

	// TODO: Implement this function

	/* Following is a general high level layout that you can follow
	 (you are not obligated to design your solution in this manner.
	  This is provided just for your ease). */


	/******************* STEP 1: Send one partial solution to each worker ********************/
	/*
	 * for (all workers) {
	 * 		- create a partial solution.
	 * 		- send that partial solution to a worker
	 * }
	 */


	/******************* STEP 2: Send partial solutions to workers as they respond ********************/
	/*
	 * while() {
	 * 		- receive completed work from a worker processor.
	 * 		- create a partial solution
	 * 		- send that partial solution to the worker that responded
	 * 		- Break when no more partial solutions exist and all workers have responded with jobs handed to them
	 * }
	 */

	/********************** STEP 3: Terminate **************************
	 *
	 * Send a termination/kill signal to all workers.
	 *
	 */
    int num_processors;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processors);
    std::vector<std::vector<unsigned int>>all_partial;
    std::set<unsigned int>hashset;
    for(unsigned int i = 0; i < n; i++){
        hashset.insert(i);
    }
    std::vector<unsigned int>partial;
    dfs(k, partial, hashset, all_partial);
    int n_finished = 0;
    bool is_end = false;
    for(int i = 1; i < num_processors; i++){
        std::vector<unsigned int>send = all_partial.back();
        MPI_Send(&send[0], k, MPI_UNSIGNED, i, 111, comm);
        if (all_partial.size() == 0) break;
        all_partial.pop_back();
    }
    std::vector<std::vector<unsigned int>>sols;
    int n_worker, n_sol;
    while (true){
        MPI_Status stat;
        MPI_Recv(&n_sol, 1, MPI_UNSIGNED, MPI_ANY_SOURCE, 333, comm, &stat);
        n_worker = stat.MPI_SOURCE;
        if (n_sol > 0){
            int size = n_sol * n;
            unsigned int * p = (unsigned int *)malloc(sizeof(unsigned int) * size);
            MPI_Recv(p, size, MPI_UNSIGNED, n_worker, 111, comm, &stat);
            int i = 0;
            while (i < size){
                if (i % n == 0){
                    all_solns.push_back(std::vector<unsigned int>());
                }
                all_solns[all_solns.size() - 1].push_back(*(p + (i++)));
            }
        }
        if(!is_end){
            std::vector<unsigned int>send = all_partial.back();
            MPI_Send(&send[0], k, MPI_UNSIGNED, n_worker, 111, comm);
            all_partial.pop_back();
            if (all_partial.size() == 0) {
                is_end = true;
                n_finished += 1;
            }
        }
        else{
            n_finished += 1;
            if (n_finished >= num_processors)break;
        }
    }
    partial.resize(k, 555);
    for(int i=1; i < num_processors; i++){
        MPI_Send(&partial[0], k, MPI_UNSIGNED, i, 222, comm);
    }
}

void nqueen_worker(	unsigned int n,
					unsigned int k) {
	// TODO: Implement this function

	// Following is a general high level layout that you can follow (you are not obligated to design your solution in this manner. This is provided just for your ease).

	/*******************************************************************
	 *
	 * while() {
	 *
	 * 		wait for a message from master
	 *
	 * 		if (message is a partial job) {
	 *				- finish the partial solution
	 *				- send all found solutions to master
	 * 		}
	 *
	 * 		if (message is a kill signal) {
	 *
	 * 				quit
	 *
	 * 		}
	 *	}
	 */
    std::vector<unsigned int>partial(k, 1);
    while(true){
        MPI_Status stat;
        MPI_Recv(&partial[0], k, MPI_UNSIGNED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
        if(stat.MPI_TAG == 111){
            std::set<unsigned int>hashset;
            std::vector<std::vector<unsigned int>>sols;
            for(unsigned int i=0; i < n; i++){
                hashset.insert(i);
            }
            for(auto a:partial){
                hashset.erase(a);
            }
            dfs(n, partial, hashset, sols);
            
            unsigned int n_sols = sols.size();
            MPI_Send(&n_sols, 1, MPI_UNSIGNED, 0, 333, MPI_COMM_WORLD);
            if (n_sols > 0){
                int size = n_sols * n;
                unsigned int * p = (unsigned int*)malloc(sizeof(unsigned int) * size);
                int i = 0;
                for(auto v:sols){
                    for(auto a:v){
                        *(p + (i++)) = a;
                    }
                }
                MPI_Send(p, size, MPI_UNSIGNED, 0, 111, MPI_COMM_WORLD);
            }
        }
        else if(stat.MPI_TAG == 222){
            break;
        }
    }
}



/*************************** DEFINE YOUR HELPER FUNCTIONS HERE ************************/







