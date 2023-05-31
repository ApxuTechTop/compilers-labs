#pragma once

#include <CBaseVisitor.h>
#include <libc/ast/Ast.hpp>
#include <libc/parser.hpp>

namespace ccompiler::ast::detail {

class Builder final : public CBaseVisitor {
  private:
	ast::Document& document;

  public:
	explicit Builder(ast::Document& doc) : document(doc) {}
	std::any visitDocument(CParser::DocumentContext* context) override;
	std::any visitElements(CParser::ElementsContext* context) override;
	std::any
	visitIncludeHeader(CParser::IncludeHeaderContext* context) override;
	std::any
	visitFunctionDefine(CParser::FunctionDefineContext* context) override;
	std::any visitGlobalInitializing(
		CParser::GlobalInitializingContext* context) override;
	std::any visitAnyType(CParser::AnyTypeContext* context) override;
	std::any
	visitVarDeclaration(CParser::VarDeclarationContext* context) override;
	std::any visitBlock(CParser::BlockContext* context) override;
	std::any visitBlockState(CParser::BlockStateContext* context) override;
	std::any visitDeclState(CParser::DeclStateContext* context) override;
	std::any visitExprState(CParser::ExprStateContext* context) override;
	std::any visitEmptyState(CParser::EmptyStateContext* context) override;
	std::any visitReturnState(CParser::ReturnStateContext* context) override;

	std::any visitPostfixExpr(CParser::PostfixExprContext* context) override;
	std::any visitPrefixExpr(CParser::PrefixExprContext* context) override;
	std::any visitFunctionExpr(CParser::FunctionExprContext* context) override;
	std::any
	visitSubscriptExpr(CParser::SubscriptExprContext* context) override;
	std::any visitParensExpr(CParser::ParensExprContext* context) override;
	std::any visitUnaryExpr(CParser::UnaryExprContext* context) override;
	std::any visitBinaryExpr(CParser::BinaryExprContext* context) override;
	std::any visitTernaryExpr(CParser::TernaryExprContext* context) override;
	std::any
	visitAssignmentExpr(CParser::AssignmentExprContext* context) override;
	std::any visitNameExpr(CParser::NameExprContext* context) override;
	std::any visitConstExpr(CParser::ConstExprContext* context) override;
};

} // namespace ccompiler::ast::detail