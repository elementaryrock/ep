#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isKeyword(const char *word)
{
    const char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue",
        "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long",
        "register", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };

    for (int i = 0; i < sizeof keywords / sizeof keywords[0]; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

/* The same function handles both kinds of quoted constants. */
int printQuoted(FILE *fp, int quote)
{
    int ch;

    putchar(quote);
    while ((ch = fgetc(fp)) != EOF && ch != '\n' && ch != '\r') {
        putchar(ch);
        if (ch == quote) {
            printf(" is %s\n", quote == '"' ? "string constant" : "a character constant");
            return 1;
        }
        if (ch == '\\') {
            ch = fgetc(fp);
            if (ch == EOF || ch == '\n' || ch == '\r')
                break;
            putchar(ch);
        }
    }

    putchar('\n');
    fprintf(stderr, "Error: unfinished quoted constant.\n");
    return 0;
}

int main(void)
{
    FILE *fp = fopen("sum.txt", "r");
    int ch;
    char word[50];

    if (fp == NULL) {
        perror("sum.txt");
        return 1;
    }

    puts("____________OUTPUT____________");
    while ((ch = fgetc(fp)) != EOF) {
        if (isspace(ch))
            continue;

        /* Preprocessor directive: print the whole line. */
        if (ch == '#') {
            do {
                putchar(ch);
                ch = fgetc(fp);
            } while (ch != EOF && ch != '\n' && ch != '\r');
            puts(" is a preprocessor directive");
        }

        /* String or character constant. */
        else if (ch == '"' || ch == '\'') {
            if (!printQuoted(fp, ch)) {
                fclose(fp);
                return 1;
            }
        }

        /* Read a name, then decide whether it is a keyword. */
        else if (isalpha(ch) || ch == '_') {
            int i = 0;
            do {
                word[i++] = (char)ch;
                ch = fgetc(fp);
            } while (isalnum(ch) || ch == '_');

            word[i] = '\0';
            printf("%s is %s\n", word, isKeyword(word) ? "keyword" : "identifier");
            if (ch != EOF)
                ungetc(ch, fp);
        }

        /* Digits can be printed directly: no number buffer is needed. */
        else if (isdigit(ch)) {
            do {
                putchar(ch);
                ch = fgetc(fp);
            } while (isdigit(ch));

            puts(" is constant");
            if (ch != EOF)
                ungetc(ch, fp);
        }

        else if (ch != '\0' && strchr("+-*/%=", ch) != NULL) {
            printf("%c is operator\n", ch);
        }

        else if (ch != '\0' && strchr(",;[]{}()", ch) != NULL) {
            printf("%c is special character\n", ch);
        }

        else {
            fprintf(stderr, "Error: unsupported input byte 0x%02X.\n", ch);
            fclose(fp);
            return 1;
        }
    }

    if (ferror(fp)) {
        fprintf(stderr, "Error reading sum.txt.\n");
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}
