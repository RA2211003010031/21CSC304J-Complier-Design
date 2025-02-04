%{
#include "calc.tab.h"
#include <math.h>
%}

%%

[ \t]+                ; /* Ignore spaces and tabs */
-?[0-9]+(\.[0-9]+)?    { yylval.num = atof(yytext); return NUMBER; }
"*"                   { return '*'; }
"abs"                 { return ABS; }
"fabs"                { return FABS; }
"cot"                 { return COT; }
"("                   { return '('; }
")"                   { return ')'; }
\n                    { return '\n'; }
.                     { printf("Invalid character: %c\n", yytext[0]); }

%%

int yywrap() {
    return 1;
}