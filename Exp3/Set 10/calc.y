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
%token LOG5 LOG3 SIN ACOS

%type <num> expression

%%

equation:
    expression '\n' { printf("Result: %.2f\n", $1); return 0; }
;

expression:
    LOG5 '(' expression ')' { $$ = log($3) / log(5); }
  | LOG3 '(' expression ')' { $$ = log($3) / log(3); }
  | SIN '(' expression ')' { $$ = sin($3); }
  | ACOS '(' expression ')' { $$ = acos($3); }
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