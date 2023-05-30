#pragma once

#include <libc/ast/Ast.hpp>
#include <string>
#include <vector>
#include <iosfwd>

namespace ccompiler {

struct Error {
	std::size_t line;
	std::size_t column;
	std::string message;
	Error(std::size_t l, std::size_t c, std::string m) : line(l), column(c), message(m) {}
};
using Errors = std::vector<Error>;
struct ParseResult {
	ParseResult(ast::Document doc) : document(std::move(doc)) {}
	ParseResult(Errors errs) : errors(std::move(errs)) {}

	ast::Document document;
	Errors errors;
};

ParseResult parse(std::istream& in);
void dump_ast(ast::Document& document, std::ostream& out);
void dump_errors(const Errors& errors, std::ostream& out);

} // namespace ccompiler