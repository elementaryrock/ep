# Lexical Analysis Experiments

The files are grouped by experiment so the project can be uploaded to GitHub
without mixing source files, inputs, documentation, and generated outputs.

## Experiments

| Folder | Main work | Related files |
|---|---|---|
| `exp 1 - lexical analyser` | C lexical analyzer | Practice input, explanations, corrections, alternate source, and compiled outputs |
| `exp 2 - recursive descent parser` | Recursive-descent expression parser | C source and compiled output |
| `exp 3 - vowel consonant counter` | Flex scanner for counting vowels and consonants | Flex source, generated scanner, and compiled output |
| `exp 4 - line counter` | Line-counting Flex experiment | `4.line.l` source file |

The executable files are preserved in their experiment folders for local
reference, but compiled binaries are ignored by Git using `.gitignore`. The
source files and documentation remain available to commit to GitHub.

## Running the experiments

Open a terminal inside the experiment's folder before compiling or running it.
Experiment 1 reads `sum.txt` from the current working folder; its compile/run
instructions are in `exp 1 - lexical analyser/input.md`.

`exp 4 - line counter/4.line.l` is currently empty and has been preserved unchanged.
The folder name is based on its existing filename; no line-counting code was added.

The shared `.vscode/tasks.json` remains at the project root. It builds the active
C file in that file's folder, so it still works with the new layout.
