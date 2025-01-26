%{
int line_count = 0, space_count = 0, tab_count = 0;
%}

%%
\n          { line_count++; }
[ ]         { space_count++; }
\t          { tab_count++; }
%%

int yywrap() {}

int main(int argc, char **argv) {
    yylex();
    printf("Lines: %d, Spaces: %d, Tabs: %d\n", line_count, space_count, tab_count);
    return 0;
}