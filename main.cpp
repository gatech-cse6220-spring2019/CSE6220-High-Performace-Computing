#include <vector>
#include <iostream>
#include <fstream>
#include <mpi.h>
#include <string>
#include <sstream>
#include "solver.h"
#include "utils.h"


int main(int argc, char **argv) {


	MPI_Init(&argc, &argv);
	int proc_id, num_procs;

	// my proc id
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);

	// total number of procs in the world
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	// the board size and the size of partial soln to be computed by master
	unsigned int n,k;

	if (proc_id==0) {

		std::ostringstream err_msg;
		bool passed = input_sanity_check(err_msg, argc, argv);

		if (not passed) {
			std::cout << err_msg.str() << std::endl;
			MPI_Abort(MPI_COMM_WORLD, 1);
		}

		n=atoi(argv[1]);
		k=atoi(argv[2]);

	}

	MPI_Bcast(&n, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
	MPI_Bcast(&k, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

	// vector containing all solutions
	std::vector<std::vector<unsigned int> > all_solns;

	double time_elapsed=0.0;
	time_elapsed -= MPI_Wtime();

	if (num_procs==1) {
		seq_solver(n, all_solns);
	}
	else {

		if (proc_id==0) {
			nqueen_master(n, k, all_solns);
		}
		else {
			nqueen_worker(n, k);
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}

	time_elapsed += MPI_Wtime();


	if (proc_id==0) {
		std::ofstream out_file;
		out_file.open("out_"+std::to_string(n)+"_"+std::to_string(num_procs)+"_"+std::to_string(k)+".txt");
		write_output(out_file, time_elapsed, all_solns);
		out_file.close();

		std::cout << n << "\t" << num_procs << "\t" << k << "\t" << time_elapsed << std::endl;

	}

	MPI_Finalize();

	return 0;
}