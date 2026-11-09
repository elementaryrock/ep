#include <stdio.h>
#include <string.h>

char input[100], stack[100];
int ip = 0, len;

/* Print one row: stack, remaining input, action */
void printRow(const char *action)
{
    char rest[110];
    sprintf(rest, "%s$", input + ip);
    printf("$%-15s%-16s%s\n", stack, rest, action);
}

/* Try one reduction on the TOP of the stack. Returns 1 if it reduced. */
int reduce(void)
{
    int n = strlen(stack);
    char next = input[ip];          /* lookahead: next input symbol */

    /* E -> id */
    if (n >= 2 && stack[n-2] == 'i' && stack[n-1] == 'd')
    {
        stack[n-2] = 'E';
        stack[n-1] = '\0';
        printRow("REDUCE E->id");
        return 1;
    }
    /* E -> (E) */
    if (n >= 3 && stack[n-3] == '(' && stack[n-2] == 'E' && stack[n-1] == ')')
    {
        stack[n-3] = 'E';
        stack[n-2] = '\0';
        printRow("REDUCE E->(E)");
        return 1;
    }
    /* E -> E*E */
    if (n >= 3 && stack[n-3] == 'E' && stack[n-2] == '*' && stack[n-1] == 'E')
    {
        stack[n-2] = '\0';
        printRow("REDUCE E->E*E");
        return 1;
    }
    /* E -> E+E, but wait if a '*' comes next (* has higher precedence) */
    if (n >= 3 && stack[n-3] == 'E' && stack[n-2] == '+' && stack[n-1] == 'E' && next != '*')
    {
        stack[n-2] = '\0';
        printRow("REDUCE E->E+E");
        return 1;
    }
    return 0;
}

int main()
{
    char action[20];

    puts("GRAMMAR is E->E+E | E->E*E | E->(E) | E->id");
    printf("Enter input string: ");
    scanf("%99s", input);
    len = strlen(input);

    printf("\n%-16s%-16s%s\n", "STACK", "INPUT", "ACTION");

    while (ip < len)
    {
        /* SHIFT */
        if (input[ip] == 'i' && input[ip+1] == 'd')
        {
            strcat(stack, "id");
            ip += 2;
            printRow("SHIFT id");
        }
        else
        {
            int n = strlen(stack);
            stack[n] = input[ip];
            stack[n+1] = '\0';
            ip++;
            sprintf(action, "SHIFT %c", stack[n]);
            printRow(action);
        }

        /* REDUCE as many times as possible */
        while (reduce())
            ;
    }

    if (strcmp(stack, "E") == 0)
        printf("\nThe input string is ACCEPTED\n");
    else
        printf("\nThe input string is REJECTED\n");
    return 0;
}
