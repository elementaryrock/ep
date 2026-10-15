#include <stdio.h>
#include <string.h>
#include <ctype.h>
int isKeyword(const char *word)
{
    const char *Keyword[] = {"auto","int","char","float","void"};
    for (int i=0; i< sizeof(Keyword)/sizeof(Keyword[0]);i++)
    {
        if (strcmp(word,Keyword[i])==0)
        {
            return 1;
        }
    }
    return 0;
}

int PrintQuote(FILE *fp,int quote){
    int ch;
    putchar(quote);
    while((ch=fgetc(fp))!=EOF && ch !='\n' && ch !='\r'){
        putchar(ch);
        if(ch==quote){
            printf(" is %s\n",quote=='"'?" a string constant" :" a character constant");
            return 1;
        }
        if (ch == '\\') {
            ch = fgetc(fp);
            if (ch == EOF || ch == '\n' || ch == '\r')
                break;
            putchar(ch);
        }
    }
    printf("String incomplete");
    return 0;
}

int main(){
    FILE *fp = fopen("sum.txt","r");
    int ch;
    char word[50];
    int i;

    if (fp == NULL){
        printf("File not found");
        return 1;
    }

    while((ch=fgetc(fp))!= EOF){
      if(isspace(ch)){
        continue;
      }
        if (ch=='#'){
            do{
                putchar(ch);
                ch=fgetc(fp);
            }while(ch!=EOF && ch !='\n' && ch!='\r');
            puts("is a preprocessor directive");
        }

        else if (ch=='"'||ch=='\''){
            if(!PrintQuote(fp,ch)){
                fclose(fp);
                return 1;
            }
        }

        else if (isalpha(ch)||ch == '_'){
            i=0;
            do{
               word[i++]=(char)ch;
               ch=fgetc(fp);
            }while(isalnum(ch)||ch=='_');
            word[i]='\0';
            printf("%s is a %s\n",word,isKeyword(word)?"keyword":"identifier");
            if (ch!=EOF){
                ungetc(ch,fp);
            }
        }

        else if (isdigit(ch)){
            do{
                putchar(ch);
                ch=fgetc(fp);
            }while(isdigit(ch));
            puts("is a digit");
            if (ch!=EOF){
                ungetc(ch,fp);
            }
        }

        else if (ch!='\0' && strchr("*+-/=",ch)!=NULL){
            printf("%c is an operator\n",ch);
        }

        else if (ch!='\0' && strchr("[]{}?,.",ch)!=NULL){
            printf("%c is a special character\n",ch);
        }
        else{
          printf("%c is an unrecognized character\n", ch);
        }
    }
    fclose(fp);

}
