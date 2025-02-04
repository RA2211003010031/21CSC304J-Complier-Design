%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calc.tab.h"

void yyerror(const char *s);
%}

%%

[0-9]+(\.[0-9]*)?  { yylval.num = atof(yytext); return NUMBER; }
"pow"              { return POW; }
"exp"              { return EXP; }
"("                { return '('; }
")"                { return ')'; }
","                { return ','; }
"-"                { return '-'; }
"/"                { return '/'; }
"\n"               { return '\n'; }
[ \t]+             { /* Ignore whitespace */ }
.                  { printf("Invalid character: %s\n", yytext); }

%%

int yywrap() { return 1; }