
%{
#include <stdio.h>
int line = 0, chars = 0, words = 0;
%}

%%
[^ \t\n]+  { words++; chars += yyleng; }
[ \t]     { chars++; }
\n        { line++; chars++; }
.         ;
%%

int main() {
    yyin = fopen("input.txt", "r");

    if (yyin == NULL) {
        printf("File not found\n");
        return 1;
    }

    yylex();

    printf("Lines: %d\nWords: %d\nChar: %d\n",
           line, words, chars);

    fclose(yyin);
    return 0;
}

int yywrap() {
    return 1;
}
