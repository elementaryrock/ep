#include <stdio.h>
#include <string.h>
#include <ctype.h>

int n, m = 0;                 // n = no. of productions, m = size of result list
char a[10][10], f[100];       // a = productions, f = result list
int seen[128];                // stops infinite loops in follow()

void first(char c);
void follow(char c);
void show(int showEps);

int main()
{
    int i, z;
    char c;

    printf("Enter no of productions: ");
    scanf("%d", &n);

    printf("Enter productions (e.g. S=ABC, use @ for epsilon):\n");
    for (i = 0; i < n; i++)
        scanf("%s", a[i]);

    do
    {
        printf("\nEnter non-terminal: ");
        scanf(" %c", &c);

        m = 0;
        first(c);
        printf("First(%c)  = { ", c);
        show(1);                       // epsilon allowed in FIRST

        m = 0;
        memset(seen, 0, sizeof(seen));
        follow(c);
        printf("Follow(%c) = { ", c);
        show(0);                       // epsilon never in FOLLOW

        printf("Continue (1/0)? ");
        scanf("%d", &z);
    } while (z == 1);

    return 0;
}

/* Print f[] without duplicates */
void show(int showEps)
{
    int i, j, dup;
    for (i = 0; i < m; i++)
    {
        dup = 0;
        for (j = 0; j < i; j++)
            if (f[i] == f[j]) dup = 1;

        if (!dup && (f[i] != '@' || showEps))
            printf("%c ", f[i]);
    }
    printf("}\n");
}

/* ---------- FIRST ---------- */
void first(char c)
{
    int k, j, x, old, w, eps;

    if (!isupper(c))               // terminal (or @): FIRST = itself
    {
        f[m++] = c;
        return;
    }

    for (k = 0; k < n; k++)
    {
        if (a[k][0] != c) continue;           // only rules of c

        eps = 1;                              // can we look at next symbol?
        for (j = 2; a[k][j] != '\0' && eps; j++)
        {
            old = m;                          // where new symbols start
            first(a[k][j]);                   // add FIRST of this symbol

            eps = 0;
            w = old;
            for (x = old; x < m; x++)         // look only at NEW symbols
            {
                if (f[x] == '@') eps = 1;     // this symbol can vanish
                else f[w++] = f[x];           // keep everything except @
            }
            m = w;
        }

        if (eps) f[m++] = '@';                // ALL symbols vanished
    }
}

/* ---------- FOLLOW ---------- */
void follow(char c)
{
    int i, j, x, k, old, eps;

    if (seen[(int)c]) return;                 // already done, avoid looping
    seen[(int)c] = 1;

    if (a[0][0] == c)                         // Rule 1: start symbol gets $
        f[m++] = '$';

    for (i = 0; i < n; i++)
        for (j = 2; a[i][j] != '\0'; j++)
            if (a[i][j] == c)                 // found c on a right side
            {
                eps = 1;                      // 1 = "everything after c vanishes"
                for (x = j + 1; a[i][x] != '\0' && eps; x++)
                {
                    old = m;
                    first(a[i][x]);           // Rule 2: add FIRST of next symbol
                    eps = 0;
                    for (k = old; k < m; k++)
                        if (f[k] == '@') eps = 1;
                }

                if (eps && a[i][0] != c)      // Rule 3: c at end / rest vanishes
                    follow(a[i][0]);          // add FOLLOW of left side
            }
}
