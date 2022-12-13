%{
    #include "TypeVar.h"
    #include "hw3_output.h"
    using namespace output;
    #include "parser.tab.hpp"
%}

%option noyywrap
%option yylineno

%%
void                                return VOID;
int                                 return INT;
byte                                return BYTE;
b                                   return B;
bool                                return BOOL;
and                                 return AND;
or                                  return OR;
not                                 return NOT;
true                                return TRUE;
false                               return FALSE;
return                              return RETURN;
if                                  return IF;
else                                return ELSE;
while                               return WHILE;
break                               return BREAK;
continue                            return CONTINUE;
;                                   return SC;
,                                   return COMMA;
\(                                  return LPAREN;
\)                                  return RPAREN;
\{                                  return LBRACE;
\}                                  return RBRACE;
=                                   return ASSIGN;
\<|\>|\<=|\>=                       return RELOP;
==|!=                               return EQOP;
\+|\-                               return BINOP;
\*|\/                               return MULTDIVTOP;
[a-zA-Z][a-zA-Z0-9]*                {yylval = std::make_shared<TypeVar>(); yylval->id = yytext; return ID;}
0|[1-9][0-9]*                       {yylval = std::make_shared<TypeVar>(); yylval->value = stoi(yytext); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\"       return STRING;
\/\/[^\r\n]*[\r|\n|\r\n]?|[\t\n\r ] ;
.                                  {errorLex(yylineno); exit(1);}
%%