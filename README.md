# 42-minishell
A small-scale Linux shell program, intended to mimic the original BASH. Given as a mandatory common core project by 42 Schools.

# Objective
The task is to create a small but functioninng shell that mimics the behavior of the original Bourne Again SHell, created in 1986. This project is the first team project of the 42 cursus, and must be done by 2 students. The students should collaborate, communicate and build efficiently, and manage their time well.

# How Does BASH Work?
When first ran, BASH prints a prompt to the terminal screen and waits for the user to enter a command, seperated by newline characters. Once the input is filled, it parses the command into tokens, expands the required variables, and then executes the command.

In short:

Prompt > Parse Input > Tokenization > Expansion > Execution > Repeat

(todo: add an image here showcasing the steps)

# Resources
This section covers an extensive amount of resources for building your own Bash.

## Shell Basics
- REPL (Read-Eval-Print Loop): https://en.wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop
- Computer Terminal: https://en.wikipedia.org/wiki/Computer_terminal
- Shell: https://en.wikipedia.org/wiki/Shell_(computing)
- Shell Script: https://en.wikipedia.org/wiki/Shell_script
- Unix Shell: https://en.wikipedia.org/wiki/Unix_shell
- Command-line Interface: https://en.wikipedia.org/wiki/Command-line_interface

## Bash (Bourne Again SHell)
- Bash: https://en.wikipedia.org/wiki/Bash_(Unix_shell)
- GNU Bash Manual: https://www.gnu.org/software/bash/manual/
- Shell Terminology: https://www.ibm.com/docs/en/aix/7.1.0?topic=concepts-shells-terminology
- Shell Expansion: https://www.spsanderson.com/steveondata/posts/2024-10-18/
- Special Variables: https://www.tutorialspoint.com/unix/unix-special-variables.htm

## Lexer

### Lexical Analysis
- Automata Theory: https://en.wikipedia.org/wiki/Automata_theory
- Finite State Machine: https://en.wikipedia.org/wiki/Finite-state_machine
- Deterministic Finite Automaton: https://en.wikipedia.org/wiki/Deterministic_finite_automaton
- Non-deterministic Finite Automaton: https://en.wikipedia.org/wiki/Nondeterministic_finite_automaton
- Lexical Analysis: https://en.wikipedia.org/wiki/Lexical_analysis
- Lexer Hack: https://en.wikipedia.org/wiki/Lexer_hack

### Backus-Naur Form
- Backus-Naur Form: https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form
- BNF Notation in Compiler Design: https://www.geeksforgeeks.org/compiler-design/bnf-notation-in-compiler-design/

## Parser

### Parsing
- Parsing: https://en.wikipedia.org/wiki/Parsing
- Parse Tree: https://en.wikipedia.org/wiki/Parse_tree
- Scanerless (Lexerless) Parsing: https://en.wikipedia.org/wiki/Scannerless_parsing

### Abstract Syntax Tree
- Abstract Syntax: https://en.wikipedia.org/wiki/Abstract_syntax
- Abstract Syntax Tree: https://en.wikipedia.org/wiki/Abstract_syntax_tree

## Executor

### Operating System
Recommended if you seek to gain deeper knowledge about processes.

- System Software: https://en.wikipedia.org/wiki/System_software
- Operating System: https://en.wikipedia.org/wiki/Operating_system
- Computer Program: https://en.wikipedia.org/wiki/Computer_program

### Processes
Needed to gain a deeper understanding of process to implement your process-creation logic.

- Process: https://en.wikipedia.org/wiki/Process_(computing)
- Process State: https://en.wikipedia.org/wiki/Process_state#Terminated
- Parent Process: https://en.wikipedia.org/wiki/Parent_process
- Child Process: https://en.wikipedia.org/wiki/Child_process
- Runtime System: https://en.wikipedia.org/wiki/Runtime_system

### Process Exit
Understanding exit codes is a crucial part of shell design.

- Exit: https://en.wikipedia.org/wiki/Exit_(system_call)
- Exit Status: https://en.wikipedia.org/wiki/Exit_status

### Environment Variables
- Environment Variable: ttps://en.wikipedia.org/wiki/Environment_variable
- Unix Environment Variables: https://www.geeksforgeeks.org/linux-unix/environment-variables-in-linux-unix/

### Fork
You will create your child processes through the Fork system call.

- Fork: https://en.wikipedia.org/wiki/Fork_(system_call)
- Fork in C: https://www.geeksforgeeks.org/c/fork-system-call/

### Exec & Execve
Exec system call is how you use other programs in your child processes.

- Exec: https://en.wikipedia.org/wiki/Exec_(system_call)
- Exec family in C: https://www.geeksforgeeks.org/c/exec-family-of-functions-in-c/
- Execve: https://man7.org/linux/man-pages/man2/execve.2.html

### Unix Signals
Signals are used for inter-process communication. You will mostly use it to handle CTRL-C, CTRL-D and CTRL-\\.

- Inter-process Communication: https://en.wikipedia.org/wiki/Inter-process_communication
- Signal: https://en.wikipedia.org/wiki/Signal_(IPC)
- Kill: https://en.wikipedia.org/wiki/Kill_(command)
- Unix Signals: https://www.math.stonybrook.edu/~ccc/dfc/dfc/signals.html
- Signal Codes: https://dsa.cs.tsinghua.edu.cn/oj/static/unix_signal.html
- Signals in C: https://www.geeksforgeeks.org/c/signals-c-language/

## GNU Readline
GNU Readline library helps you prompt and read input from the user. It also provides command line editing and command history.

- GNU Readline: https://en.wikipedia.org/wiki/GNU_Readline
- GNU Readline Documentation: https://tiswww.case.edu/php/chet/readline/readline.html
- Bash Commnad Line Editing: https://www.gnu.org/software/bash/manual/html_node/Command-Line-Editing.html
- Video: https://www.youtube.com/watch?v=Dxk5LUY37x4

## Miscellaneous

### Color Codes
Use of color codes is allowed. So why not use them to make your shell pretty?

- ANSI Color Codes: https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124

# Common Pitfalls

## GNU Readline Memory Leaks
Sometimes, using GNU Readline may cause leftover "still reachable memory" errors in Valgrind. Although we are not required to fix them, the subject document implies that it is possible to fix it. At the time of writing this, I don't know how to fix it. I will update this section with an explanation of a possible fix, as soon as I implement it myself.

(more coming soon)
