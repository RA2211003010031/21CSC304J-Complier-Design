%{
int line_number = 1;
%}

%%
\n          { printf("\n%d ", ++line_number); }
.           { printf("%s", yytext); }
%%

int yywrap() {}

int main() {
    printf("1 ");
    yylex();
    return 0;
}