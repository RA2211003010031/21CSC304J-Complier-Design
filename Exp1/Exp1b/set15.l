%%
[a-zA-Z_][a-zA-Z0-9_]*  { printf("Identifier: %s\n", yytext); }
.                        { printf("Not an identifier: %s\n", yytext); }
%%

int yywrap(){}

int main() {
    yylex();
    return 0;
}