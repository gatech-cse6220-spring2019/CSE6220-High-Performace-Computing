#include "utils.h"

bool input_sanity_check(std::ostringstream& err_msg, int argc, char **argv) {

	int n,k;

	err_msg << "\n";

	if (argc!=3) {
		err_msg << "usage: " << argv[0] << " n k" << std::endl;
		return false;
	}

	n=atoi(argv[1]);
	k=atoi(argv[2]);

	if (n<0) {
		err_msg << "ERROR: n should be greater than zero" << std::endl;
		return false;
	}
	if (k<0) {
		err_msg << "ERROR: k should be greater than zero" << std::endl;
		return false;
	}
	if (n<k) {
		err_msg << "ERROR: n should be greater than or equal to k" << std::endl;
		return false;
	}

	return true;
}

void write_output(	std::ostream& os,
					double time_elapsed,
					std::vector<std::vector<unsigned int> >& all_solns) {

	os << all_solns.size() << std::endl;
    unsigned int nq = all_solns[0].size();
	for (unsigned int i=0; i < all_solns.size(); i++) {
		for (unsigned int j=0; j < nq - 1; j++) {
			os << all_solns[i][j] << " ";
		}
        os<<all_solns[i][nq-1];
		os << std::endl;
	}

}
