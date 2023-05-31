#include <libc/ast/XmlSerializer.hpp>

#include <string>
#include <unordered_map>

namespace ccompiler::ast {

pugi::xml_node XmlSerializer::append_child(const char* name) {
	return nodes.top().append_child(name);
}

void XmlSerializer::append_text(const char* text) {
	auto text_node = nodes.top().append_child(pugi::node_pcdata);
	text_node.set_value(text);
}

void XmlSerializer::exec(Document& document, std::ostream& out) {
	XmlSerializer xml_serializer;
	xml_serializer.nodes.push(xml_serializer.doc.append_child("program"));
	document.get_value()->accept(xml_serializer);
	xml_serializer.doc.save(out, "  ");
}

void XmlSerializer::visit(Elements& value) {
	for (const auto& element : value.elements()) {
		element->accept(*this);
	}
}

void XmlSerializer::visit(IncludeHeader& value) {
	auto elem_key = append_child("include");
	nodes.push(elem_key);
	append_text(value.name().c_str());
	nodes.pop();
}

void XmlSerializer::visit(Type& value) {
	static const std::unordered_map<Type::QualifierType, const char*>
		qualifier_map = { { Type::QualifierType::Const, "const" },
						  { Type::QualifierType::Volatile, "volatile" },
						  { Type::QualifierType::Restrict, "restrict" },
						  { Type::QualifierType::Pointer, "pointer" } };
	auto node = append_child("type");
	node.append_attribute("name") = value.name().c_str();
	for (const auto& qualifier : value.qualifiers()) {
		node.append_child(qualifier_map.at(qualifier));
	}
}

void XmlSerializer::visit(VarDeclarations& value) {
	for (auto& declaration : value.declarations()) {
		declaration.accept(*this);
	}
}
void XmlSerializer::visit(VarDeclarations::Declaration& value) {
	auto node = append_child("var");
	nodes.push(node);
	node.append_attribute("name") = value.name.c_str();
	value.type()->accept(*this);
	if (value.init_expression) {
		value.init_expression->accept(*this);
	}
	nodes.pop();
}

void XmlSerializer::visit(Statement& value) { value.state()->accept(*this); }

void XmlSerializer::visit(ReturnStatement& value) {
	auto node = append_child("return");
	nodes.push(node);
	value.expression()->accept(*this);
	nodes.pop();
}

void XmlSerializer::visit(Block& value) {
	auto node = append_child("block");
	nodes.push(node);
	for (const auto& statement : value.statements()) {
		statement->accept(*this);
	}
	nodes.pop();
}

void XmlSerializer::visit(FunctionDeclaration& value) {
	auto node = append_child("function");
	node.append_attribute("name") = value.name().c_str();
	auto out_type_node = node.append_child("out");
	nodes.push(out_type_node);
	value.type()->accept(*this);
	nodes.pop();
	auto in_types_node = node.append_child("in");
	nodes.push(in_types_node);
	for (const auto& var : value.parameters()) {
		auto arg_node = append_child("var");
		arg_node.append_attribute("name") = var.name.c_str();
		nodes.push(arg_node);
		var.type->accept(*this);
		nodes.pop();
	}
	nodes.pop();
	if (value.block() != nullptr) {
		value.block()->accept(*this);
	}
}

void XmlSerializer::visit(Expression& value) {
	auto node = append_child("expression");
	node.append_attribute("type") = value.type().c_str();
	for (const auto& arg : value.arguments()) {
		auto arg_node = node.append_child("arg");
		nodes.push(arg_node);
		arg->accept(*this);
		nodes.pop();
	}
}

void XmlSerializer::visit(FunctionCall& value) {
	auto node = append_child("call");
	node.append_attribute("name") = value.name().c_str();
	for (const auto& arg : value.arguments()) {
		auto arg_node = node.append_child("arg");
		nodes.push(arg_node);
		arg->accept(*this);
		nodes.pop();
	}
}

void XmlSerializer::visit(Variable& value) {
	append_text(value.name().c_str());
}
void XmlSerializer::visit(Literal& value) {
	append_text(value.value().c_str());
}

} // namespace ccompiler::ast
