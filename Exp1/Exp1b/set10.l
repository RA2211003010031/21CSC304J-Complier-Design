%{
int balance = 0;
%}

%%
\(          { balance++; }
\)          { balance--; }
%%

int yywrap(){}

int main() {
    yylex();
    if (balance == 0)
        printf("Well-formed parentheses.\n");
    else
        printf("Not well-formed.\n");
    return 0;
}