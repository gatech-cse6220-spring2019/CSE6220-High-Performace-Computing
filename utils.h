#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>


// checking for fallacies in input.
bool input_sanity_check(	std::ostringstream& err_msg,
							int argc,
							char **argv);


// write all solutions the the out stream
void write_output(	std::ostream& os,
					double time_elapsed,
					std::vector<std::vector<unsigned int> >& all_solns);

#endif // UTILS_H