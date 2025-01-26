%%
[a-z]+      { printf("Match: %s\n", yytext); }
%%

int yywrap() {}

int main() {
    yylex();
    return 0;
}