#pragma once

#include <libc/ast/Visitor.hpp>

#include <pugixml.hpp>

#include <ostream>
#include <stack>

namespace ccompiler::ast {

class XmlSerializer final : public Visitor {
  public:
	static void exec(Document& document, std::ostream& out);
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

  private:
	pugi::xml_node append_child(const char* name);
	void append_text(const char* text);

	pugi::xml_document doc;
	std::stack<pugi::xml_node> nodes;
};

} // namespace c_space::ast
