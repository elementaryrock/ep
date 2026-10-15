#include<stdio.h>
#include<string.h>
#include <stdlib.h>

int stacktop=0,stackindex=0,pos=0,inlen=0;
char stack[20],input[20],shift[20],reduce[20];
void rcheck();
int main()
{
	puts("Grammar\n");
	puts("Enter the string\n");
	scanf("%s",input);
	puts("stack\tinput\t operation\t");
	inlen=strlen(input);
	strcpy(shift,"shift -> ");
	for(stacktop=0,pos=0;pos<inlen;stacktop++,pos++)
	{
		if(input[pos]=='i' && input[pos+1]=='d')
		{
			stack[stacktop]=input[pos];
			stack[stacktop+1]=input[pos+1];
			stack[stacktop+2]='\0';
			input[pos]=' ';
			input[pos+1]=' ';
			printf("%s\t%s\t%s id\t",stack,input,shift);
			rcheck();
		}
		else{
			stack[stacktop]=input[pos];
			stack[stacktop+1]='\0';
			input[pos]=' ';
			printf("%s\t%s\t%s %c\t",stack,input,shift,stack[stacktop]);
			rcheck();
		}
	}
}
void rcheck()
{
	strcpy(reduce,"Reduce to E");
	for(stackindex=0;stackindex<strlen(stack);stackindex++)
	{
		if(stack[stackindex]=='i'&& stack[stackindex+1]=='d')
		{
			stack[stackindex]='E';
			stack[stackindex+1]='\0';
			printf("%s\t%s\t%s %c\t",stack,input,reduce);
			pos++;
		}
	}
	for(stackindex=0;stackindex<strlen(stack);stackindex++)
	{
		if(stack[stackindex]=='E'&& stack[stackindex+1]=='+' && stack[stackindex+2]=='E')
		{
			stack[stackindex]='E';
			stack[stackindex+1]='\0';
			stack[stackindex+2]='\0';
			printf("%s\t%s\t%s\t",stack,input,reduce);
			stacktop=stacktop-2;
		}
	}
	for(stackindex=0;stackindex<strlen(stack);stackindex++)
	{
		if(stack[stackindex]=='E'&& stack[stackindex+1]=='+' && stack[stackindex+2]=='E')
		{
			stack[stackindex]='E';
			stack[stackindex+1]='\0';
			stack[stackindex+2]='\0';
			printf("%s\t%s\t%s\t",stack,input,reduce);
			stacktop=stacktop-2;

		}
	}
	for(stackindex=0;stackindex<strlen(stack);stackindex++)
	{
		if(stack[stackindex]=='E'&& stack[stackindex+1]=='*' && stack[stackindex+2]=='E')
		{
			stack[stackindex]='E';
			stack[stackindex+1]='\0';
			stack[stackindex+2]='\0';
			printf("%s\t%s\t%s\t",stack,input,reduce);
			stacktop=stacktop-2;

		}
	}
	for(stackindex=0;stackindex<strlen(stack);stackindex++)
	{
		if(stack[stackindex]=='('&& stack[stackindex+1]=='E' && stack[stackindex+2]==')')
		{
			stack[stackindex]='E';
			stack[stackindex+1]='\0';
			stack[stackindex+2]='\0';
			printf("%s\t%s\t%s\t",stack,input,reduce);
			stacktop = stacktop-2;

		}
	}
}
