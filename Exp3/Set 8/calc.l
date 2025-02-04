%{
#include "calc.tab.h"
#include <stdio.h>
%}

%%

[0-9]+(\.[0-9]+)?  { yylval.num = atof(yytext); return NUMBER; }
"ceil"             { return CEIL; }
"floor"            { return FLOOR; }
"-"                { return '-'; }
"/"                { return '/'; }
"("                { return '('; }
")"                { return ')'; }
"\n"               { return '\n'; }
[ \t]              { /* Ignore whitespace */ }
.                  { printf("Invalid character: %s\n", yytext); }

%%

int yywrap() { return 1; }