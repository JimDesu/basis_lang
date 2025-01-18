#include "pch.h"
#include "CppUnitTest.h"
#include "../basis/basis.hpp"
#include "../basis/parser.hpp"
#include "../basis/Tokenizer.hpp"

#include <fstream>
#include <string>
#include <deque>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace basis;

namespace parsertests
{
	TEST_CLASS(parsertests)
	{
	public:
		TEST_METHOD(parsertests_range1)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 1, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 2, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 3, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 4, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 2, 2, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 2, 3, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 2, 5, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 2, 7, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 2, 9, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 2, 11, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 2, 13, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 2, 117, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 3, 1, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 4, 1, "" ) );

			iter_t x = end_of_range(tokens.cbegin(), tokens.cend());
			Assert::AreEqual(3, (*x)->line);
		}
		TEST_METHOD(parsertests_token_recognizer)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::AMPHORA, 1, 1, "" ) );

			iter_t start = tokens.cbegin();
			Assert::IsFalse((*match(token_t::HEX))(start, tokens.cend()));
			Assert::IsTrue(start == tokens.cbegin());
			Assert::IsTrue((*match(token_t::AMPHORA))(start, tokens.cend()));
			Assert::IsTrue(start == tokens.cend());
		}
		TEST_METHOD(parsertests_token_p2)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::AMPHORA, 1, 1, "" ) );
			Token tok{ token_t::BANG };
			auto s = [&tok](PToken pt) { tok.type = pt->type; };
			auto tp1 = match(token_t::HEX, s );
			iter_t start = tokens.cbegin();
			Assert::IsFalse((*tp1)(start, tokens.cend()));
			Assert::IsFalse(tok.type == token_t::AMPHORA);
			Assert::IsTrue(start == tokens.cbegin());
			auto tp2 = match( token_t::AMPHORA, s );
			Assert::IsTrue((*tp2)(start, tokens.cend()));
			Assert::IsTrue(tok.type == token_t::AMPHORA);
			Assert::IsTrue(start == tokens.cend());
		}
		TEST_METHOD(parsertests_any)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 1, "" ) );

			auto p1 = any();
			(*p1) << match(token_t::AMPERSAND) 
		          << match(token_t::AMPHORA);
			iter_t start{ tokens.cbegin() };
			Assert::IsFalse((*p1)(start, tokens.cend()));
			Assert::IsTrue(start == tokens.cbegin());
			auto p2 = any();
			(*p2) << match(token_t::AMPERSAND)
				  << match(token_t::AMPHORA)
				  << match(token_t::LANGLE)
				  << match(token_t::HEX);
			Assert::IsTrue((*p2)(start, tokens.cend()));
			Assert::IsTrue(start == tokens.cend());
		}
		TEST_METHOD(parsertests_any2)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 1, "" ) );

			auto pp = match(token_t::AMPERSAND)
				    + match(token_t::AMPHORA)
				    + match(token_t::LANGLE)
				    + match(token_t::HEX);
			iter_t start{ tokens.cbegin() };
			Assert::IsTrue((*pp)(start, tokens.cend()));
			Assert::IsTrue(start == tokens.cend());
		}
		TEST_METHOD(parsertests_seq)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 1, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::AMPERSAND, 1, 2, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::AMPHORA, 1, 3, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::LANGLE, 1, 4, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::INTEGER, 1, 5, "" ) );

			auto ps1 = each();
			(*ps1) << match(token_t::HEX)
				   << match(token_t::AMPHORA)
				   << match(token_t::AMPERSAND)
				   << match(token_t::LANGLE);
			iter_t start{ tokens.cbegin() };
			Assert::IsFalse((*ps1)(start, tokens.cend()));
			Assert::IsTrue(start == tokens.cbegin());
			auto ps2 = each();
			(*ps2) << match(token_t::HEX)
				   << match(token_t::AMPERSAND)
				   << match(token_t::AMPHORA)
				   << match(token_t::LANGLE);
			Assert::IsTrue((*ps2)(start, tokens.cend()));
			Assert::IsTrue((*start)->type == token_t::INTEGER && (*start)->pos == 5);
		}
		TEST_METHOD(parsertests_seq2)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::LBRACKET, 1, 1, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::WRITEVAR, 1, 2, "fish" ) );
			tokens.push_back( std::make_shared<Token>( token_t::RBRACKET, 1, 1, "" ) );

			Token tok{ token_t::BANG };
			auto s = [&tok](PToken pt) {
				tok.type = pt->type;
				tok.text = pt->text;
			};

			auto ps1 = match(token_t::LBRACKET)
				     * match(token_t::WRITEVAR,s)
				     * match(token_t::RBRACKET);
			iter_t start{ tokens.cbegin() };
			Assert::IsTrue((*ps1)(start, tokens.cend()));
			Assert::AreEqual("fish", tok.text.c_str());
			Assert::IsTrue(tok.type == token_t::WRITEVAR);
		}
		TEST_METHOD(parsertests_opt) {
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 1, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::INTEGER, 1, 2, "" ) );

			// succeeds, but consumes no matching input
			auto p1 = ~match(token_t::AMPHORA);
			iter_t start1{ tokens.cbegin() };
			Assert::IsTrue((*p1)(start1, tokens.cend()));
			Assert::IsTrue(start1 == tokens.cbegin());
			// succeeds and consumes matching input
			auto p2 = ~match(token_t::HEX);
			iter_t start2{ tokens.cbegin() };
			Assert::IsTrue((*p2)(start2, tokens.cend()));
			Assert::IsTrue((*start2)->type == token_t::INTEGER && (*start2)->pos == 2);
		}
		TEST_METHOD(parsertests_opt2) {
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 1, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::INTEGER, 1, 2, "" ) );

			// succeeds, but consumes no matching input
			auto p1 = ~match(token_t::AMPHORA);
			iter_t start1{ tokens.cbegin() };
			Assert::IsTrue((*p1)(start1, tokens.cend()));
			Assert::IsTrue(start1 == tokens.cbegin());
			// succeeds and consumes matching input
			auto p2 = ~match(token_t::HEX);
			iter_t start2{ tokens.cbegin() };
			Assert::IsTrue((*p2)(start2, tokens.cend()));
			Assert::IsTrue((*start2)->type == token_t::INTEGER && (*start2)->pos == 2);
		}
		TEST_METHOD(parsertests_multi) {
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 1, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 2, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 3, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::HEX, 1, 4, "" ) );
			tokens.push_back( std::make_shared<Token>( token_t::INTEGER, 1, 5, "" ) );

			int matchCount{ 0 };
			auto s = [&](PToken pt) { ++matchCount; };

			std::shared_ptr<Parser> p2t = std::make_shared<p2_token>(token_t::HEX, s);
			auto pm = match(token_t::HEX, s)++;
			iter_t start{ tokens.cbegin() };
			Assert::IsTrue((*pm)(start, tokens.cend()));
			Assert::AreEqual(4, matchCount);
			Assert::IsTrue((*start)->type == token_t::INTEGER && (*start)->pos == 5);
		}
		TEST_METHOD(parsertests_multi2) {
			std::string lines[]{ "1 2 3 4", " 5 6 7 8", " 9 10 11 12", "13 14 15 16" };
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens);

			for(int i = 0; i < 4; i++) tokenizer.withNextLine(lines[i]).tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)16, tokens.size());
			int matchCount{ 0 };
			auto s = [&](PToken pt) { ++matchCount; };
			iter_t start{ tokens.cbegin() };
			iter_t stop = end_of_range(tokens.cbegin(), tokens.cend());
            Assert::IsTrue("13" == (*stop)->text);
			Assert::IsTrue( (*(match(token_t::INTEGER, s)++))(start, stop) );
			Assert::AreEqual(12, matchCount);
		}
		TEST_METHOD(parsertests_require) {
			std::deque<std::shared_ptr<Token>> tokens;
			tokens.push_back( std::make_shared<Token>( token_t::INTEGER, 1, 5, "" ) );

			bool didFail{ false };
			auto f = [&](PToken pt) { didFail = true; };

			auto preq = require(match(token_t::HEX),f);
			iter_t start{ tokens.cbegin() };
			Assert::IsFalse(didFail);
			Assert::IsFalse((*preq)(start, tokens.cend()));
			Assert::IsTrue(didFail);
		}
	};
}
