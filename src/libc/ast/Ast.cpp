#include <libc/ast/Ast.hpp>

#include <libc/ast/Visitor.hpp>

#include <libc/ast/TypeChecker.hpp>
namespace ccompiler::ast {

void Elements::accept(Visitor& visitor) { visitor.visit(*this); }

void IncludeHeader::accept(Visitor& visitor) { visitor.visit(*this); }

void Type::accept(Visitor& visitor) { visitor.visit(*this); }

bool operator==(Type& lhs, Type& rhs) {
	return (lhs.name() == rhs.name())
		   && (lhs.qualifiers() == rhs.qualifiers());
}

bool operator!=(Type& lhs, Type& rhs) { return !(lhs == rhs); }

void VarDeclarations::accept(Visitor& visitor) { visitor.visit(*this); }
void VarDeclarations::Declaration::accept(Visitor& visitor) {
	visitor.visit(*this);
}

void Statement::accept(Visitor& visitor) { visitor.visit(*this); }

void ReturnStatement::accept(Visitor& visitor) { visitor.visit(*this); }

void Block::accept(Visitor& visitor) { visitor.visit(*this); }

void FunctionDeclaration::accept(Visitor& visitor) { visitor.visit(*this); }
Type FunctionDeclaration::runtime_type(TypeChecker& visitor) {
	return visitor.visit(*this);
}
Type FunctionDeclaration::FunctionParameter::runtime_type(
	TypeChecker& visitor) {
	return visitor.visit(*this);
}

void Expression::accept(Visitor& visitor) { visitor.visit(*this); }
Type Expression::runtime_type(TypeChecker& visitor) {
	return visitor.visit(*this);
}

void FunctionCall::accept(Visitor& visitor) { visitor.visit(*this); }
Type FunctionCall::runtime_type(TypeChecker& visitor) {
	return visitor.visit(*this);
}

void Variable::accept(Visitor& visitor) { visitor.visit(*this); }
Type Variable::runtime_type(TypeChecker& visitor) {
	return visitor.visit(*this);
}

void Literal::accept(Visitor& visitor) { visitor.visit(*this); }
Type CharLiteral::runtime_type(TypeChecker& visitor) {
	return visitor.visit(*this);
}
Type StringLiteral::runtime_type(TypeChecker& visitor) {
	return visitor.visit(*this);
}
Type IntegerLiteral::runtime_type(TypeChecker& visitor) {
	return visitor.visit(*this);
}
Type FloatLiteral::runtime_type(TypeChecker& visitor) {
	return visitor.visit(*this);
}

} // namespace ccompiler::ast
