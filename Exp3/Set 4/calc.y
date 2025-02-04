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
%token TAN TANH ATAN

%left '*' '/'
%right TAN TANH ATAN

%type <num> expression

%%

equation:
    expression '\n' { printf("Result: %.2f\n", $1); return 0; }
;

expression:
    expression '/' expression { $$ = $1 / $3; }
  | expression '*' expression { $$ = $1 * $3; }
  | TAN '(' expression ')' { $$ = tan($3); }
  | TANH '(' expression ')' { $$ = tanh($3); }
  | ATAN '(' expression ')' { $$ = atan($3); }
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