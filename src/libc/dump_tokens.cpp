#include <libc/dump_tokens.hpp>

#include <CLexer.h>
#include <antlr4-runtime.h>
#include <fmt/format.h>

namespace ccompiler {
static std::string_view get_token_type_name(const CLexer& lexer,
											const antlr4::Token& token) {
	return lexer.getVocabulary().getSymbolicName(token.getType());
}

void dump_tokens(std::istream& in, std::ostream& out) {
	antlr4::ANTLRInputStream stream(in);
	CLexer lexer(&stream);

	for (auto token = lexer.nextToken();
		 token->getType() != antlr4::Token::EOF; token = lexer.nextToken()) {
		out << fmt::format(
			"Loc=<{row}:{col}>\t{token_class} '{lexeme}'\n",
			fmt::arg("row", token->getLine()),
			fmt::arg("col", token->getCharPositionInLine()),
			fmt::arg("token_class", get_token_type_name(lexer, *token)),
			fmt::arg("lexeme", token->getText()));
	}
}
} // namespace ccompiler