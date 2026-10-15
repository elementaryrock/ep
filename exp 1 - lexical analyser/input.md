# Input guide for 1.lex.c

## 1. Compile and run

Run these commands from inside `exp 1 - lexical analyser`.

The program always reads **sum.txt in the current working folder**. Edit that file
to try different inputs. It does not read input from the keyboard or accept a
different filename as an argument.

With Linux GCC:

```sh
gcc -std=c11 -Wall -Wextra -pedantic 1.lex.c -o lex
./lex
```

In the current WSL environment, Windows GCC is available instead:

```sh
gcc.exe -std=c11 -Wall -Wextra -pedantic 1.lex.c -o lex.exe
./lex.exe
```

The included `sum.txt` is a practice input. Its contents are analyzed, not executed.

## 2. What inputs are supported?

| Input type | Examples | Output category |
|---|---|---|
| Keyword | `int`, `char`, `return`, `while` | `keyword` |
| Identifier | `a`, `sum`, `total2`, `_count`, `student_name` | `identifier` |
| Integer digits | `0`, `5`, `123` | `constant` |
| String | `"hello"`, `"sum = %d\n"`, `""` | `string constant` |
| Character literal | `'a'`, `'\n'`, `'\''` | `a character constant` |
| Single-character operator | `+`, `-`, `*`, `/`, `%`, `=` | `operator` |
| Special character | `,`, `;`, `[`, `]`, `{`, `}`, `(`, `)` | `special character` |
| Directive line | `#include <stdio.h>`, `#define MAX 10` | `a preprocessor directive` |
| Whitespace | Spaces, tabs, newlines | Skipped |

### Keywords

These are the 32 keywords from your supplied code:

```text
auto      break     case      char      const     continue  default  do
double    else      enum      extern    float     for       goto     if
int       long      register  return    short     signed    sizeof   static
struct    switch    typedef   union     unsigned  void      volatile while
```

Names are case-sensitive: `int` is a keyword; `Int` is an identifier.
`main`, `printf`, and `scanf` are identifiers, not keywords.

### Identifiers

The first character must be a letter or underscore. The remaining characters may
be letters, digits, or underscores. Keep each identifier to **49 characters or
fewer**, because `word[50]` also needs a final `\0`. The code does not check this
length; longer names write beyond the buffer.

- `total2` and `_value` each produce one identifier.
- `2total` produces constant `2` followed by identifier `total`.
- `first name` produces two identifiers.
- `total-value` produces identifier `total`, operator `-`, and identifier `value`.

### Numbers

Only a sequence of digits is recognized as one numeric constant.

- `123` produces one constant.
- `-25` produces operator `-` and constant `25`.
- `075` is printed as a constant, without checking its base or calculating a value.

Numbers are printed directly, so they have no fixed buffer-length limit.

### Strings and characters

Text between matching quotes is one token. Symbols inside it are not classified
separately: `"a+b;"` is one string.

A backslash makes the next character part of the quoted text, even if it is a
quote. Examples include `"say \"hello\""`, `"C:\\temp"`, `'\n'`, and `'\''`.
The program prints escapes as written; it does not convert `\n` into a newline.

Quoted text has no fixed buffer-length limit. A real newline inside a quoted token
is unsupported; write `\n` inside a string to represent a newline instead.

### Preprocessor directives

Outside quotes, `#` starts a directive token that extends to the end of the physical
line or file. Everything on that line is printed together, including `<stdio.h>`.
The program does not open included headers or expand macros.

## 3. Practice input and exact output

Put this in `sum.txt`:

```c
int total2 = 10;
char letter = 'a';
printf("hello");
```

Output:

```text
____________OUTPUT____________
int is keyword
total2 is identifier
= is operator
10 is constant
; is special character
char is keyword
letter is identifier
= is operator
'a' is a character constant
; is special character
printf is identifier
( is special character
"hello" is string constant
) is special character
; is special character
```

## 4. What cannot be done?

This is a basic token classifier, not a complete C lexer or compiler.

| Unsupported feature | Actual behavior |
|---|---|
| Decimal numbers | `3.14` prints `3` as a constant, then stops with an error at `.`. |
| Exponent notation | `1e3` becomes constant `1` and identifier `e3`. |
| Hexadecimal notation and suffixes | `0xFF` becomes `0` and `xFF`; `10UL` becomes `10` and `UL`. These are not recognized as single numbers. |
| Multi-character operators | `==` is two `=` operators; `++` is two `+` operators; `+=` is `+` followed by `=`. |
| Comparison/logical/bitwise operators | `<`, `>`, `!`, `&`, `|`, `^`, `~`, `?`, and `:` cause an error outside quotes/directives. |
| Comments | Comments are **not skipped**. `// note` becomes `/`, `/`, and `note`; `/* note */` becomes `/`, `*`, `note`, `*`, and `/`. Unsupported symbols in comment text can cause an error. |
| Other punctuation | `.`, `@`, `$`, backticks, and other unlisted bytes cause an error outside quotes/directives. |
| Newer C keywords | Words such as `inline`, `restrict`, and `_Bool` are identifiers. |
| Full literal validation | Empty character literals, multiple characters such as `'ab'`, or unknown escapes such as `"\q"` are classified by their quote boundaries, not validated as C. |
| Advanced string forms | `L"hello"` produces identifier `L` and a string; adjacent strings remain separate tokens. |
| Multiline strings/directives | Backslash-newline continuation is unsupported. A directive is only one physical line. |
| Correct directive placement | Any `#` outside quotes starts a directive, even in the middle of a line. Placement is not validated. |
| Unicode names and embedded NUL tokens | Unicode identifier rules are unsupported; NUL outside quotes/directives is rejected. |
| Syntax checking or execution | Does not check semicolons, balanced brackets, variable declarations, types, or expression validity. It does not calculate or run anything. |

## 5. Errors and end of file

The program returns **0** after successful scanning and **1** after an error.
Errors are printed to the error stream.

Errors include a missing/unreadable `sum.txt`, a file-read failure,
an unsupported byte, and a quoted token without a closing
quote before a newline or end of file.

Tokens printed before an error remain visible. An unfinished quoted token can leave
partial text in the output; the program does not invent a closing quote.

An empty input file is allowed. The final identifier, number, or directive does not
need a final newline.
