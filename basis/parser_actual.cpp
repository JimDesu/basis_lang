#include <cctype>
#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string> 

#include "messages.hpp"
#include "parser.hpp"
#include "Elements.hpp"
#include "parser_actual.hpp"
 
using namespace basis;

template <typename T>
class elementParser : public Parser {
    Sink<std::shared_ptr<T>> sink;
    std::shared_ptr<p_multi> spParser;
public:
    elementParser(Sink<std::shared_ptr<T>> s, std::shared_ptr<Parser> spp) 
        : sink{ s }, spParser{ spp } {};

    bool operator()(iter_t& start, iter_t finish) override {
        //elementStack.push(std::make_shared<T>());
        if ((*spParser)(start,finish)) {
            //sink(elementStack.pop());
            return true;
        }
        return false;
    }
};

class ep_CompilationUnit : public Parser {
    std::shared_ptr<CompilationUnit> spCompilationUnit;
    Sink<std::shared_ptr<CompilationUnit>> sink;
    std::shared_ptr<p_multi> spParser;
public:
    ep_CompilationUnit(Sink<std::shared_ptr<CompilationUnit>> s) :sink(s) {
        // add any received declarations to the current compilation unit
        auto a = [&](std::shared_ptr<Declaration> spDecl) {
                if (spCompilationUnit) {
                    spCompilationUnit->declarations.push_back(spDecl);
                }
            };
        spParser = some( 
          any() << decl_module(a)
                << decl_import(a)
                << decl_export(a)
                << decl_class(a)
                << decl_record(a)
                << decl_command(a)
                << decl_intrinsic(a)
                << decl_enumeration(a)
                << decl_typeAlias(a)
                << decl_subtype(a)
                << decl_instance(a)
                << decl_instance(a)
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

std::shared_ptr<Parser> basis::compilation_unit(Sink<std::shared_ptr<CompilationUnit>> c) {
    return std::make_shared<ep_CompilationUnit>(c);
}

class decl_Module : public Parser {
    Sink<std::shared_ptr<Declaration>> sink;
public:
    decl_Module(Sink<std::shared_ptr<Declaration>> ds) : sink{ ds } {};
    bool operator()(iter_t& start, iter_t finish) override {
        return false;
    }
};

std::shared_ptr<Parser> basis::decl_module(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_import(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_export(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_class(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_record(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_command(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_intrinsic(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_enumeration(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_typeAlias(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_subtype(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}

std::shared_ptr<Parser> basis::decl_instance(Sink<std::shared_ptr<Declaration>> d) {
    return std::shared_ptr<Parser>();
}


