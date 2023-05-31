#include <fmt/format.h>
#include <libc/ast/ScopeChecker.hpp>
#include <libc/ast/TypeChecker.hpp>

namespace ccompiler::ast {

void ScopeChecker::exec(Document& document, std::ostream& out) {
	Scope global_scope;
	scopes.push_back(global_scope);
	document.get_value()->accept(*this);
	scopes.pop_back();
	for (const auto& error : errors) {
		out << error << '\n';
	}
}

void ScopeChecker::visit(Elements& value) {
	for (const auto& element : value.elements()) {
		element->accept(*this);
	}
}

void ScopeChecker::visit(IncludeHeader& /*value*/) {
	// на будущее если в файле есть глобальные переменные
}

void ScopeChecker::visit(Type& /*value*/) {
	// проверить тип
}

void ScopeChecker::visit(VarDeclarations& value) {
	for (auto& declaration : value.declarations()) {
		declaration.accept(*this);
	}
}
void ScopeChecker::visit(VarDeclarations::Declaration& value) {
	if (scopes.back().symbols.contains(value.name)) {
		const auto& ref = scopes.back().symbols.at(value.name).ref;
		std::visit(
			[&](auto& old_value) {
				errors.emplace_back(
					fmt::format("{}:{} Redefinition variable {}, "
								"previous definition was here {}:{}\n",
								value.line, value.column, value.name,
								old_value.line, old_value.column));
			},
			ref);

	} else {
		scopes.back().symbols.emplace(value.name, value);
		// scopes.back().symbols[value.name] = Symbol(value);
	}
	value.type()->accept(*this);
	if (value.init_expression != nullptr) {
		Type value_type = *dynamic_cast<Type*>(value.type());
		TypeChecker type_visitor(scopes);
		Type init_expression_type
			// = dynamic_cast<Expression*>(value.init_expression)
			// 	  ->runtime_type(type_visitor);
			= type_visitor.visit(value.init_expression);
		if (value_type != init_expression_type) {
			errors.emplace_back(fmt::format(
				"{}:{} Expected type {}, got {}", value.line, value.column,
				value_type.name(), init_expression_type.name()));
		}
		value.init_expression->accept(*this);
	}
}

void ScopeChecker::visit(Statement& value) { value.state()->accept(*this); }

void ScopeChecker::visit(ReturnStatement& value) {
	value.expression()->accept(*this);
}

void ScopeChecker::visit(Block& value) {
	Scope new_scope;
	scopes.push_back(new_scope);
	for (const auto& statement : value.statements()) {
		statement->accept(*this);
	}
	scopes.pop_back();
}

void ScopeChecker::visit(FunctionDeclaration& value) {
	if (scopes.front().symbols.contains(value.name())) {
		const auto& ref = scopes.front().symbols.at(value.name()).ref;

		std::visit(
			[&](auto& old_value) {
				errors.emplace_back(
					fmt::format("{}:{} Redefinition function {}, "
								"previous definition was here {}:{}\n",
								value.line, value.column, value.name(),
								old_value.line, old_value.column));
			},
			ref);
	}
	scopes.front().symbols.emplace(value.name(), value);
	// scopes.front().symbols[value.name()] = Symbol(value);
	Scope new_scope;
	scopes.push_back(new_scope);
	value.type()->accept(*this);
	for (auto& var : value.parameters()) {
		scopes.front().symbols.emplace(var.name, var);
		// scopes.back().symbols[var.name] = Symbol(var);
		var.type()->accept(*this);
	}
	if (value.block() != nullptr) {
		// value.block()->accept(*this);
		Block& block = *(dynamic_cast<Block*>(value.block()));
		for (const auto& statement : block.statements()) {
			statement->accept(*this);
		}
	}
	scopes.pop_back();
}

void ScopeChecker::visit(Expression& value) {
	for (const auto& arg : value.arguments()) {
		arg->accept(*this);
	}
	TypeChecker type_visitor(scopes);
	Type first_arg_type = value.runtime_type(type_visitor);
	for (auto*& arg : value.arguments()) {
		Type arg_type = type_visitor.visit(arg);
		if (first_arg_type != arg_type) {
			errors.emplace_back(fmt::format(
				"{}:{} Expected type {}, got {}", arg->line, arg->column,
				first_arg_type.name(), arg_type.name()));
		}
	}
}

void ScopeChecker::visit(FunctionCall& value) {
	if (!scopes.front().symbols.contains(value.name())) {
		errors.emplace_back(
			fmt::format("{}:{} Reference to undefined function {}\n",
						value.line, value.column, value.name()));
	}
	for (const auto& arg : value.arguments()) {
		arg->accept(*this);
	}
}

void ScopeChecker::visit(Variable& value) {
	bool isFinded = false;
	//
	// auto & scope : std::ranges::reverse_view(scopes)
	for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
		auto& scope = *it;
		if (scope.symbols.contains(value.name())) {
			isFinded = true;
			break;
		}
	}
	if (!isFinded) {
		errors.emplace_back(
			fmt::format("{}:{} Reference to undefined variable {}\n",
						value.line, value.column, value.name()));
	}
}
void ScopeChecker::visit(Literal& /*value*/) {}

} // namespace ccompiler::ast