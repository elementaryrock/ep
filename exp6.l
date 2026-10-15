%{
#include <stdio.h>
%}

%%
"int"|"float"|"char"|"if"|"else"|"while"|"for"|"return"|"void" {
    printf("Keyword: %s\n", yytext);
}
[a-zA-A_][a-zA-Z0-9_]* {
    printf("Identifier: %s\n", yytext);
}
[0-9]+(\.[0-9]+)? {
    printf("Constant/Number: %s\n", yytext);
}
"+"|"-"|"*"|"/"|"=="|"<"|">"|"<="|">="|"=" {
    printf("Operator: %s\n", yytext);
}
"{"|"}"|"("|")"|";"|"," {
    printf("Special Symbol: %s\n", yytext);
}
[ \t\n]+ ;
. {
    printf("Unrecognized character: %s\n", yytext);
}
%%

int main()
{
    FILE *fp;
    fp = fopen("newfile.txt","r");
    if (fp==NULL)
    {
      printf("Cannot open file\n");
      return 1;
    }
    yyin = fp;
    yylex();
}
int yywrap(void)
{
    return 1;
}
