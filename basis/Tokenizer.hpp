#ifndef _INCLUDE_TOKENIZER_HPP
#define _INCLUDE_TOKENIZER_HPP

#include <memory>
#include <iterator>

#include "basis.hpp"
#include "parser.hpp"

namespace basis {

    const std::string LINE_COMMENT{ ";" };
    const std::string COMMENT_START{ "(*" };
    const std::string COMMENT_END{ "*)" };
    const std::string WRITEVAR_INTRO{ "\'" };
    const std::string RESWORD_INTRO{ "." };
    const std::string TEXT_DELIMITER{ "\"" };
    const std::string HEX_START{ "0x" };

    enum class Mode {
        NORMAL,
        COMMENT
    };

    struct Tokenizer {
        std::deque<std::shared_ptr<Token>>* tokens;
        const std::string file_name;
        int line_no;
        size_t line_len;
        int curr_pos;
        char curr_char;
        const std::string* pLine;
        Mode mode;
        char prev_char;
        int comment_depth;
        //int tok_offset;
        bool status_ok;
        int next_pos;
        
        // public for unit testing
        void emit(token_t tt);
        void emit(token_t tt, std::string text);
        inline char charAt(int i) const;
        inline bool isDigit(int i) const;
        inline std::string charsTo(int i) const;
        inline bool isNibble(int pos) const;
        bool matches(const std::string target) const;
        bool matches(const int pos, const std::string target) const;
        void fail_line(const std::string message);
        bool tt_comment();
        bool tt_line_comment();
        bool tt_hex();
        bool tt_number();
        bool tt_symbol();
        bool tt_text();
        bool is_identifier_char(int position) const;
        bool is_identifier_head_char(int position) const;
        bool is_writevar_head_char(int position) const;
        bool tt_resword();
        bool tt_identifier();

        // actual interface
        Tokenizer(std::deque<std::shared_ptr<Token>>* pTokens);
        Tokenizer(std::deque<std::shared_ptr<Token>>* pTokens, const char * fileName);
        Tokenizer& withNextLine(const std::string& line);
        bool tokenize();
        bool isOK() const;
    };
}


#endif
