#pragma once

#include <libc/ast/Visitor.hpp>
#include <ostream>
#include <stack>
#include <unordered_map>

namespace ccompiler::ast {

class ScopeChecker final : public Visitor {

  public:
	void exec(Document& document, std::ostream& out);
	void visit(Elements& value) override;

	void visit(IncludeHeader& value) override;
	void visit(Type& value) override;

	void visit(VarDeclarations& value) override;
	void visit(VarDeclarations::Declaration& value) override;
	void visit(Statement& value) override;
	void visit(ReturnStatement& value) override;
	void visit(Block& value) override;
	void visit(FunctionDeclaration& value) override;
	void visit(Expression& value) override;
	void visit(FunctionCall& value) override;
	void visit(Variable& value) override;
	void visit(Literal& value) override;
	class Symbol {
	  private:
	  public:
		Symbol(VarDeclarations::Declaration& value) {
			line = value.line;
			column = value.column;
			name = value.name;
		}
		Symbol(FunctionDeclaration& value) {
			line = value.line;
			column = value.column;
			name = value.name();
		}
		Symbol(FunctionDeclaration::FunctionParameter& value) {
			line = value.line;
			column = value.column;
			name = value.name;
		}
		std::size_t line;
		std::size_t column;
		std::string name;
	};
	class Scope {
	  public:
		std::unordered_map<std::string, Symbol> symbols;
	};

  private:
	std::vector<std::string> errors;
	std::vector<Scope> scopes;
};

} // namespace ccompiler::ast