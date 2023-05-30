#pragma once

#include <memory>
#include <vector>

namespace ccompiler::ast {

class String;
class Value;
class Visitor;

class Document final {
  public:
	template <class T, class... Args> T* create_node(Args&&... args) {
		static_assert(std::is_base_of_v<Value, T>);
		values_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		return dynamic_cast<T*>(values_.back().get());
	}

	void set_value(Value* value) { value_ = value; }
	ast::Value* get_value() const { return value_; }

  private:
	std::vector<std::unique_ptr<Value>> values_;
	ast::Value* value_ = nullptr;
};

class Value {
  public:
	virtual ~Value() = default;
	virtual void accept(Visitor& visitor) = 0;
};

class Elements : public Value {
  public:
	Elements(std::vector<Value*> elements) : elems(std::move(elements)) {}
	void accept(Visitor& visitor) override;
	const std::vector<Value*> elements() const {return elems;}
  private:
	std::vector<Value*> elems;
};

class IncludeHeader : public Value {
  public:
	IncludeHeader(std::string str) : lib_name(std::move(str)) {}
	std::string& name() { return lib_name; }
	void accept(Visitor& visitor) override;

  private:
	std::string lib_name;
};

class Type : public Value {
  public:
	enum class QualifierType { Const, Pointer, Volatile, Restrict };
	using Qualifiers = std::vector<QualifierType>;
	Type(std::string name, Qualifiers qs)
		: type_name(std::move(name)), type_qualifiers(std::move(qs)) {}
	std::string& name() { return type_name; }
	Qualifiers& qualifiers() { return type_qualifiers; }
	void accept(Visitor& visitor) override;

  private:
	std::string type_name;
	Qualifiers type_qualifiers;
};

class VarDeclarations : public Value {
  public:
	struct Declaration {
		std::string name;
		Value* init_expression;
		Declaration(std::string var_name, Value* init) : name(std::move(var_name)), init_expression(init) {}
	};
	using Declarations = std::vector<Declaration>;
	VarDeclarations(Value* type, Declarations declarations)
		: var_type(type), var_declarations(std::move(declarations)) {}
	Value* type() { return var_type; }
	Declarations& declarations() { return var_declarations; }
	void accept(Visitor& visitor) override;

  private:
	Value* var_type;
	Declarations var_declarations;
};

class Statement : public Value {
  public:
	Statement(Value* value) : some_state(value) {}
	void accept(Visitor& visitor) override;
	Value* state() {return some_state;}
  private:
	Value* some_state;
};

class ReturnStatement : public Value {
	public:
	ReturnStatement(Value* value) : return_expression(value) {}
	Value* expression() {return return_expression;}
	void accept(Visitor& visitor) override;
	private:
	Value* return_expression;
};
class WhileStatement : public Value {};
class ForStatement : public Value {};
class SwitchStatement : public Value {};

class Block : public Value {
  public:
	using Statements = std::vector<Value*>;
	Block(Statements states) : block_statements(std::move(states)) {}
	Statements& statements() { return block_statements; }
	void accept(Visitor& visitor) override;

  private:
	Statements block_statements;
};

class FunctionDeclaration : public Value {
  public:
	struct FunctionParameter {
		Value* type;
		std::string name;
		FunctionParameter(Value* parameter_type, std::string parameter_name) : type(parameter_type), name(std::move(parameter_name)) {}
	};
	using FunctionParameters = std::vector<FunctionParameter>;
	FunctionDeclaration(Value* f_type, std::string f_name,
						FunctionParameters f_parameters, Value* block)
		: out_type(f_type), function_name(std::move(f_name)),
		  function_parameters(std::move(f_parameters)), function_block(block) {
	}
	void accept(Visitor& visitor) override;
	Value* type() {return out_type;}
	std::string& name() {return function_name;}
	FunctionParameters& parameters() {return function_parameters;}
	Value* block() {return function_block;}

  private:
	Value* out_type;
	std::string function_name;
	FunctionParameters function_parameters;
	Value* function_block;
};
class Expression : public Value {
  public:
	using Arguments = std::vector<ast::Value*>;
	Expression(std::string type, Arguments arguments)
		: expr_type(std::move(type)), args(std::move(arguments)) {}
	void accept(Visitor& visitor) override;
	std::string& type() {return expr_type;}
	Arguments& arguments() {return args;}
  protected:
	std::string expr_type;
	Arguments args;
};
class FunctionCall : public Value {
  public:
	using Arguments = std::vector<ast::Value*>;
	FunctionCall(std::string name, Arguments arguments)
		: function_name(std::move(name)), args(std::move(arguments)) {}
	void accept(Visitor& visitor) override;
	std::string& name() {return function_name;}
	Arguments& arguments() {return args;}
  private:
	std::string function_name;
	Arguments args;
};
class Variable : public Value {
  public:
	Variable(std::string name) : var_name(std::move(name)) {}
	void accept(Visitor& visitor) override;
	std::string& name() {return var_name;}
  private:
	std::string var_name;
};
class Literal : public Value {
  public:
	Literal(std::string value) : val(std::move(value)) {}
	void accept(Visitor& visitor) override;
	std::string& value() {return val;}
  private:
	std::string val;
};



} // namespace c::ast
