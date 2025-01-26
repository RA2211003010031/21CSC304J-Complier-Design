%%
[A-Z]       { printf("Uppercase: %s\n", yytext); }
[a-z]       { printf("Lowercase: %s\n", yytext); }
.           { /* Ignore other characters */ }
%%

int yywrap() {}

int main() {
    yylex();
    return 0;
}
