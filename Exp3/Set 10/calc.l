%{
#include "calc.tab.h"
#include <math.h>
%}

%%

[ \t]+                ; /* Ignore spaces and tabs */
[0-9]+(\.[0-9]+)?     { yylval.num = atof(yytext); return NUMBER; }
"log5"                { return LOG5; }
"log3"                { return LOG3; }
"sin"                 { return SIN; }
"acos"                { return ACOS; }
"("                   { return '('; }
")"                   { return ')'; }
\n                    { return '\n'; }
.                     { printf("Invalid character: %c\n", yytext[0]); }

%%

int yywrap() {
    return 1;
}