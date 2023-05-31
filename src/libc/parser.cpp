
#include <CLexer.h>
#include <CParser.h>
#include <fmt/format.h>
#include <libc/ast/ScopeChecker.hpp>
#include <libc/ast/XmlSerializer.hpp>
#include <libc/ast/detail/Builder.hpp>
#include <libc/parser.hpp>

namespace ccompiler {

class StreamErrorListener : public antlr4::BaseErrorListener {
  private:
	Errors errs;

  public:
	void syntaxError(antlr4::Recognizer*, antlr4::Token*, std::size_t line,
					 std::size_t column, const std::string& message,
					 std::exception_ptr /*e*/) override {
		errs.emplace_back(line, column, message);
	}
	const Errors& errors() const { return errs; }
};

ParseResult parse(std::istream& in) {
	antlr4::ANTLRInputStream stream(in);
	CLexer lexer(&stream);
	antlr4::CommonTokenStream tokens(&lexer);
	CParser parser(&tokens);

	StreamErrorListener error_listener;
	parser.removeErrorListeners();
	parser.addErrorListener(&error_listener);

	auto* document_parse_tree = parser.document();

	const auto& errors = error_listener.errors();
	if (!errors.empty()) {
		return { errors };
	}

	ast::Document document;
	ast::detail::Builder builder(document);
	builder.visit(document_parse_tree);

	return { std::move(document) };
}

void dump_ast(ast::Document& document, std::ostream& out) {
	ast::XmlSerializer::exec(document, out);
}

void dump_errors(const Errors& errors, std::ostream& out) {
	for (const auto& error : errors) {
		out << fmt::format("{}:{} {}\n", error.line, error.column,
						   error.message);
	}
}

bool check_symbols(ast::Document& document, std::ostream& out) {
	ast::ScopeChecker checker;
	checker.exec(document, out);
	return true;
}

} // namespace ccompiler