#include "pch.h"
#include "CppUnitTest.h"
#include "../basis/basis.hpp"

#include <fstream>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace basis;

namespace basistests
{
	TEST_CLASS(basistests)
	{
	public:
		TEST_METHOD(LoadNonexistentFile)
		{
			// This should fail
			std::ifstream temp;
			Assert::IsTrue ( !load_source(temp, "no_such_file") );
		}
		TEST_METHOD(LoadExtantFile)
		{
			// This should succeed in opening and readign the file
			std::ifstream file;
			Assert::IsTrue(
				load_source(file, "C:/repo/basis_lang/basis_tests/testfile1.b") );
			std::string expected("Test file 1");
			std::string file_content;
			std::getline(file, file_content);
			Assert::AreEqual(expected, file_content);
		}
	};
}
