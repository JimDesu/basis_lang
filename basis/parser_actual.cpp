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

class ep_Import : public Parser {

public:
    ep_Import(Sink<std::shared_ptr<Declaration>> spDeclSink) {};
    bool operator()(iter_t& start, iter_t finish) override {
        return false;
    };
};
class ep_CompilationUnit : public Parser {
    std::shared_ptr<CompilationUnit> spCompilationUnit;
    Sink<std::shared_ptr<CompilationUnit>> sink;
    std::shared_ptr<p_multi> spParser;
public:
    ep_CompilationUnit(Sink<std::shared_ptr<CompilationUnit>> s) :sink(s) {
        // add any received declarations to the current compilation unit
        auto a = [&](std::shared_ptr<Declaration> spDecl) {
                if (spCompilationUnit.get()) {
                    spCompilationUnit->declarations.push_back(spDecl);
                }
            };
        spParser = std::make_shared<p_multi>( 
          any() << std::make_shared<ep_Import>(a)
        );
    };

    bool operator()(iter_t& start, iter_t finish) override {
        spCompilationUnit = std::make_shared<CompilationUnit>();
        if ((*spParser)(start,finish)) {
            sink(spCompilationUnit);
            return true;
        }
        return false;
    }
};



