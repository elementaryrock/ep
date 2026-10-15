#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int stackIndex=0,stackTop=0,inputLength=0,inputPos=0;
char stack[10],reduce[15],shift[15],input[15];

void PerformRed();

int main(){
  puts("Grammar is of the form \n E->E+E \n E->E*E \n E-> (E) \n E -> id");
  puts("Enter string:");
  scanf("%s",input);
  inputLength=strlen(input);
  strcpy(shift,"Shift ->");
  puts("stack\tInput\tOperation");
  for(inputPos=0;inputPos<inputLength;stackTop++,inputPos++){
    if(input[inputPos]=='i' && input[inputPos+1]=='d'){
      stack[stackTop]=input[inputPos];
      stack[stackTop+1]=input[inputPos+1];
      stack[stackTop+2]='\0';
      input[inputPos]=' ';
      input[inputPos+1]=' ';
      printf("$%s\t$%s\t%s id\n",stack,input,shift);
      PerformRed();
    }
    else{
      stack[stackTop]=input[inputPos];
      stack[stackTop+1]='\0';
      input[inputPos]=' ';
      printf("$%s\t$%s\t%s id\n",stack,input,shift);
      PerformRed();
    }
  }

}

void PerformRed(){
  for(stackIndex=0;stackIndex<strlen(stack);stackIndex++){
    if(stack[stackIndex]=='i' && stack[stackIndex+1]=='d'){
      stack[stackIndex]='E';
      stack[stackIndex+1]='\0';
      printf("$%s\t$%s Reduce to E\n",stack,input);
      inputPos++;
    }
  }
  for(stackIndex=0;stackIndex<strlen(stack);stackIndex++){
    if(stack[stackIndex]=='E' && stack[stackIndex+1]=='+' && stack[stackIndex+2]=='E'){
      stack[stackIndex]='E';
      stack[stackIndex+1]='\0';
      stack[stackIndex+2]='\0';
      printf("$%s\t$%sReduce to E\n",stack,input);
      stackTop=stackTop-2;
    }
  }
  for(stackIndex=0;stackIndex<strlen(stack);stackIndex++){
    if(stack[stackIndex]=='E' && stack[stackIndex+1]=='*' && stack[stackIndex+2]=='E'){
      stack[stackIndex]='E';
      stack[stackIndex+1]='\0';
      stack[stackIndex+2]='\0';
      printf("$%s\t$%sReduce to E\n",stack,input);
      stackTop=stackTop-2;
    }
  }
  for(stackIndex=0;stackIndex<strlen(stack);stackIndex++){
    if(stack[stackIndex]=='(' && stack[stackIndex+1]=='E' && stack[stackIndex+2]==')'){
      stack[stackIndex]='E';
      stack[stackIndex]='\0';
      stack[stackIndex]='\0';
      printf("$%s\t$%sReduce to E\n",stack,input);
      stackTop=stackTop-2;
    }
  }
}
