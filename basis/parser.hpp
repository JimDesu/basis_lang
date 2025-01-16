#ifndef _INCLUDE_PARSER_HPP
#define _INCLUDE_PARSER_HPP

#include <deque>
#include <fstream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "basis.hpp"
#include "Elements.hpp"

namespace basis {

    enum class token_t {
        AMPERSAND,
        AMPHORA,
        ASTERISK,
        BANG,
        COLON,
        COMMA,
        COMMAND,
        DCOLON,
        DECIMAL,
        EQUALS,
        FAIL,
        HEX,
        HYPHEN,
        IDENTIFIER,
        INTEGER,
        LANGLE,
        LBRACE,
        LBRACKET,
        LPAREN,
        PIPE,
        RANGLE,
        RARROW,
        RBRACE,
        RBRACKET,
        RECORD,
        RPAREN,
        TEXT,
        TURNSTYLE,
        WRITEVAR
    };

    struct Token {
        token_t type;
        int line;
        int pos;
        std::string text;
    };

    using PToken = std::shared_ptr<Token>;
    using iter_t = std::deque<std::shared_ptr<Token>>::const_iterator;

    class Parser {
    public:
        // start should never be advanced if the parse fails
        // depending on optionality, it may or may not be advanced on success
        virtual bool operator()(iter_t& start, iter_t finish) = 0;
    };

    template <typename T>
    using sink = std::function<void(T)>;

    // parse functions ----------------------------------------
    iter_t end_of_range( iter_t start, iter_t finish);
    bool parse_file(std::ifstream* file, char const * const fileName);
    bool collect_tokens(std::deque<std::shared_ptr<Token>>* tokens, std::ifstream* file, const char* fileName);
    bool parse_toplevel(std::deque<std::shared_ptr<Token>>* tokens);

    std::shared_ptr<Parser> make_toplevel();

    // parse combinators ----------------------------------------
    class p_token : public Parser {
        token_t token_type;
    public:
        p_token(token_t tok_type) : token_type{ tok_type } {};
        virtual bool operator()(iter_t& start, iter_t finish);
    };

    class p2_token : public Parser {
        token_t token_type;
        sink <std::shared_ptr<Token>> token_sink;
    public:
        p2_token(token_t tok_type, sink<std::shared_ptr<Token>> s):
            token_type{ tok_type }, token_sink{ s } {};
        virtual bool operator()(iter_t& start, iter_t finish);
    };

    class p_error : public Parser {
        sink<std::shared_ptr<Token>> err_sink;
        std::shared_ptr<Parser> p_parser;
    public:
        p_error(std::shared_ptr<Parser> spp, sink <std::shared_ptr<Token>> s)
            : p_parser{ spp }, err_sink{ s } {};
        virtual bool operator()(iter_t& start, iter_t finish);
    };
    class p_any : public Parser {
        std::vector<std::shared_ptr<Parser>> parsers;
    public:
        p_any() {};
        p_any& add(std::shared_ptr<Parser> spp) {
            parsers.push_back(spp);
            return *this;
        }
        virtual bool operator()(iter_t& start, iter_t finish);
    };

    class p_seq : public Parser {
        std::vector<std::shared_ptr<Parser>> parsers;
    public:
        p_seq() {};
        p_seq& add(std::shared_ptr<Parser> spp) {
            parsers.push_back(spp);
            return *this;
        }
        virtual bool operator()(iter_t& start, iter_t finish);
    };

    class p_opt : public Parser {
        std::shared_ptr<Parser> p_parser;
    public:
        p_opt(std::shared_ptr<Parser> pp) : p_parser{ pp } {};
        virtual bool operator()(iter_t& start, iter_t finish);
    };

    class p_multi : public Parser {
        std::shared_ptr<Parser> p_parser;
    public:
        p_multi(std::shared_ptr<Parser> pp): p_parser{ pp } {};
        virtual bool operator()(iter_t& start, iter_t finish);
    };

    std::shared_ptr<Parser> match(token_t tok_type);
    std::shared_ptr<Parser> match(token_t tok_type, sink<std::shared_ptr<Token>> s);
    std::shared_ptr<Parser> require(std::shared_ptr<Parser> spp, sink<std::shared_ptr<Token>> s);
    std::shared_ptr<p_any> any();
    std::shared_ptr<p_seq> seq();
    std::shared_ptr<Parser> operator~(std::shared_ptr<Parser> spp);
    std::shared_ptr<Parser> operator++(std::shared_ptr<Parser> spp, int);
    std::shared_ptr<Parser> operator+(std::shared_ptr<Parser> lhs, std::shared_ptr<Parser> rhs);
    std::shared_ptr<Parser> operator*(std::shared_ptr<Parser> lhs, std::shared_ptr<Parser> rhs);

    p_any& operator<<(p_any& pa, std::shared_ptr<Parser> spp);
    p_seq& operator<<(p_seq& pa, std::shared_ptr<Parser> spp);
}

#endif
