#include <cctype>
#include <format>
#include <memory>
#include <iostream>

#include "Tokenizer.hpp"

using namespace basis;


void Tokenizer::emit(token_t tt) {
    std::shared_ptr<Token> t = std::make_shared<Token>();
    t->type = tt;
    t->line = line_no;
    t->pos = curr_pos;
    tokens->push_back(t);
};

void Tokenizer::emit(token_t tt, std::string text) {
    std::shared_ptr<Token> t = std::make_shared<Token>();
    t->type = tt;
    t->line = line_no;
    t->pos = curr_pos;
    t->text = text;
    tokens->push_back(t);
}

inline char basis::Tokenizer::charAt(int i) const {
    return (*pLine)[i];
}

inline bool basis::Tokenizer::isDigit(int i) const {
    return i < line_len && std::isdigit(charAt(i));

}

inline std::string basis::Tokenizer::charsTo(int i) const {
    return pLine->substr(curr_pos, i);
}

bool Tokenizer::matches(const std::string target) const {
    return matches(curr_pos, target);
}

bool Tokenizer::matches(const int pos, const std::string target) const {
    int len = (int)target.length();
    if (pos + len > line_len) return false;
    for (int i = 0; i < len; i++) {
        if (target[i] != charAt(i + pos)) return false;
    }
    return true;
}

void basis::Tokenizer::fail_line(const std::string message) {
    std::cout << std::format(ERROR::PARSER_ERROR,message, file_name, line_no, 1 + curr_pos);
    status_ok = false;
}

bool basis::Tokenizer::tt_comment() {
    if (matches(COMMENT_START)) {
        next_pos += (int) COMMENT_START.length();
        mode = Mode::COMMENT;
        comment_depth = 1;
        return true;
    }
    return false;
}

bool Tokenizer::tt_line_comment() {
    // If the current position is the beginning of a comment, then this line is toast.
    return matches(LINE_COMMENT);
}

inline bool basis::Tokenizer::isNibble(int pos) const {
    if (pos >= line_len) return false;
    char c = charAt(pos);
    return (c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F');
}

bool basis::Tokenizer::tt_hex() {
    if (!matches(HEX_START)) return false;
    int pos = curr_pos;
    pos += (int) HEX_START.length();
    // check first byte
    if (!(isNibble(pos) && isNibble(pos))) {
       fail_line(LEXER::INVALID_HEX);
       return false;
    }
    pos += 2;
    while (isNibble(pos) && isNibble(pos + 1)) pos += 2;
    // if we couldn't get two more hex chars, but get one, we've got a trailing half-byte
    if (isNibble(pos)) {
        fail_line(LEXER::INVALID_HEX);
        return false;
    }
    emit(token_t::HEX, charsTo(pos - curr_pos));
    next_pos = pos;
    return true;
}

// Note: longer symbols come first: clarity/efficiency trade-off.
constexpr int NUM_SYMBOLS = 19;
const struct TextMap {
    const std::string text;
    const token_t tok_type;
} symbols[NUM_SYMBOLS] {
    // pairs
    { "::", token_t::DCOLON },
    { "->", token_t::RARROW },
    { "|-", token_t::TURNSTYLE },
    // singletons
    { "&", token_t::AMPERSAND },
    { "@", token_t::AMPHORA },
    { "*", token_t::ASTERISK },
    { ":", token_t::COLON },
    { "=", token_t::EQUALS },
    { "!", token_t::BANG },
    { "-", token_t::HYPHEN },
    { "<", token_t::LANGLE, },
    { "{", token_t::LBRACE },
    { "[", token_t::LBRACKET },
    { "(", token_t::LPAREN },
    { "|", token_t::PIPE },
    { ">", token_t::RANGLE },
    { "}", token_t::RBRACE },
    { "]", token_t::RBRACKET },
    { ")", token_t::RPAREN },
};

bool Tokenizer::tt_symbol() {
    for( int i = 0; i < NUM_SYMBOLS; i++ ) {
        if (matches(symbols[i].text)) {
            emit(symbols[i].tok_type);
            next_pos += (int)symbols[i].text.length();
            return true;
        }
    }
    return false;
}

constexpr char HYPHEN = '-';
bool Tokenizer::tt_number() {
    int i = 0;
    // if we're startign with a hyphen, that may be OK
    if (curr_char == HYPHEN) ++i;
    // if we're not a digit, then not a number
    if (std::isdigit(charAt(curr_pos + i))) {
        ++i;
    } else {
        return false;
    }
    // accumulate digits
    while (isDigit(curr_pos + i)) ++i;
    // see if we're a decimal
    if( matches(curr_pos + i, ".") )  {
        int dpPosition = i;
        ++i;
        // accumulate more digits
        while (isDigit(curr_pos + i)) ++i;
        // if no more digits past the decimal point's position, we're not valid
        if( dpPosition == i ) return false;
        // done
        emit(token_t::DECIMAL, charsTo(i));
        next_pos += i;
    } else {
        // not a decimal, so done
        emit(token_t::INTEGER, charsTo(i));
        next_pos += i;
    }
    return true;
}

constexpr char QUOTE = '"';
constexpr char ESCAPE = '\\';
constexpr char NEWLN = 'n';

bool Tokenizer::tt_text() {
    if (matches(TEXT_DELIMITER)) {
        bool escape = false;
        int i = curr_pos + (int)TEXT_DELIMITER.length();;
        std::string text;
        while (i < line_len) {
            char c = charAt(i);
            if (escape) {
                switch (c) {
                    case QUOTE:
                        text.append(1,QUOTE);
                        break;
                    case ESCAPE:
                        text.append(1, ESCAPE);
                        break;
                    case NEWLN:
                        text.append(1, '\n');
                        break;
                    default:;
                        fail_line( LEXER::UNTERMINATED_STRING );
                        return false;
                }
                escape = false;
            } else {
                switch (c) {
                    case QUOTE:
                        emit(token_t::TEXT, text);
                        next_pos = i + 1;
                        return true;
                        break;
                    case ESCAPE:
                        escape = true;
                        break;
                    default:
                        text.append(1, c);
                }
            }
            ++i;
        }
    }
    return false;
}

bool Tokenizer::is_identifier_char(int position) const {
    if (position >= line_len) return false;
    char c{ charAt(position) };
    return isalnum(c) || c == '_';
}

bool Tokenizer::is_identifier_head_char(int position) const {
    return ( position < line_len && isalpha(charAt(position)) );
}

bool basis::Tokenizer::is_writevar_head_char(int position) const {
    return ( matches(position, WRITEVAR_INTRO ) );
}

bool Tokenizer::tt_identifier() {
    int i = curr_pos;
    bool is_writevar{ false };
    // the identifier may be a writevar, which starts with an apostrophe
    if (is_writevar_head_char(i)) {
        is_writevar = true;
        ++i;
    }
    // the identifier proper must start with an alpha character
    if (is_identifier_head_char(i)) {
        ++i;
    } else {
        return false;
    }
    // assemble the remainder of the identifier
    while (is_identifier_char(i)) ++i;
    // special cases... if the next character here is "." or "'" then
    // we don't have a clean identifier
    if (matches(i, WRITEVAR_INTRO) || matches(i, RESWORD_INTRO)) return false;
    // emit the identifier
    emit(is_writevar ? token_t::WRITEVAR : token_t::IDENTIFIER, charsTo(i - curr_pos));
    next_pos = i;
    return true;
}

constexpr int NUM_RESWORDS = 3;
const TextMap reswords[NUM_RESWORDS] {
    { ".cmd", token_t::COMMAND },
    { ".fail", token_t::FAIL },
    { ".record", token_t::RECORD },
};

bool Tokenizer::tt_resword() {
    // do not allow "." attached to an identifier
    if (curr_pos > 0 && is_identifier_char(curr_pos - 1)) return false;
    // scan for reserved words
    for( int i = 0; i < NUM_RESWORDS; i++ ) {
        const std::string s = reswords[i].text;
        if (matches(s)) {
            int slen = (int)s.length();
            if (is_identifier_char(curr_pos + slen)) {
                // cannot match a reserved word if there's another letter to follow
                return false;
            } else {
                emit(reswords[i].tok_type);
                next_pos += slen;
                return true;
            }
        }
    }
    return false;
}

Tokenizer::Tokenizer(std::deque<std::shared_ptr<Token>>* pTokens)
    : Tokenizer{ pTokens, "" } {};

Tokenizer::Tokenizer(std::deque<std::shared_ptr<Token>>* pTokens, char const * const fileName)
    :
    file_name{ fileName },
    tokens{ pTokens },
    line_no{ 0 },
    line_len{ 0 },
    curr_pos{ 0 },
    curr_char{ 0 },
    pLine{ 0 },
    mode(Mode::NORMAL),
    prev_char{ 0 },
    comment_depth{ 0 },
    status_ok{ true },
    next_pos{ 0 } {};

Tokenizer& Tokenizer::withNextLine(const std::string& line) {
    status_ok = true;
    pLine = &line;
    line_len = line.size();
    ++line_no;
    return *this;
}

bool Tokenizer::tokenize() {
    prev_char = 0;
    next_pos = 0;
    curr_pos = 0;
    while ( curr_pos < line_len ) {
        curr_char = charAt(curr_pos);
        switch (mode) {
        case Mode::NORMAL:
            if (tt_line_comment()) return true;
            if (std::isspace(curr_char)) {
                next_pos = curr_pos + 1;
                break;
            }
            if (!( tt_comment()
                || (status_ok && tt_resword())
                || (status_ok && tt_hex())  // must preceed symbols due to minus sign
                || (status_ok && tt_number())  // must preceed symbols due to minus sign
                || (status_ok && tt_symbol())
                || (status_ok && tt_text())
                || (status_ok && tt_identifier())
                )) {
                fail_line(LEXER::UNEXPECTED_CHAR_SEQUENCE);
                return false;
            }
            break;
        case Mode::COMMENT:
            if (prev_char == COMMENT_START[0] && curr_char == COMMENT_START[1]) {
                ++comment_depth;
            } else {
                if (prev_char == COMMENT_END[0] && curr_char == COMMENT_END[1]) {
                    if (--comment_depth == 0) mode = Mode::NORMAL;
                }
            }
            next_pos = curr_pos + 1;
            break;
        }
        if (next_pos >= line_len) break;
        prev_char = charAt(next_pos - 1);
        curr_pos = next_pos;
    }
    return true;
}

bool basis::Tokenizer::isOK() const {
    // ending in comment or string mode is not OK, both need to finish to be valid
    return status_ok && mode == Mode::NORMAL;
}


