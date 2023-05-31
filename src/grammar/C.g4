grammar C;

document: elements;
elements: element*;

element: includeHeader | functionDefine | globalInitializing;

includeHeader: GRID INCLUDE HEADERNAME;

HEADERNAME: LESST ~ ["\n\r]* MORET;

functionDefine:
	anyType functionName LPAR functionParameters RPAR (
		SEMICOLON
		| block
	);
anyType:
	qualifiers=typeQualifier? typeName qualifiers=typeQualifier*;
typeName: INT
		| DOUBLE
		| LONG
		| SHORT
		| CHAR
		| VOID
		| FLOAT
		| varId;
functionName: varId;
functionParameters:
	functionParameter?
	| (functionParameter (COMMA functionParameter)*);
functionParameter: anyType varId;

typeQualifier: CONST | VOLATILE | RESTRICT | MULT;

globalInitializing: varDeclaration SEMICOLON;

statement:
	block SEMICOLON? #BlockState
	| varDeclaration SEMICOLON #DeclState
	| expression SEMICOLON #ExprState
	| SEMICOLON #EmptyState
	| RETURN expression? SEMICOLON #ReturnState
	;
block: LBRACE statement* RBRACE;

varDeclaration:
	anyType varAssignment (COMMA varAssignment)*;
varAssignment:
	varId (EQUAL expression)?;

expression:
	expression postfixOperation #PostfixExpr
	| prefixOperation expression #PrefixExpr
	| varId LPAR (args+=expression (COMMA args+=expression)*)? RPAR #FunctionExpr
	| expression LBRACKET expression RBRACKET #SubscriptExpr
	| LPAR expression RPAR #ParensExpr
	| unaryOperation expression #UnaryExpr
    | expression binaryOperation expression #BinaryExpr
	| expression QUEST expression COLON expression #TernaryExpr
	| expression assignmentOperation expression #AssignmentExpr
    | varId #NameExpr
	| constValue #ConstExpr
    ; 

constValue: CHAR_CONST | FLOAT_CONST | INTEGER_CONST | STRING_CONST;



varId: IDENTIFICATOR;

assignmentOperation: binaryOperation? EQUAL ;

binaryOperation:
	binaryAriphmeticOperation
	| bitwiseShiftOperation
	| binaryCompareOperation
	| binaryBitwiseOperation
	| binaryLogicOperation
	;

bitwiseShiftOperation: LESST LESST | MORET MORET;

unaryOperation:
	unaryAriphmeticOperation
	| unaryLogicOperation
	| unaryBitwiseOperation
	// | castOperation
	// | dereferenceOperation
	// | addressOperation
	;
binaryAriphmeticOperation:
	multOp
	| divOp
	| remainderOp
	| plusOp
	| minusOp;
binaryCompareOperation:
	lessThenOp
	| moreThenOp
	| lessEqualOp
	| moreEqualOp
	| equalOp
	| notEqualOp;
binaryLogicOperation: logicOrOp | logicAndOp;
binaryBitwiseOperation: bitOrOp | bitAndOp | bitXorOp;
unaryAriphmeticOperation: plusOp | minusOp;
unaryLogicOperation: logicNotOp;
unaryBitwiseOperation: bitNotOp;

prefixOperation: incrementOp | decrementOp;
postfixOperation: incrementOp | decrementOp;
incrementOp: PLUS PLUS;
decrementOp: MINUS MINUS;

multOp: MULT;
divOp: DIV;
plusOp: PLUS;
minusOp: MINUS;
remainderOp: PERC;

lessThenOp: LESST;
moreThenOp: MORET;
lessEqualOp: LESST EQUAL;
moreEqualOp: MORET EQUAL;
equalOp: EQUAL EQUAL;
notEqualOp: EXCLAM EQUAL;

logicOrOp: VERTLINE VERTLINE;
logicAndOp: AMPER AMPER;
logicNotOp: EXCLAM;

bitOrOp: VERTLINE;
bitAndOp: AMPER;
bitXorOp: CIRC;
bitNotOp: TILDA;



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
INCLUDE: 'include';



COMMENTDEF: DIV DIV (~'\n')* -> skip;
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