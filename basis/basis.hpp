#ifndef INCLUDE_BASIS_HPP
#define INCLUDE_BASIS_HPP

#include <iostream>

#include "messages.hpp"

namespace basis {

	struct cmd_args {
		const char *filename;
	};

	bool load_source(std::ifstream& file, const char * filename);
	bool compile(const cmd_args *  args);
	bool parse_file(std::ifstream* file, const char* fileName);
}

#endif
