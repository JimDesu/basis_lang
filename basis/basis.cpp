#include <iostream>
#include <format>
#include <fstream>

#include "basis.hpp"
#include "messages.hpp"

using namespace basis;

bool basis::load_source(std::ifstream& src, char const* const filename) {
    std::ifstream file{ filename };
    if (file.is_open()) {
		std::cout << std::format(INFO::SOURCE_FILE, filename);
        file.swap(src);
        return true;
    } else {
		std::cout << std::format(ERROR::CANNOT_OPEN_SOURCE, filename);
        return false;
    }
}

bool basis::compile(const cmd_args * args)
{
    std::ifstream source_file;
    return load_source(source_file, args->filename) 
        && parse_file(&source_file, args->filename);
}
