%{
#include <string.h>
int word_count = 0;
char target[100];
%}

%%
{target}    { word_count++; }
%%

int yywrap() {}

int main() {
    printf("Enter the word to count: ");
    scanf("%s", target);
    yylex();
    printf("Frequency of '%s': %d\n", target, word_count);
    return 0;
}