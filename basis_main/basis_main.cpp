#include <iostream>

#include "../basis/basis.hpp"
#include "../basis/messages.hpp"

using namespace basis;

bool collect_args(cmd_args* args, int argc, char* argv[]) {
    if (argc != 2) {
        return false;
    }
    args->filename = argv[1];
    return true;
}

int main(int argc, char* argv[])
{
    cmd_args args = cmd_args{};
    if (collect_args(&args, argc, argv)) return compile(&args) ? 0 : 1;
    std::cout << ERROR::USAGE;
    return false;
}
