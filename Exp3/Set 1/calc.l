%{
#include "calc.tab.h"
#include <stdio.h>
%}

%%

[0-9]+(\.[0-9]+)?  { yylval.num = atof(yytext); return NUMBER; }
"sin"              { return SIN; }
"cos"              { return COS; }
"asin"             { return ASIN; }
"acos"             { return ACOS; }
"+"                { return '+'; }
"*"                { return '*'; }
"\n"               { return '\n'; }
[ \t]              { /* Ignore whitespace */ }
"("                { return '('; }
")"                { return ')'; }
.                  { printf("Invalid character: %s\n", yytext); }

%%

int yywrap() { return 1; }