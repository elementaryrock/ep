//AISWARYA PB

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int stackIndex = 0, stackTop = 0, inputPos = 0, inputLength = 0;
char input[16], reduceAction[20], stack[15], shiftAction[10];

void checkReductions();

int main()
{
    puts(" GRAMMAR is E->E+E \n E->E*E \n E->(E) \n E->id");
    puts(" enter input string ");
    scanf("%s", input);
    inputLength = strlen(input);
    strcpy(shiftAction, "SHIFT - >");
    puts("stack \t input \t action ");

    for (stackTop = 0; inputPos < inputLength; stackTop++, inputPos++)
    {
        if (input[inputPos] == 'i' && input[inputPos + 1] == 'd')
        {
            stack[stackTop] = input[inputPos];
            stack[stackTop + 1] = input[inputPos + 1];
            stack[stackTop + 2] = '\0';
            input[inputPos] = ' ';
            input[inputPos + 1] = ' ';
            printf("\n$%s\t%s$\t%s id", stack, input, shiftAction);
            checkReductions();
        }
        else
        {
            stack[stackTop] = input[inputPos];
            stack[stackTop + 1] = '\0';

            input[inputPos] = ' ';
            printf("\n$%s\t%s$\t%s %c symbol ", stack, input, shiftAction, stack[stackTop]);
            checkReductions();
        }
    }
}

void checkReductions()
{
    strcpy(reduceAction, " REDUCE TO E");

    /* Rule: E -> id */
    for (stackIndex = 0; stackIndex < strlen(stack); stackIndex++)
        if (stack[stackIndex] == 'i' && stack[stackIndex + 1] == 'd')
        {
            stack[stackIndex] = 'E';
            stack[stackIndex + 1] = '\0';
            printf("\n$%s\t%s$\t%s", stack, input, reduceAction);
            inputPos++;
        }

    /* Rule: E -> E+E */
    for (stackIndex = 0; stackIndex < strlen(stack); stackIndex++)
        if (stack[stackIndex] == 'E' && stack[stackIndex + 1] == '+' && stack[stackIndex + 2] == 'E')
        {
            stack[stackIndex] = 'E';
            stack[stackIndex + 1] = '\0';
            stack[stackIndex + 2] = '\0';
            printf("\n$%s\t%s$\t%s", stack, input, reduceAction);
            stackTop = stackTop - 2;
        }

    /* Rule: E -> E*E */
    for (stackIndex = 0; stackIndex < strlen(stack); stackIndex++)
        if (stack[stackIndex] == 'E' && stack[stackIndex + 1] == '*' && stack[stackIndex + 2] == 'E')
        {
            stack[stackIndex] = 'E';
            stack[stackIndex + 1] = '\0';
            stack[stackIndex + 1] = '\0';
            printf("\n$%s\t%s$\t%s", stack, input, reduceAction);
            stackTop = stackTop - 2;
        }

    /* Rule: E -> (E) */
    for (stackIndex = 0; stackIndex < strlen(stack); stackIndex++)
        if (stack[stackIndex] == '(' && stack[stackIndex + 1] == 'E' && stack[stackIndex + 2] == ')')
        {
            stack[stackIndex] = 'E';
            stack[stackIndex + 1] = '\0';
            stack[stackIndex + 1] = '\0';
            printf("\n$%s\t%s$\t%s", stack, input, reduceAction);
            stackTop = stackTop - 2;
        }
}

/* OUTPUT

GRAMMAR is E->E+E
 E->E*E
 E->(E)
 E->id
 enter input string
(id+id)*id
stack    input   action

$(       id+id)*id$     SHIFT - > ( symbol
$(id       +id)*id$     SHIFT - > id
$(E        +id)*id$      REDUCE TO E
$(E+        id)*id$     SHIFT - > + symbol
$(E+id        )*id$     SHIFT - > id
$(E+E         )*id$      REDUCE TO E
$(E           )*id$      REDUCE TO E
$(E)           *id$     SHIFT - > ) symbol
$E             *id$      REDUCE TO E
$E*             id$     SHIFT - > * symbol
$E*id             $     SHIFT - > id
$E*E              $      REDUCE TO E
$E                $      REDUCE TO E
*/
