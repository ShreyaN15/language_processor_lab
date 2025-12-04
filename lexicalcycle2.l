%{
#include <stdio.h>
%}

%%
"if"        { printf("KEYWORD_IF\n"); }
"else"      { printf("KEYWORD_ELSE\n"); }
"do"        { printf("KEYWORD_DO\n"); }
"int"       { printf("KEYWORD_INT\n"); }
"float"     { printf("KEYWORD_FLOAT\n"); }
"char"      { printf("KEYWORD_CHAR\n"); }
"while"     { printf("KEYWORD_WHILE\n"); }
"switch"    { printf("KEYWORD_SWITCH\n"); }
"double"    { printf("KEYWORD_DOUBLE\n"); }
"case"      { printf("KEYWORD_CASE\n"); }
"for"       { printf("KEYWORD_FOR\n"); }

[ \t\n]+     ;   /* ignore whitespace */
[0-9]+       { printf("TOKEN_INTEGER: %s\n", yytext); }
[a-zA-Z_][a-zA-Z0-9_]*  { printf("TOKEN_IDENTIFIER: %s\n", yytext); }

"=="         { printf("TOKEN_EQUAL\n"); }
"!="         { printf("TOKEN_NOTEQUAL\n"); }
"<="         { printf("TOKEN_LESSEQUAL\n"); }
">="         { printf("TOKEN_GREATEREQUAL\n"); }

"="          { printf("TOKEN_ASSIGNMENT\n"); }
"<"          { printf("TOKEN_LESSTHAN\n"); }
">"          { printf("TOKEN_GREATERTHAN\n"); }

"+"          { printf("TOKEN_PLUS\n"); }
"-"          { printf("TOKEN_MINUS\n"); }
"*"          { printf("TOKEN_MULTIPLY\n"); }
"/"          { printf("TOKEN_DIVIDE\n"); }

"{"          { printf("TOKEN_LBRACE\n"); }
"}"          { printf("TOKEN_RBRACE\n"); }
"("          { printf("TOKEN_LPAREN\n"); }
")"          { printf("TOKEN_RPAREN\n"); }
";"          { printf("TOKEN_SEMICOLON\n"); }

\"[^\"]*\"   { printf("TOKEN_STRING: %s\n", yytext); }

\/\/[^\n]*   ;   /* single-line comment */
\/\*([^*]|\*+[^*/])*\*+\/  ;   /* multi-line comment */

.            { printf("TOKEN_UNRECOGNIZED: %s\n", yytext); }
%%

int yywrap() {
    return 0;
}

int main() {
    yylex();
    return 0;
}
