%{
int word_count = 0;
%}

%%
[^\n\t ]+   { word_count++; }
%%

int yywrap() {}

int main(int argc, char **argv) {
    yylex();
    printf("Total Words: %d\n", word_count);
    return 0;
}