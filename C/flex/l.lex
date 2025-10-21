%{
#include <stdio.h>
%}

IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*

%%
{IDENTIFIER}   { printf("valid: %s\n", yytext); }
.|\n           { }
%%

int main()
{
    return yylex(), 0;
}
// for running -> flex l.lex ; gcc lex.yy.c -lfl -o lexer ; ./lexer