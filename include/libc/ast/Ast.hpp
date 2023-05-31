#pragma once

#include <memory>
#include <vector>

namespace ccompiler::ast {
class Base;
class Visitor;

class Document final {
  public:
	template <class T, class... Args> T* create_node(Args&&... args) {
		static_assert(std::is_base_of_v<Base, T>);
		values_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		return dynamic_cast<T*>(values_.back().get());
	}

	void set_value(Base* value) { value_ = value; }
	ast::Base* get_value() const { return value_; }

  private:
	std::vector<std::unique_ptr<Base>> values_;
	ast::Base* value_ = nullptr;
};

class Tokenizable {
  public:
	std::size_t line;
	std::size_t column;
};

class Base : public Tokenizable {
  public:
	virtual ~Base() = default;
	virtual void accept(Visitor& visitor) = 0;
};

class Element : public Base {};

class Elements : public Base {
  public:
	Elements(std::vector<Base*> elements) : elems(std::move(elements)) {}
	void accept(Visitor& visitor) override;
	const std::vector<Base*> elements() const { return elems; }

  private:
	std::vector<Base*> elems;
};

class IncludeHeader : public Element {
  public:
	IncludeHeader(std::string str) : lib_name(std::move(str)) {}
	std::string& name() { return lib_name; }
	void accept(Visitor& visitor) override;

  private:
	std::string lib_name;
};

class Type : public Base {
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

class VarDeclarations : public Base {
  public:
	struct Declaration : public Base {
		std::string name;
		Base* init_expression;
		Base* var_type;
		Base* type() { return var_type; }
		Declaration(std::string var_name, Base* init, Base* type)
			: name(std::move(var_name)), init_expression(init),
			  var_type(type) {}
		void accept(Visitor& visitor) override;
	};
	using Declarations = std::vector<Declaration>;
	VarDeclarations(Base* type, Declarations declarations)
		: var_type(type), var_declarations(std::move(declarations)) {}
	Base* type() { return var_type; }
	Declarations& declarations() { return var_declarations; }
	void accept(Visitor& visitor) override;

  private:
	Base* var_type;
	Declarations var_declarations;
};

class Statement : public Base {
  public:
	Statement(Base* value) : some_state(value) {}
	void accept(Visitor& visitor) override;
	Base* state() { return some_state; }

  private:
	Base* some_state;
};

class ReturnStatement : public Base {
  public:
	ReturnStatement(Base* value) : return_expression(value) {}
	Base* expression() { return return_expression; }
	void accept(Visitor& visitor) override;

  private:
	Base* return_expression;
};
class WhileStatement : public Base {};
class ForStatement : public Base {};
class SwitchStatement : public Base {};

class Block : public Base {
  public:
	using Statements = std::vector<Base*>;
	Block(Statements states) : block_statements(std::move(states)) {}
	Statements& statements() { return block_statements; }
	void accept(Visitor& visitor) override;

  private:
	Statements block_statements;
};

class FunctionDeclaration : public Element {
  public:
	struct FunctionParameter : public Tokenizable {
		Base* type;
		std::string name;
		FunctionParameter(Base* parameter_type, std::string parameter_name)
			: type(parameter_type), name(std::move(parameter_name)) {}
	};
	using FunctionParameters = std::vector<FunctionParameter>;
	FunctionDeclaration(Base* f_type, std::string f_name,
						FunctionParameters f_parameters, Base* block)
		: out_type(f_type), function_name(std::move(f_name)),
		  function_parameters(std::move(f_parameters)), function_block(block) {
	}
	void accept(Visitor& visitor) override;
	Base* type() { return out_type; }
	std::string& name() { return function_name; }
	FunctionParameters& parameters() { return function_parameters; }
	Base* block() { return function_block; }

  private:
	Base* out_type;
	std::string function_name;
	FunctionParameters function_parameters;
	Base* function_block;
};
class Expression : public Base {
  public:
	using Arguments = std::vector<ast::Base*>;
	Expression(std::string type, Arguments arguments)
		: expr_type(std::move(type)), args(std::move(arguments)) {}
	void accept(Visitor& visitor) override;
	std::string& type() { return expr_type; }
	Arguments& arguments() { return args; }

  protected:
	std::string expr_type;
	Arguments args;
};
class FunctionCall : public Base {
  public:
	using Arguments = std::vector<ast::Base*>;
	FunctionCall(std::string name, Arguments arguments)
		: function_name(std::move(name)), args(std::move(arguments)) {}
	void accept(Visitor& visitor) override;
	std::string& name() { return function_name; }
	Arguments& arguments() { return args; }

  private:
	std::string function_name;
	Arguments args;
};
class Variable : public Base {
  public:
	Variable(std::string name) : var_name(std::move(name)) {}
	void accept(Visitor& visitor) override;
	std::string& name() { return var_name; }

  private:
	std::string var_name;
};
class Literal : public Base {
  public:
	Literal(std::string value) : val(std::move(value)) {}
	void accept(Visitor& visitor) override;
	std::string& value() { return val; }

  private:
	std::string val;
};

} // namespace c::ast
