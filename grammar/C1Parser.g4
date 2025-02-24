parser grammar C1Parser;
options { tokenVocab = C1Lexer; }

compilationUnit: ;
decl: ;
constdecl: ;
constdef: ;
vardecl: ;
vardef: ;
funcdef: ;
block: ;
stmt: ;
lval: ;
cond: ;
exp:
    (Plus | Minus) exp
    | exp (Plus | Minus) exp
    | exp (Multiply | Divide | Modulo) exp
    | LeftParen exp RightParen
    | number
;
number: IntConst | FloatConst;
