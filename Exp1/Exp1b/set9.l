%%
[0-9]+      { printf("Digit: %s\n", yytext); }
[a-zA-Z]+   { printf("Word: %s\n", yytext); }
.           { printf("Other: %s\n", yytext); }
%%

int yywrap() {}

int main() {
    yylex();
    return 0;
}