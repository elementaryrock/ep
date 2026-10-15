%{
#include <stdio.h>
int consonants=0,vowels=0;
%}

%%
[aeiouAEIOU] {vowels++;}
[a-zA-Z] {consonants++;}
. ;
\n {printf("Vowels: %d \nConsonants: %d \n",vowels,consonants);}
%%

int main(){
  printf("Enter word: ");
  yylex();
  return 0;
}

int yywrap(){
  return 1;
}
