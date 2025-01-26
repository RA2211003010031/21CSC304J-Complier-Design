%{
int positive_count = 0, negative_count = 0;
%}

%%
[0-9]+      { positive_count++; }
-[0-9]+     { negative_count++; }
%%

int yywrap(){}

int main() {
    yylex();
    printf("Positive Numbers: %d, Negative Numbers: %d\n", positive_count, negative_count);
    return 0;
}
