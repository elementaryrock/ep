# How the simplified lex.c works

## 1. Purpose

A lexical analyzer splits source text into **tokens**, the small units of a program.

For example:

```c
int sum = a + 5;
```

produces:

```text
int is keyword
sum is identifier
= is operator
a is identifier
+ is operator
5 is constant
; is special character
```

The lexer classifies these tokens. It does not calculate the sum or check whether
the whole program is valid.

## 2. Headers and important variables

| Item | Purpose |
|---|---|
| `stdio.h` | File reading and printing |
| `string.h` | `strcmp` for word comparison; `strchr` for symbol lookup |
| `ctype.h` | `isspace`, `isalpha`, `isalnum`, and `isdigit` |
| `FILE *fp` | The input file being read |
| `int ch` | The latest input byte, or `EOF` |
| `char word[50]` | Stores one identifier/keyword |
| `size_t i` | The next buffer position while reading a name |

**Why is `ch` an `int` rather than a `char`?** `fgetc` returns an integer so it can
represent every input byte and the separate end-of-file value `EOF`.

## 3. isKeyword

This function stores the same 32 keywords as your supplied code and compares the
input word with each of them.

```c
if (strcmp(word, keywords[i]) == 0)
    return 1;
```

`strcmp` returns zero when two strings are equal. Therefore `isKeyword` returns
`1` if it finds a match and `0` if it reaches the end without finding one.

The number of keywords is calculated using:

```c
sizeof keywords / sizeof keywords[0]
```

This means **the whole array's size divided by one element's size**. It avoids
typing a separate keyword count. The calculation works because `keywords` is an
actual array here. `const` means the function does not change the keyword text.

## 4. printQuoted

One function handles both double-quoted strings and single-quoted characters.
Its `quote` argument records which closing character to look for.

1. Print the opening quote.
2. Read and print the next character.
3. If it matches the opening quote, print the category and return `1`.
4. If it is a backslash, read and print the following character without checking
   that character as a closing quote.
5. Repeat until the token finishes.
6. If a newline or EOF arrives before the closing quote, report an error and return `0`.

For `"say \"hello\""`, the escaped quotes stay inside the string.
Only the final unescaped `"` ends it. Characters are printed directly, so no quote
buffer or quote-state flags are needed.

## 5. main: open the file

```c
FILE *fp = fopen("sum.txt", "r");
```

Open `sum.txt` for reading. If `fopen` returns `NULL`, `perror("sum.txt")` prints a
system error and `return 1` ends the program.

The filename is fixed, just as in your supplied code. To change the input, edit
`sum.txt` in the folder from which you run the executable.

## 6. main: classify each token

```c
while ((ch = fgetc(fp)) != EOF)
```

Read one character and keep scanning until the file ends. The branches are:

### A. Whitespace

`isspace(ch)` identifies spaces, tabs, and line endings. `continue` skips them and
starts the next loop iteration.

### B. Preprocessor directive

When the character is `#`, print it and the rest of the physical line, then print
`is a preprocessor directive`. The contents of the line are not classified separately.

### C. String or character constant

For either `"` or `'`, call `printQuoted`. On failure, close the input file and
return `1`.

### D. Identifier or keyword

A name starts with a letter or underscore. Keep reading while the characters are
letters, digits, or underscores.

- Start `i` at zero for each name.
- Store each character in `word[i++]`.
- Finish with `word[i] = '\0'`.
- Call `isKeyword` and print the appropriate category.
- Put the lookahead character back if it is not EOF.

`\0` terminates a C string; without it, `strcmp` and `%s` would not know where the
stored word ends. The 50-byte array holds up to 49 name characters plus this terminator.
There is no length check, so practice inputs must stay within that size.

### E. Integer constant

When the first character is a digit, print consecutive digits directly with
`putchar`. When a non-digit or EOF appears, print `is constant`.

Unlike a name, a number does not need to be stored for comparison, so it does not
need a buffer. The program prints the digit sequence without converting it to an integer.

### F. Operator

```c
strchr("+-*/%=", ch) != NULL
```

`strchr` looks for the character in the listed string. A non-`NULL` result means
it was found. Print that one character as an operator.

This version has no operator lookahead: `==` is printed as two `=` tokens.
Likewise, `/` is always an operator; it does not start comment-skipping logic.

### G. Special character

Use the same lookup method for `,;[]{}()` and print the matching character.

Both symbol branches reject `\0` explicitly: `strchr` can also find its own string's
terminating null byte, which should not count as a supported input symbol.

### H. Unsupported input

If no branch matches, print an error containing the byte's hexadecimal value,
close the file, and return `1`. Unsupported characters are not silently discarded.

## 7. Why ungetc is needed

Reading a name or number requires reading one character beyond it to find its end.
That extra character can belong to the next token.

Example: `sum=5`

1. Read `s`, `u`, and `m` into `word`.
2. Read `=` and discover that the name is finished.
3. Print `sum is identifier`.
4. `ungetc('=', fp)` puts `=` back.
5. The next outer-loop iteration reads and prints `=` as an operator.
6. The number branch then reads and prints `5`.

`ungetc` is used only when the lookahead is not EOF. A final name or number is still
printed even when the file has no final newline.

## 8. Finish the program

After the scanning loop, `ferror(fp)` checks whether reading failed. An error returns
`1`. Otherwise, `fclose(fp)` closes the file and `return 0` signals successful scanning.

## 9. Lab-exam revision

Remember:

> Open file → read character → select branch → finish token → print category → repeat → close file.

- `fgetc`: read the next character, or EOF.
- `ungetc`: put lookahead back for the next token.
- `strcmp`: compare a name with each keyword.
- `strchr`: check whether a symbol is in a list.
- `isalpha`: check for a letter.
- `isalnum`: check for a letter or digit.
- `isdigit`: check for a digit.
- `isspace`: check for whitespace.
- `putchar`: print one character.
- `\0`: end a stored C string.

Read `input.md` for supported forms and limits. Read `correction.md` for changes
relative to the code you pasted.
