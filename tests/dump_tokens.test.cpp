#include <gtest/gtest.h>

#include <libc/dump_tokens.hpp>

#include <sstream>
#include <string>

TEST(LexerSuite, ValidNumbers) {
	// Given
	std::stringstream in("0 123456789 3. .99 1e6 14.8e8 7.1E-3");

	// When
	std::stringstream out;
	ccompiler::dump_tokens(in, out);

	// Then
	EXPECT_EQ(out.str(), "Loc=<1:0>\tINTEGER_CONST '0'\n"
						 "Loc=<1:2>\tINTEGER_CONST '123456789'\n"
						 "Loc=<1:12>\tFLOAT_CONST '3.'\n"
						 "Loc=<1:15>\tFLOAT_CONST '.99'\n"
						 "Loc=<1:19>\tFLOAT_CONST '1e6'\n"
						 "Loc=<1:23>\tFLOAT_CONST '14.8e8'\n"
						 "Loc=<1:30>\tFLOAT_CONST '7.1E-3'\n");
}

TEST(LexerSuite, ValidStrings) {
	std::stringstream in("\"some string\" ']'");

	std::stringstream out;
	ccompiler::dump_tokens(in, out);

	EXPECT_EQ(out.str(), "Loc=<1:0>\tSTRING_CONST '\"some string\"'\n"
						 "Loc=<1:14>\tCHAR_CONST '\']\''\n");
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
