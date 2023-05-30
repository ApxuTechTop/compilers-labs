#include <libc/ast/detail/Builder.hpp>

#include <iostream>

// static std::string trim_quotes(const std::string& str) {
// 	// NOLINTNEXTLINE
// 	if (str[0] == '"' && str[str.size() - 1] == '"') {
// 		return str.substr(1, str.size() - 2);
// 	} else {
// 		return str;
// 	}
// }

static std::string trim_less_greater(const std::string& str) {
	// NOLINTNEXTLINE
	if (str[0] == '<' && str[str.size() - 1] == '>') {
		return str.substr(1, str.size() - 2);
	} else {
		return str;
	}
}

namespace ccompiler::ast::detail {

std::any Builder::visitDocument(CParser::DocumentContext* context) {
	auto* value = std::any_cast<Value*>(visitChildren(context));
	document.set_value(value);
	return value;
}

std::any Builder::visitElements(CParser::ElementsContext* context) {
	std::vector<Value*> elements;
	for (const auto& melement : context->element()) {
		elements.push_back(std::any_cast<Value*>(visit(melement)));
	}
	return static_cast<Value*>(document.create_node<Elements>(elements));
}

std::any Builder::visitIncludeHeader(CParser::IncludeHeaderContext* context) {
	std::string header_name
		= trim_less_greater(context->HEADERNAME()->getText());
	return static_cast<Value*>(
		document.create_node<IncludeHeader>(header_name));
}

std::any Builder::visitAnyType(CParser::AnyTypeContext* context) {
	std::string type_name = context->typeName()->getText();
	Type::Qualifiers qualifiers;
	for (const auto& qualifier : context->typeQualifier()) {
		std::string qualifier_text = qualifier->getText();
		if (qualifier_text == "const") {
			qualifiers.push_back(Type::QualifierType::Const);
		} else if (qualifier_text == "volatile") {
			qualifiers.push_back(Type::QualifierType::Volatile);
		} else if (qualifier_text == "restrict") {
			qualifiers.push_back(Type::QualifierType::Restrict);
		} else if (qualifier_text == "*") {
			qualifiers.push_back(Type::QualifierType::Pointer);
		}
	}
	return static_cast<Value*>(document.create_node<Type>(
		std::move(type_name), std::move(qualifiers)));
}

std::any Builder::visitFunctionDefine(CParser::FunctionDefineContext* context) {
	std::string function_name = context->functionName()->getText();
	Value* function_out_type = std::any_cast<Value*>(visit(context->anyType()));
	FunctionDeclaration::FunctionParameters parameters;
	for (const auto& parameter_context : context->functionParameters()->functionParameter()) {
		parameters.emplace_back(std::any_cast<Value*>(visit(parameter_context->anyType())), parameter_context->varId()->getText());
	}
	Value* block = nullptr;
	if (context->block()) {
		block = std::any_cast<Value*>(visit(context->block()));
	}
	return static_cast<Value*>(document.create_node<FunctionDeclaration>(function_out_type, std::move(function_name), std::move(parameters), block));
}

std::any Builder::visitGlobalInitializing(CParser::GlobalInitializingContext* context) {
	return visit(context->varDeclaration());
}

std::any Builder::visitVarDeclaration(CParser::VarDeclarationContext* context) {
	Value* var_type = std::any_cast<Value*>(visit(context->anyType()));
	VarDeclarations::Declarations declarations;
	for (const auto& assignment_context : context->varAssignment()) {
		std::string var_name = assignment_context->varId()->getText();
		Value* init_expression = nullptr;
		if (assignment_context->expression()) {
			init_expression = std::any_cast<Value*>(visit(assignment_context->expression()));
		}
		declarations.emplace_back(var_name, init_expression);
	}
	return static_cast<Value*>(document.create_node<VarDeclarations>(var_type, std::move(declarations)));
}

std::any Builder::visitBlock(CParser::BlockContext* context) {
	Block::Statements statements;
	for (const auto& statement_context : context->statement()) {
		statements.emplace_back(std::any_cast<Value*>(visit(statement_context)));
	}
	return static_cast<Value*>(document.create_node<Block>(std::move(statements)));
}

std::any Builder::visitBlockState(CParser::BlockStateContext* context) {
	return visit(context->block());
}
std::any Builder::visitDeclState(CParser::DeclStateContext* context) {
	return static_cast<Value*>(document.create_node<Statement>(std::any_cast<Value*>(visit(context->varDeclaration()))));
}
std::any Builder::visitExprState(CParser::ExprStateContext* context) {
	return static_cast<Value*>(document.create_node<Statement>(std::any_cast<Value*>(visit(context->expression()))));
}
std::any Builder::visitEmptyState(CParser::EmptyStateContext*) {
	return static_cast<Value*>(nullptr);
}
std::any Builder::visitReturnState(CParser::ReturnStateContext* context) {
	return static_cast<Value*>(document.create_node<Statement>(static_cast<Value*>(document.create_node<ReturnStatement>(std::any_cast<Value*>(visit(context->expression()))))));
}

std::any Builder::visitPostfixExpr(CParser::PostfixExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(std::any_cast<Value*>(visit(context->expression())));
	std::string op_type = context->postfixOperation()->getText() + "int";
	return static_cast<Value*>(document.create_node<Expression>(std::move(op_type), std::move(args)));
}
std::any Builder::visitPrefixExpr(CParser::PrefixExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(std::any_cast<Value*>(visit(context->expression())));
	std::string op_type = context->prefixOperation()->getText();
	return static_cast<Value*>(document.create_node<Expression>(std::move(op_type), std::move(args)));
}
std::any Builder::visitFunctionExpr(CParser::FunctionExprContext* context) {
	Expression::Arguments args;
	std::string function_name = context->varId()->getText();
	FunctionCall::Arguments function_args;
	std::cout << "Im here\n";
	for (const auto& arg_context : context->args) {
		std::cout << "arg: " << arg_context->getText() << '\n';
		function_args.emplace_back(std::any_cast<Value*>(visit(arg_context)));
	}
	Value* function = document.create_node<FunctionCall>(std::move(function_name), std::move(function_args));
	args.emplace_back(function);
	return static_cast<Value*>(document.create_node<Expression>("call", std::move(args)));
}
std::any Builder::visitSubscriptExpr(CParser::SubscriptExprContext* context) {
	Expression::Arguments args;
	for (const auto& expr_context : context->expression()) {
		args.emplace_back(std::any_cast<Value*>(visit(expr_context)));
	}
	return static_cast<Value*>(document.create_node<Expression>("[]", std::move(args)));
}
std::any Builder::visitParensExpr(CParser::ParensExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(std::any_cast<Value*>(visit(context->expression())));
	return static_cast<Value*>(document.create_node<Expression>("()", std::move(args)));
}
std::any Builder::visitUnaryExpr(CParser::UnaryExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(std::any_cast<Value*>(visit(context->expression())));
	return static_cast<Value*>(document.create_node<Expression>(context->unaryOperation()->getText(), std::move(args)));
}
std::any Builder::visitBinaryExpr(CParser::BinaryExprContext* context) {
	Expression::Arguments args;
	for (const auto& expr_context : context->expression()) {
		args.emplace_back((std::any_cast<Value*>(visit(expr_context))));
	}
	return static_cast<Value*>(document.create_node<Expression>(context->binaryOperation()->getText(), std::move(args)));
}
std::any Builder::visitTernaryExpr(CParser::TernaryExprContext* context) {
	Expression::Arguments args;
	for (const auto& expr_context : context->expression()) {
		args.emplace_back((std::any_cast<Value*>(visit(expr_context))));
	}
	return static_cast<Value*>(document.create_node<Expression>("?:", std::move(args)));
}
std::any Builder::visitAssignmentExpr(CParser::AssignmentExprContext* context) {
	Expression::Arguments args;
	for (const auto& expr_context : context->expression()) {
		args.emplace_back((std::any_cast<Value*>(visit(expr_context))));
	}
	return static_cast<Value*>(document.create_node<Expression>(context->assignmentOperation()->getText(), std::move(args)));
}
std::any Builder::visitNameExpr(CParser::NameExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(document.create_node<Variable>(context->varId()->getText()));
	return static_cast<Value*>(document.create_node<Expression>("var", std::move(args)));
}
std::any Builder::visitConstExpr(CParser::ConstExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(document.create_node<Variable>(context->constValue()->getText()));
	return static_cast<Value*>(document.create_node<Expression>("literal", std::move(args)));
}

} // namespace ccompiler::ast::detail