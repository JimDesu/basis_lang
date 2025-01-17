#include <cctype>
#include <deque>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string> 

#include "basis.hpp"
#include "messages.hpp"
#include "parser.hpp"
#include "Tokenizer.hpp"

using namespace basis;

iter_t basis::end_of_range( iter_t start, iter_t finish) {
    int rangeStart = (*start)->pos;
    iter_t current{ start };
    while ((++current != finish) && ((*current)->pos > rangeStart));
    return current;
}

bool basis::parse_file(std::ifstream* file, const char * fileName) {
    std::deque<std::shared_ptr<Token>> tokens;

    return collect_tokens(&tokens, file, fileName)
        && parse_toplevel(&tokens);
}

bool basis::collect_tokens(std::deque<std::shared_ptr<Token>>* tokens, std::ifstream* file,
                           const char * fileName) {
    std::string line;
    bool res{ true };
    Tokenizer tokenizer{ tokens, fileName };
    while ( *file && res ) {
        std::getline(*file, line);
        res = tokenizer.withNextLine(line).tokenize();
    }
    return res;
}

bool basis::parse_toplevel(std::deque<std::shared_ptr<Token>>* tokens) {
    iter_t start = tokens->cbegin();
    iter_t finish = tokens->cend();
    iter_t next_end{ start };

//TODO
//    auto topLevel = make_toplevel();
    while (next_end != finish) {

        next_end = end_of_range(next_end,finish);
    }

    return false;
}

bool basis::p_token::operator()(iter_t& start, iter_t finish) {
    if (start == finish || (*start)->type != token_type) return false;
    ++start;
    return true;
}

bool basis::p2_token::operator()(iter_t& start, iter_t finish)
{
    if (start == finish || (*start)->type != token_type) return false;
    token_sink(*start);
    ++start;
    return true;
}

bool basis::p_error::operator()(iter_t& start, iter_t finish)
{
    if ((*p_parser)(start, finish)) return true;
    err_sink(*start);
    return false;
}

bool basis::p_any::operator()(iter_t& start, iter_t finish) {
    for (int i = 0; i < parsers.size(); i++) {
        if ((*parsers[i])(start, finish)) return true;
    }
    return false;
}


bool basis::p_seq::operator()(iter_t& start, iter_t finish)
{
    iter_t next{ start };
    for (int i = 0; i < parsers.size(); i++) {
        if (!(*parsers[i])(next, finish)) return false;
    }
    start = next;
    return true;
}


bool basis::p_opt::operator()(iter_t& start, iter_t finish)
{
    return (*p_parser)(start, finish) || true;
}

bool basis::p_multi::operator()(iter_t& start, iter_t finish)
{
    iter_t current{ start };
    while (current != finish && (*p_parser)(current, finish));
    if (current == start) return false;
    start = current;
    return true;
}

std::shared_ptr<Parser> basis::match(token_t tok_type)
{
    return std::make_shared<p_token>(tok_type);
}

std::shared_ptr<Parser> basis::match(token_t tok_type, Sink<std::shared_ptr<Token>> s)
{
    return std::make_shared<p2_token>(tok_type, s);
}

std::shared_ptr<Parser> basis::require(std::shared_ptr<Parser> spp, Sink<std::shared_ptr<Token>> s)
{
    return std::make_shared<p_error>(spp, s);
}

std::shared_ptr<p_any> basis::any()
{
    return std::make_shared<p_any>();
}

std::shared_ptr<p_seq> basis::sequence()
{
    return std::make_shared<p_seq>();
}

std::shared_ptr<p_opt> basis::optional(std::shared_ptr<Parser> pp) {
    return std::make_shared<p_opt>(pp);
}

std::shared_ptr<p_multi> basis::multiple(std::shared_ptr<Parser> pp) {
    return std::make_shared<p_multi>(pp);
}

std::shared_ptr<Parser> basis::operator~(std::shared_ptr<Parser> spp)
{
    return std::make_shared<p_opt>(spp);
}

std::shared_ptr<Parser> basis::operator++(std::shared_ptr<Parser> spp, int)
{
    return std::make_shared<p_multi>(spp);
}

std::shared_ptr<Parser> basis::operator+(std::shared_ptr<Parser> lhs, std::shared_ptr<Parser> rhs)
{
    std::shared_ptr<p_any> pa = any();
    (*pa) << lhs << rhs;
    return pa;
}

std::shared_ptr<Parser> basis::operator*(std::shared_ptr<Parser> lhs, std::shared_ptr<Parser> rhs)
{
    std::shared_ptr<p_seq> ps = sequence();
    (*ps) << lhs << rhs;
    return ps;
}

p_any& basis::operator<<(p_any& pa, std::shared_ptr<Parser> spp)
{
    pa.add(spp);
    return pa;
}

std::shared_ptr<p_any> basis::operator<<(std::shared_ptr<p_any> pa, std::shared_ptr<Parser> spp) {
    pa->add(spp);
    return pa;
}

p_seq& basis::operator<<(p_seq& pa, std::shared_ptr<Parser> spp)
{
    pa.add(spp);
    return pa;
}
