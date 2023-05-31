#pragma once

#include <libc/ast/Ast.hpp>
#include <libc/ast/ScopeChecker.hpp>
#include <ostream>

namespace ccompiler::ast {

class TypeChecker {
  public:
	std::vector<ScopeChecker::Scope> scopes;
	TypeChecker(std::vector<ScopeChecker::Scope> scopes_) : scopes(scopes_) {}
	Type visit(Base* value) {
		if (Expression* arg = dynamic_cast<Expression*>(value)) {
			return visit(*arg);
		} else if (FunctionCall* arg = dynamic_cast<FunctionCall*>(value)) {
			return visit(*arg);
		} else if (Variable* arg = dynamic_cast<Variable*>(value)) {
			return visit(*arg);
		} else if (StringLiteral* arg = dynamic_cast<StringLiteral*>(
					   dynamic_cast<Literal*>(value))) {
			return visit(*arg);
		} else if (CharLiteral* arg = dynamic_cast<CharLiteral*>(
					   dynamic_cast<Literal*>(value))) {
			return visit(*arg);
		} else if (IntegerLiteral* arg = dynamic_cast<IntegerLiteral*>(
					   dynamic_cast<Literal*>(value))) {
			return visit(*arg);
		} else if (FloatLiteral* arg = dynamic_cast<FloatLiteral*>(
					   dynamic_cast<Literal*>(value))) {
			return visit(*arg);
		}
		throw;
	}
	Type visit(Expression& value) {
		auto& args = value.arguments();
		auto* first_arg = args[0];
		return visit(first_arg);
	}
	Type visit(FunctionDeclaration& value) {
		return *static_cast<Type*>(value.type());
	}
	Type visit(FunctionDeclaration::FunctionParameter& value) {
		return *static_cast<Type*>(value.type());
	}
	Type visit(FunctionCall& value) {
		auto& ref = scopes.front().symbols.at(value.name()).ref;
		Base* ptr = nullptr;
		std::visit([&](auto& k) { ptr = k.type(); }, ref);
		return *dynamic_cast<Type*>(ptr);
	}
	Type visit(Variable& value) {
		for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
			auto& scope = *it;
			if (scope.symbols.contains(value.name())) {
				auto& ref = scope.symbols.at(value.name()).ref;
				Base* ptr = nullptr;
                std::visit([&](auto& k) { ptr = k.type(); }, ref);
				return *dynamic_cast<Type*>(ptr);
			}
		}
		throw;
	}
	Type visit(StringLiteral& /*value*/) {
		return { "char",
				 { Type::QualifierType::Const,
				   Type::QualifierType::Pointer } };
	}
	Type visit(CharLiteral& /*value*/) { return { "char", {} }; }
	Type visit(IntegerLiteral& /*value*/) { return { "int", {} }; }
	Type visit(FloatLiteral& /*value*/) { return { "float", {} }; }
	Type visit(ReturnStatement& value) {
		return visit(*static_cast<Expression*>(value.expression()));
	}
};

} // namespace ccompiler::ast