lexer grammar CLexer;

// document: elements;
// elements: element*;

// element: includeHeader | functionDefine | globalInitializing;

// includeHeader: GRID 'include' HEADERNAME;

// HEADERNAME: LESST ~ ["\n\r]* MORET;

// functionDefine:
// 	anyType functionName LPAR functionParameters RPAR (
// 		SEMICOLON
// 		| (LBRACE statement RBRACE)
// 	);
// anyType:
// 	typeQualifier? (
// 		INT
// 		| DOUBLE
// 		| LONG
// 		| SHORT'
// 		| CHAR
// 		| VOID
// 		| FLOAT
// 		| IDENTIFICATOR
// 	) (MULT typeQualifier)*;
// functionName: IDENTIFICATOR;
// functionParameters:
// 	functionParameter?
// 	| (functionParameter (COMMA functionParameter)*);
// functionParameter: anyType IDENTIFICATOR;

// typeQualifier: 'const' | 'volatile' | 'restrict';

// globalInitializing: varDeclaration SEMICOLON;

// statement:
// 	block SEMICOLON?
// 	| varDeclaration SEMICOLON
// 	| expression SEMICOLON
// 	| SEMICOLON;
// block: LBRACE statement* RBRACE;
// functionCall: IDENTIFICATOR LPAR functionArguments RPAR;
// functionArguments:
// 	functionArgument?
// 	| (functionArgument (COMMA functionArgument)*);
// functionArgument: expression;
// expression:
// 	IDENTIFICATOR #NameExpr
// 	| functionCall #FunctionExpr
//     | expression binaryOperation expression #BinaryExpr
// 	| expression LBRACKET expression RBRACKET #SubscriptExpr
// 	| unaryOperation expression #UnaryExpr
// 	| expression QUEST expression COLON expression #TernaryExpr
//     | prefixOperation expression #PrefixExpr
//     | expression postfixOperation #PostfixExpr
//     ; 

// binaryOperation:
// 	binaryAriphmeticOperation
// 	| binaryLogicOperation
// 	| binaryBitwiseOperation
// 	| binaryCompareOperation;
// unaryOperation:
// 	unaryAriphmeticOperation
// 	| unaryLogicOperation
// 	| unaryBitwiseOperation;
// binaryAriphmeticOperation:
// 	multOp
// 	| divOp
// 	| plusOp
// 	| minusOp
// 	| remainderOp;
// binaryCompareOperation:
// 	lessThenOp
// 	| moreThenOp
// 	| lessEqualOp
// 	| moreEqualOp
// 	| equalOp
// 	| notEqualOp;
// binaryLogicOperation: logicOrOp | logicAndOp;
// binaryBitwiseOperation: bitOrOp | bitAndOp | bitXorOp;
// unaryAriphmeticOperation: plusOp | minusOp;
// unaryLogicOperation: logicNotOp;
// unaryBitwiseOperation: bitNotOp;

// prefixOperation: incrementOp | decrementOp;
// postfixOperation: incrementOp | decrementOp;
// incrementOp: PLUS PLUS;
// decrementOp: MINUS MINUS;

// multOp: MULT;
// divOp: DIV;
// plusOp: PLUS;
// minusOp: MINUS;
// remainderOp: PERC;

// lessThenOp: LESST;
// moreThenOp: MORET;
// lessEqualOp: LESST EQUAL;
// moreEqualOp: MORET EQUAL;
// equalOp: EQUAL EQUAL;
// notEqualOp: EXCLAM EQUAL;

// logicOrOp: VERTLINE VERTLINE;
// logicAndOp: AMPER AMPER;
// logicNotOp: EXCLAM;

// bitOrOp: VERTLINE;
// bitAndOp: AMPER;
// bitXorOp: CIRC;
// bitNotOp: TILDA;

// varDeclaration:
// 	anyType (
// 		varAssignment?
// 		| (varAssignment (COMMA varAssignment)*)
// 	);
// varAssignment:
// 	expression EQUAL expression; // потому что у нас есть адресная арифметика!

WHILE: 'while';
DO: 'do';
BREAK: 'break';
IF: 'if';
ELSE: 'else';
CASE: 'case';
SWITCH: 'switch';
STRUCT: 'struct';
UNION: 'union';
ENUM: 'enum';
TYPEDEF: 'typedef';
RETURN: 'return';
CONST: 'const';
RESTRICT: 'restrict';
VOLATILE: 'volatile';
SIZEOF: 'sizeof';
AUTO: 'auto';
DOUBLE: 'double';
INT: 'int';
LONG: 'long';
CHAR: 'char';
FLOAT: 'float';
REG: 'register';
EXTERN: 'extern';
SHORT: 'short';
UNSIG: 'unsigned';
DEFAULT: 'default';
FOR: 'for';
SIG: 'signed';
CONTINUE: 'continue';
VOID: 'void';



COMMENTDEF: DIV DIV ~ '\n'*;
COMMENTNEST: DIV MULT INVALID* MULT DIV;


FLOAT_CONST: DIGIT+ DOT DIGIT* EXPONENT? | DOT DIGIT+ EXPONENT? | DIGIT+ EXPONENT;
EXPONENT: ('e' | 'E') ('+'? | '-') DIGIT+;
INTEGER_CONST: '0' | [1-9] [0-9]*;
CHAR_CONST: '\'' . '\'';
STRING_CONST: '"' ~ ["\n\r]* '"';



IDENTIFICATOR: (LETTER | '_') (LETTER | DIGIT | '_')*;

GRID: '#';
LBRACE: '{';
RBRACE: '}';
LBRACKET: '[';
RBRACKET: ']';
LPAR: '(';
RPAR: ')';
COMMA: ',';
COLON: ':';
SEMICOLON: ';';

LESST: '<';
MORET: '>';
PERC: '%';
AMPER: '&';
CIRC: '^';
EQUAL: '=';
EXCLAM: '!';
QUEST: '?';
DOT: '.';
VERTLINE: '|';
DIV: '/';
MULT: '*';
MINUS: '-';
PLUS: '+';
TILDA: '~';

DIGIT: [0-9];
LETTER: [a-zA-Z];

WS: [ \n\t\r]+ -> skip;

INVALID: .;