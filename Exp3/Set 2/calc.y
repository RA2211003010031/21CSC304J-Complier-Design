%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void yyerror(const char *s);
int yylex();
%}

%union {
    double num;
}

%token <num> NUMBER
%token POW EXP

%left '-' '/'
%right POW EXP /* Power and Exponential functions */

%type <num> expression

%%

equation:
    expression '\n' { printf("Result: %.2f\n", $1); return 0; } /* Two decimal places */
;

expression:
    expression '-' expression { $$ = $1 - $3; }
  | expression '/' expression { $$ = $1 / $3; }
  | POW '(' expression ',' expression ')' { $$ = pow($3, $5); }
  | EXP '(' expression ')' { $$ = exp($3); }
  | '(' expression ')' { $$ = $2; }
  | NUMBER { $$ = $1; }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter an arithmetic expression:\n");
    yyparse();
    return 0;
}