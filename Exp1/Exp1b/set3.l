%{
int char_count = 0;
%}

%%
.           { char_count++; }
%%

int yywrap() {}

int main(int argc, char **argv) {
    yylex();
    printf("Total Characters: %d\n", char_count);
    return 0;
}