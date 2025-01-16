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
    sink<std::shared_ptr<CompilationUnit>> elementSink;
public:
    ep_CompilationUnit(sink<std::shared_ptr<CompilationUnit>> s) :elementSink(s) {

    };

    // Inherited via ElementParser
    bool operator()(iter_t& start, iter_t finish) override {
        spCompilationUnit = std::make_shared<CompilationUnit>();

        // set up a multi of ...
            // set up an any of ...
                // a bunch of Declaration types
                // all of which append to spCompilationUnit

        if (false) {
            elementSink(spCompilationUnit);
        }
        return false;
    }
};



