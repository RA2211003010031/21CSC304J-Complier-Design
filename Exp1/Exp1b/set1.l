%%
[aeiouAEIOU] { printf("Vowel: %s\n", yytext); }
.            { printf("Not a vowel: %s\n", yytext); }
%%
int yywrap() {
    return 1;
}
int main() {
    yylex();
    return 0;
}