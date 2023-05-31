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
	auto* value = std::any_cast<Base*>(visitChildren(context));
	document.set_value(value);
	return value;
}

std::any Builder::visitElements(CParser::ElementsContext* context) {
	std::vector<Base*> elements;
	for (const auto& melement : context->element()) {
		elements.push_back(std::any_cast<Base*>(visit(melement)));
	}
	return static_cast<Base*>(document.create_node<Elements>(elements));
}

std::any Builder::visitIncludeHeader(CParser::IncludeHeaderContext* context) {
	std::string header_name
		= trim_less_greater(context->HEADERNAME()->getText());
	auto* ptr = document.create_node<IncludeHeader>(header_name);
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
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
	auto* ptr = document.create_node<Type>(std::move(type_name),
										   std::move(qualifiers));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}

std::any
Builder::visitFunctionDefine(CParser::FunctionDefineContext* context) {
	std::string function_name = context->functionName()->getText();
	Base* function_out_type = std::any_cast<Base*>(visit(context->anyType()));
	FunctionDeclaration::FunctionParameters parameters;
	for (const auto& parameter_context :
		 context->functionParameters()->functionParameter()) {
		auto& parameter = parameters.emplace_back(
			std::any_cast<Base*>(visit(parameter_context->anyType())),
			parameter_context->varId()->getText());
		parameter.line = parameter_context->start->getLine();
		parameter.column = parameter_context->start->getCharPositionInLine();
	}
	Base* block = nullptr;
	if (context->block()) {
		block = std::any_cast<Base*>(visit(context->block()));
	}
	auto* ptr = document.create_node<FunctionDeclaration>(
		function_out_type, std::move(function_name), std::move(parameters),
		block);
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}

std::any
Builder::visitGlobalInitializing(CParser::GlobalInitializingContext* context) {
	return visit(context->varDeclaration());
}

std::any
Builder::visitVarDeclaration(CParser::VarDeclarationContext* context) {
	Base* var_type = std::any_cast<Base*>(visit(context->anyType()));
	VarDeclarations::Declarations declarations;
	for (const auto& assignment_context : context->varAssignment()) {
		std::string var_name = assignment_context->varId()->getText();
		Base* init_expression = nullptr;
		if (assignment_context->expression()) {
			init_expression = std::any_cast<Base*>(
				visit(assignment_context->expression()));
		}
		auto& declaration
			= declarations.emplace_back(var_name, init_expression, var_type);
		declaration.line = assignment_context->start->getLine();
		declaration.column
			= assignment_context->start->getCharPositionInLine();
	}
	auto* ptr = document.create_node<VarDeclarations>(var_type,
													  std::move(declarations));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}

std::any Builder::visitBlock(CParser::BlockContext* context) {
	Block::Statements statements;
	for (const auto& statement_context : context->statement()) {
		statements.emplace_back(
			std::any_cast<Base*>(visit(statement_context)));
	}
	auto* ptr = document.create_node<Block>(std::move(statements));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}

std::any Builder::visitBlockState(CParser::BlockStateContext* context) {
	return visit(context->block());
}
std::any Builder::visitDeclState(CParser::DeclStateContext* context) {
	auto* ptr = document.create_node<Statement>(
		std::any_cast<Base*>(visit(context->varDeclaration())));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitExprState(CParser::ExprStateContext* context) {
	auto* ptr = document.create_node<Statement>(
		std::any_cast<Base*>(visit(context->expression())));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitEmptyState(CParser::EmptyStateContext*) {
	return static_cast<Base*>(nullptr);
}
std::any Builder::visitReturnState(CParser::ReturnStateContext* context) {
	auto* ptr = static_cast<Base*>(document.create_node<ReturnStatement>(
		std::any_cast<Base*>(visit(context->expression()))));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(document.create_node<Statement>(ptr));
}

std::any Builder::visitPostfixExpr(CParser::PostfixExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(std::any_cast<Base*>(visit(context->expression())));
	std::string op_type = context->postfixOperation()->getText() + "int";
	auto* ptr = document.create_node<Expression>(std::move(op_type),
												 std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitPrefixExpr(CParser::PrefixExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(std::any_cast<Base*>(visit(context->expression())));
	std::string op_type = context->prefixOperation()->getText();
	auto* ptr = document.create_node<Expression>(std::move(op_type),
												 std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitFunctionExpr(CParser::FunctionExprContext* context) {
	Expression::Arguments args;
	std::string function_name = context->varId()->getText();
	FunctionCall::Arguments function_args;
	for (const auto& arg_context : context->args) {
		function_args.emplace_back(std::any_cast<Base*>(visit(arg_context)));
	}
	Base* function = document.create_node<FunctionCall>(
		std::move(function_name), std::move(function_args));
	args.emplace_back(function);
	auto* ptr = document.create_node<Expression>("call", std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitSubscriptExpr(CParser::SubscriptExprContext* context) {
	Expression::Arguments args;
	for (const auto& expr_context : context->expression()) {
		args.emplace_back(std::any_cast<Base*>(visit(expr_context)));
	}
	auto* ptr = document.create_node<Expression>("[]", std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitParensExpr(CParser::ParensExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(std::any_cast<Base*>(visit(context->expression())));
	auto* ptr = document.create_node<Expression>("()", std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitUnaryExpr(CParser::UnaryExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(std::any_cast<Base*>(visit(context->expression())));
	auto* ptr = document.create_node<Expression>(
		context->unaryOperation()->getText(), std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitBinaryExpr(CParser::BinaryExprContext* context) {
	Expression::Arguments args;
	for (const auto& expr_context : context->expression()) {
		args.emplace_back((std::any_cast<Base*>(visit(expr_context))));
	}
	auto* ptr = document.create_node<Expression>(
		context->binaryOperation()->getText(), std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitTernaryExpr(CParser::TernaryExprContext* context) {
	Expression::Arguments args;
	for (const auto& expr_context : context->expression()) {
		args.emplace_back((std::any_cast<Base*>(visit(expr_context))));
	}
	auto* ptr = document.create_node<Expression>("?:", std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any
Builder::visitAssignmentExpr(CParser::AssignmentExprContext* context) {
	Expression::Arguments args;
	for (const auto& expr_context : context->expression()) {
		args.emplace_back((std::any_cast<Base*>(visit(expr_context))));
	}
	auto* ptr = document.create_node<Expression>(
		context->assignmentOperation()->getText(), std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitNameExpr(CParser::NameExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(
		document.create_node<Variable>(context->varId()->getText()));
	auto* ptr = document.create_node<Expression>("var", std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}
std::any Builder::visitConstExpr(CParser::ConstExprContext* context) {
	Expression::Arguments args;
	args.emplace_back(
		document.create_node<Literal>(context->constValue()->getText()));
	auto* ptr = document.create_node<Expression>("literal", std::move(args));
	ptr->line = context->start->getLine();
	ptr->column = context->start->getCharPositionInLine();
	return static_cast<Base*>(ptr);
}

} // namespace ccompiler::ast::detail