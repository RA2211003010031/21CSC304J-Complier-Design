%%
[+\-*/]     { printf("Operator: %s\n", yytext); }
.           { printf("Not an operator: %s\n", yytext); }
%%

int yywrap(){}

int main() {
    yylex();
    return 0;
}