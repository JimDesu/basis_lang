#ifndef INCLUDE_MESSAGES_HPP
#define INCLUDE_MESSAGES_HPP

#include <string>

namespace basis {
	namespace INFO {
		constexpr const char SOURCE_FILE[] = "INFO source file: {}\n";
	}

	namespace WARNING {

	}

	namespace ERROR {
		constexpr const char USAGE[] = "ERROR usage: basis <filename>\n";
		constexpr const char CANNOT_OPEN_SOURCE[] = "ERROR cannot open source file: {}\n";
		constexpr const char PARSER_ERROR[] = "ERROR {} in {}({},{})\n";
	}

	namespace LEXER {
		constexpr const char UNEXPECTED_END_OF_LINE[] = "unexpected end of data";
		constexpr const char UNEXPECTED_CHAR_SEQUENCE[] = "unexpected character sequence";
		constexpr const char UNTERMINATED_STRING[] = "unterminated string";
		constexpr const char INVALID_HEX[] = "invalid hexadecimal constant";
	}
}

#endif
