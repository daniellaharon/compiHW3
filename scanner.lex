%{
    #include "node_type.hpp"
    #include "hw3_output.hpp"
    using namespace output;
    #include "parser.tab.hpp"
%}

%option yylineno
%option noyywrap

digit           ([0-9])
letter          ([a-zA-Z])
whitespace      ([\t\n\r ]|\/\/[^\r\n]*[\r|\n|\r\n]?)


%%

void                        return VOID;
int                         return INT;
byte                        return BYTE;
b                           return B;
bool                        return BOOL;
auto                        return AUTO;
and                         return AND;
or                          return OR;
not                         return NOT;
true                        return TRUE;
false                       return FALSE;
return                      return RETURN;
if                          return IF;
else                        return ELSE;
while                       return WHILE;
break                       return BREAK;
continue                    return CONTINUE;
;                           return SC;
,                           return COMMA;
\(                          return LPAREN;
\)                          return RPAREN; 
\{                          return LBRACE;
\}                          return RBRACE;
=                           return ASSIGN;
==|!=                       return EQUALOP;
\<=|\>=|\<|\>               return RELOP;
\+|\-                       return ADDOP;
\*|\/                       return MULTOP;
{letter}+({letter}|{digit})*  {yylval = std::make_shared<Var>(); yylval->id = yytext; return ID;}
[1-9]{digit}*|0             {yylval = std::make_shared<Var>(); yylval->value = stoi(yytext); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\" return STRING;
{whitespace}                ;
.                           {errorLex(yylineno); exit(1);}
%%