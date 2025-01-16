
#include "pch.h"
#include "CppUnitTest.h"
#include "../basis/basis.hpp"
#include "../basis/Tokenizer.hpp"

#include <fstream>
#include <string>
#include <deque>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace basis;

namespace lexertests
{
	TEST_CLASS(lexertests)
	{
	public:
		TEST_METHOD(Test1_comments)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test1_comments");

			tokenizer.withNextLine("(* comment *) < (* comments again*)")
				.tokenize();
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::LANGLE, (int)tokens[0]->type);
		}
		TEST_METHOD(Test2_comments)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test2_comments");

			tokenizer.withNextLine("(* co (* ls *) *) < ) ; fish")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)2, tokens.size());
			Assert::AreEqual((int)token_t::LANGLE, (int)tokens[0]->type);
			Assert::AreEqual((int)token_t::RPAREN, (int)tokens[1]->type);
		}
		TEST_METHOD(Test3_comments)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test3_comments");

			tokenizer.withNextLine("; fish < ( ")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)0, tokens.size());
		}
		TEST_METHOD(Test4_comments)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test4_comments");

			tokenizer.withNextLine(" (* (* c *) c *")
				.tokenize();
			Assert::IsFalse(tokenizer.isOK());
			Assert::AreEqual((size_t)0, tokens.size());
		}
		TEST_METHOD(Test5_comments)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test5_comments");

			tokenizer.withNextLine(" >(* < (* c *) c *")
				.tokenize();
			tokenizer.withNextLine(" *) >")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)2, tokens.size());
			Assert::AreEqual((int)token_t::RANGLE, (int)tokens[0]->type);
			Assert::AreEqual((int)token_t::RANGLE, (int)tokens[1]->type);
		}
		TEST_METHOD(Test1_reswords)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test1_reswords");

			tokenizer.withNextLine(".cmd")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::COMMAND, (int)tokens[0]->type);
		}
		TEST_METHOD(Test2_reswords)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test2_reswords");

			tokenizer.withNextLine(" .cmd")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::COMMAND, (int)tokens[0]->type);
		}
		TEST_METHOD(Test3_reswords)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test3_reswords");

			tokenizer.withNextLine(" h.cmd")
				.tokenize();
			Assert::IsFalse(tokenizer.isOK());
		}
		TEST_METHOD(Test4_reswords)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test4_reswords");

			Assert::IsFalse(tokenizer.withNextLine(".cmd2").tokenize());
		}
		TEST_METHOD(Test1_identifier)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test1_identifier");

			tokenizer.withNextLine(" someName")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::IDENTIFIER, (int)tokens[0]->type);
		}
		TEST_METHOD(Test2_identifier)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test2_identifier");

			Assert::IsFalse(tokenizer.withNextLine(" name.a").tokenize());
		}
		TEST_METHOD(Test3_identifier)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test3_identifier");

			Assert::IsFalse(tokenizer.withNextLine(" name'a").tokenize());
		}
		TEST_METHOD(Test1_text)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test1_text");

			tokenizer.withNextLine(" \"lorem ipsum\"")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::TEXT, (int)tokens[0]->type);
		}
		TEST_METHOD(Test2_text)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test2_text");

			tokenizer.withNextLine(" \"quote\\\"quote\"")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::TEXT, (int)tokens[0]->type);
			Assert::AreEqual((size_t)11, tokens[0]->text.size());
		}
		TEST_METHOD(Test3_text)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test3_text");

			Assert::IsFalse(
				tokenizer.withNextLine("    \"missing end quote ").tokenize() );
		}
		TEST_METHOD(Test1_hex)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test1_hex");

			tokenizer.withNextLine("0xAF0af3").tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::HEX, (int)tokens[0]->type);
		}
		TEST_METHOD(Test2_hex)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test2_hex");

			tokenizer.withNextLine(" 0xAF0af3(").tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)2, tokens.size());
			Assert::AreEqual((int)token_t::HEX, (int)tokens[0]->type);
		}
		TEST_METHOD(Test3_hex)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test3_hex");

			Assert::IsFalse(tokenizer.withNextLine(" 0xAF0af").tokenize());
		}
		TEST_METHOD(Test4_hex)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test4_hex");

			Assert::IsFalse(tokenizer.withNextLine(" 0x F0af").tokenize());
		}
		TEST_METHOD(Test5_hex)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test5_hex");

			Assert::IsFalse(tokenizer.withNextLine(" 0xF 0af").tokenize());
		}
		TEST_METHOD(Test6_hex)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test6_hex");

			tokenizer.withNextLine(" 0xAF (").tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)2, tokens.size());
			Assert::AreEqual((int)token_t::HEX, (int)tokens[0]->type);
		}
		TEST_METHOD(Test1_number)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test1_number");

			tokenizer.withNextLine(" 42768")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::INTEGER, (int)tokens[0]->type);
			Assert::AreEqual((size_t)5, tokens[0]->text.size());
		}
		TEST_METHOD(Test2_number)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test2_number");

			tokenizer.withNextLine(" 427.68")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::DECIMAL, (int)tokens[0]->type);
			Assert::AreEqual((size_t)6, tokens[0]->text.size());
		}
		TEST_METHOD(Test3_number)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test3_number");

			tokenizer.withNextLine(" -427.68")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::DECIMAL, (int)tokens[0]->type);
			Assert::AreEqual((size_t)7, tokens[0]->text.size());
		}
		TEST_METHOD(Test4_number)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test4_number");

			tokenizer.withNextLine(" -427.68-")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)2, tokens.size());
			Assert::AreEqual((int)token_t::DECIMAL, (int)tokens[0]->type);
			Assert::AreEqual((int)token_t::HYPHEN, (int)tokens[1]->type);
			Assert::AreEqual((size_t)7, tokens[0]->text.size());
		}
		TEST_METHOD(Test5_number)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens, "test5_number");

			tokenizer.withNextLine(" 042768 ")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)1, tokens.size());
			Assert::AreEqual((int)token_t::INTEGER, (int)tokens[0]->type);
			Assert::AreEqual((size_t)6, tokens[0]->text.size());
		}
		TEST_METHOD(Test1_combo)
		{
			std::deque<std::shared_ptr<Token>> tokens;
			Tokenizer tokenizer(&tokens);

			tokenizer.withNextLine(" 'fish[3]")
				.tokenize();
			Assert::IsTrue(tokenizer.isOK());
			Assert::AreEqual((size_t)4, tokens.size());
			Assert::AreEqual((int)token_t::WRITEVAR, (int)tokens[0]->type);
			Assert::AreEqual((int)token_t::LBRACKET, (int)tokens[1]->type);
			Assert::AreEqual((int)token_t::INTEGER, (int)tokens[2]->type);
			Assert::AreEqual((int)token_t::RBRACKET, (int)tokens[3]->type);
		}
	};
}
