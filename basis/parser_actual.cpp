#include <cctype>
#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string> 

#include "messages.hpp"
#include "parser.hpp"
#include "Elements.hpp"
 
using namespace basis;

class ep_CompilationUnit : public Parser {
    std::shared_ptr<CompilationUnit> spCompilationUnit;
    Sink<std::shared_ptr<CompilationUnit>> sink;
public:
    ep_CompilationUnit(Sink<std::shared_ptr<CompilationUnit>> s) :sink(s) {

    };

    // Inherited via ElementParser
    bool operator()(iter_t& start, iter_t finish) override {
        spCompilationUnit = std::make_shared<CompilationUnit>();

        // set up a multi of ...
            // set up an any of ...
                // a bunch of Declaration types
                // all of which append to spCompilationUnit

        if (false) {
            sink(spCompilationUnit);
        }
        return false;
    }
};



