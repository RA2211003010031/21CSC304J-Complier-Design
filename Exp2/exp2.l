%{
int token_no = 1;
int line_no = 1;
%}

DIGIT        [0-9]
LETTER       [a-zA-Z]
ID           {LETTER}({LETTER}|{DIGIT})*
FLOAT        {DIGIT}+"."{DIGIT}*
NEWLINE      \n
PREPROCESSOR \#[a-zA-Z]+
HEADER_FILE  <{ID}(.h)>|\"{ID}(.h)\"

%%

{NEWLINE} { line_no++; }

{PREPROCESSOR}[ \t]*{HEADER_FILE} {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Header File", yytext);
}

{PREPROCESSOR} {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Preprocessor", yytext);
}

"unsigned long long" { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }
"unsigned long"      { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }
"unsigned int"       { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }
"long long"          { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }
"long"               { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }
"int"                { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }
"char"               { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }
"float"              { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }
"double"             { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }
"void"               { printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Data Type", yytext); }

"return"|"if"|"else"|"while"|"for"|"switch"|"case"|"break"|"continue"|"default"|"do"|"printf"|"scanf"|"int main()                                                                                                                                                                                                                           " {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Keyword", yytext);
}

{ID} {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Identifier", yytext);
}

{FLOAT} {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Number (float)", yytext);
}

{DIGIT}+ {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Number (int)", yytext);
}

"+"|"-"|"*"|"/" {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Operator", yytext);
}

"=" {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Assignment Operator", yytext);
}

"=="|"<="|">="|"!="|"<"|">" {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Relational Operator", yytext);
}

"{"|"}"|"("|")"|"["|"]" {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Unrecognized Character", yytext);
}

","|";" {
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Special Symbol", yytext);
}

[ \t]+   /* Ignore whitespace */

. { 
    printf("%-10d %-10d %-20s %-20s\n", token_no++, line_no, "Unknown", yytext);
}

%%

int yywrap() { return 1; }

int main(int argc, char **argv) {
    FILE *fp;
    char filename[50];

    printf("Enter the filename: \n");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file!\n");
        return 1;
    }

    yyin = fp;

    printf("\n%-10s %-10s %-20s %-20s\n", "TOKEN NO", "LINE NO", "TOKEN TYPE", "LEXEME");

    yylex();
    fclose(fp);
    return 0;
}