#pragma once

#include <libc/ast/Ast.hpp>

namespace ccompiler::ast {

class Visitor {
  public:
	virtual void visit(Elements& value) = 0;
	virtual void visit(IncludeHeader& value) = 0;
	virtual void visit(Type& value) = 0;
	virtual void visit(VarDeclarations& value) = 0;
	virtual void visit(VarDeclarations::Declaration& value) = 0;
	virtual void visit(Statement& value) = 0;
	virtual void visit(ReturnStatement& value) = 0;
	virtual void visit(Block& value) = 0;
	virtual void visit(FunctionDeclaration& value) = 0;
	virtual void visit(Expression& value) = 0;
	virtual void visit(FunctionCall& value) = 0;
	virtual void visit(Variable& value) = 0;
	virtual void visit(Literal& value) = 0;
};

} // namespace ccompiler::ast
