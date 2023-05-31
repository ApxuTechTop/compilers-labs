#include <libc/ast/Ast.hpp>

#include <libc/ast/Visitor.hpp>

namespace ccompiler::ast {

void Elements::accept(Visitor& visitor) { visitor.visit(*this); }

void IncludeHeader::accept(Visitor& visitor) { visitor.visit(*this); }

void Type::accept(Visitor& visitor) { visitor.visit(*this); }

void VarDeclarations::accept(Visitor& visitor) { visitor.visit(*this); }
void VarDeclarations::Declaration::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void Statement::accept(Visitor& visitor) { visitor.visit(*this); }

void ReturnStatement::accept(Visitor& visitor) { visitor.visit(*this); }

void Block::accept(Visitor& visitor) { visitor.visit(*this); }

void FunctionDeclaration::accept(Visitor& visitor) { visitor.visit(*this); }

void Expression::accept(Visitor& visitor) { visitor.visit(*this); }

void FunctionCall::accept(Visitor& visitor) { visitor.visit(*this); }

void Variable::accept(Visitor& visitor) { visitor.visit(*this); }

void Literal::accept(Visitor& visitor) { visitor.visit(*this); }

} // namespace ccompiler::ast
