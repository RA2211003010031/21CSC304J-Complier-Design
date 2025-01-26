%{
int char_count = 0, line_count = 0;
%}

%%

\n          {line_count++;}
.           {char_count++;}

%%

int yywrap() {}

int main(int argc, char **argv) {
    yylex();
    printf("\nLines: %d, Characters: %d\n", line_count, char_count);
    return 0;
}