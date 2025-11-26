# Resources for Minishell
This file contains both useful resources for implementing your own shell interpreter.

Each section will have links in two categories: Necessary and Optional. It's recommended to check out the Necessary links, but you don't have to dive in the Optional resources. We put them in case you want more theoretical knowledge.

It's not recommended to read these resources in a single go; instead, read one by one and apply what you learned in C code (if applicable) before moving on to the next section.

## How Does BASH Work?
When first ran, BASH prints a prompt to the terminal screen and waits for the user to enter a command, separated by newline characters. Once the input is provided, it parses the command into necessary commands and arguments, expands the required variables, and then executes the command.

In short:

Prompt > Lexical Analysis (Tokenization) > Parsing > Execution > Repeat

So, Bash is essentially a REPL (Read-eval-print loop) program.

(todo: add an image here showcasing the steps)

## Learning Roadmap
(to be added)

## Links

### Shell Basics
Resources about shells and their history. Start here to gain knowledge about shells.

- REPL (Read-Eval-Print Loop): https://en.wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop
- Computer Terminal: https://en.wikipedia.org/wiki/Computer_terminal
- Shell: https://en.wikipedia.org/wiki/Shell_(computing)
- Shell Script: https://en.wikipedia.org/wiki/Shell_script
- Unix Shell: https://en.wikipedia.org/wiki/Unix_shell
- Command-line Interface: https://en.wikipedia.org/wiki/Command-line_interface
- Command-line Completion: https://en.wikipedia.org/wiki/Command-line_completion

### Bash (Bourne Again SHell)
You are tasked with implementing not any shell, but one specific one: Bash. Bash is the default shell program in many Linux distros and is the most widely used one across the ecosystem. You need to learn how Bash works and its specific behaviors before you start this project.

Among these links, GNU Bash Manual and OpenGroup Shell Specifications are the most important. They will guide you through building your shell.

- Bash: https://en.wikipedia.org/wiki/Bash_(Unix_shell)
- GNU Bash Manual: https://www.gnu.org/software/bash/manual/
- OpenGroup Shell Specifications: https://pubs.opengroup.org/onlinepubs/009696699/utilities/contents.html
- Shell Terminology: https://www.ibm.com/docs/en/aix/7.1.0?topic=concepts-shells-terminology
- Shell Expansion: https://www.spsanderson.com/steveondata/posts/2024-10-18/
- Special Variables: https://www.tutorialspoint.com/unix/unix-special-variables.htm

### Language Theory
#### Formal Language
- Formal Language: https://en.wikipedia.org/wiki/Formal_language
- Formal Grammar: https://en.wikipedia.org/wiki/Formal_grammar
- Context-Free Grammar: https://en.wikipedia.org/wiki/Context-free_grammar

#### Backus-Naur Form
Backus-Naur Form is a notation system for defining the syntax of programming languages and other formal languages. Shell language can be defined using this notation.

- Backus-Naur Form: https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form
- BNF Notation in Compiler Design: https://www.geeksforgeeks.org/compiler-design/bnf-notation-in-compiler-design/
- Bash V2 Grammar in BNF: https://cmdse.github.io/pages/appendix/bash-grammar.html

### Building
Now it's time to start building this thing! The development process will roughly go like this:

- Lexer
	- Analysis
	- Tokenization
- Parser
	- AST
	- Expansion
	- Quote removal
	- Wildcard Expansion (bonus)
	- Error handling for syntax errors
- Executor
	- Simple commands
	- Pipelines
	- Redirection
	- Subshells (bonus)
	- Command Lists, && and || (bonus)
	- Error handling
- Builtins
	- echo
	- cd
	- pwd
	- export
	- unset
	- env
	- exit
- History
	- Saving history with Readline
	- Browse through history by up and down
	- Extra: Ctrl-R to search in history. You are not required to implement this, but using Readline correctly will automatically add it to your project. So it's a good indicator whether Readline works as intended.
- Signals
	- Ctrl-C (SIGINT): Discards the current typed prompt, or kills a process if it's running
	- Ctrl-D (EOF): Sends an EOF signal. Exits heredoc if it's active. Exits the shell if there is no prompt.
	- Ctrl-\ (SIGQUIT): Needs to be ignored if no process is running. Otherwise, must be sent to children.

##### Lexer
Also known as tokenizing/tokenizer. This is where you take a meaningless string data and convert it into several pieces of meaningful tokens, to feed them to a parser.

- Lexical Analysis: https://en.wikipedia.org/wiki/Lexical_analysis
- Finite State Machine: https://en.wikipedia.org/wiki/Finite-state_machine
- Deterministic Finite Automaton: https://en.wikipedia.org/wiki/Deterministic_finite_automaton

#### Parser
The second step. Parser uses the tokens produced by lexer and parses them into meaningful data structures for the executor to use. Quote handling, expansion/substition and all that other stuff usually happen here.

##### Parsing
- Parsing: https://en.wikipedia.org/wiki/Parsing
- Top-down Parsing: https://en.wikipedia.org/wiki/Top-down_parsing

##### Abstract Syntax Tree
- Tree Data Structure: https://en.wikipedia.org/wiki/Tree_(abstract_data_type)
- Parse Tree: https://en.wikipedia.org/wiki/Parse_tree
- Abstract Syntax: https://en.wikipedia.org/wiki/Abstract_syntax
- Abstract Syntax Tree: https://en.wikipedia.org/wiki/Abstract_syntax_tree

#### Executor
The third step. Executing, file handling, redirecting and all that stuff happen here. If you've done the Pipex project, you're probably already familiar with most of these.

##### Operating System
Optional but recommended if you seek to gain deeper knowledge about processes.

- Computer Program: https://en.wikipedia.org/wiki/Computer_program
- (optional) System Software: https://en.wikipedia.org/wiki/System_software
- (optional) Operating System: https://en.wikipedia.org/wiki/Operating_system

##### Processes
You need to learn about them to implement your process-creation logic.

- Process: https://en.wikipedia.org/wiki/Process_(computing)
- Process Group: https://en.wikipedia.org/wiki/Process_group
- Process State: https://en.wikipedia.org/wiki/Process_state#Terminated
- Parent Process: https://en.wikipedia.org/wiki/Parent_process
- Child Process: https://en.wikipedia.org/wiki/Child_process
- (optional) Runtime System: https://en.wikipedia.org/wiki/Runtime_system

##### Process Exit
Understanding how processes exit, and exit status codes is a crucial part of shell design.

- Exit: https://en.wikipedia.org/wiki/Exit_(system_call)
- Exit Status: https://en.wikipedia.org/wiki/Exit_status
- Special Exit Codes: https://tldp.org/LDP/abs/html/exitcodes.html

##### Environment Variables
Parent processes pass on their environment variables to the child processes they create. Your login shell is responsible for passing on the environment variables to all other processes.

- Environment Variables: https://en.wikipedia.org/wiki/Environment_variable
- Unix Environment Variables: https://www.geeksforgeeks.org/linux-unix/environment-variables-in-linux-unix/
- Using env in C: https://www.geeksforgeeks.org/c/c-program-print-environment-variables/
- (optional) Login Shells: https://help.gnome.org/users/gnome-terminal/stable/pref-login-shell.html.en

##### Fork
You will create your child processes through the Fork system call.

- Fork: https://en.wikipedia.org/wiki/Fork_(system_call)
- Fork in C: https://www.geeksforgeeks.org/c/fork-system-call/

##### Exec & Execve
Exec system call is how you use other programs in your child processes.

- Exec: https://en.wikipedia.org/wiki/Exec_(system_call)
- Exec family in C: https://www.geeksforgeeks.org/c/exec-family-of-functions-in-c/
- Execve: https://man7.org/linux/man-pages/man2/execve.2.html

##### Files
Extra information about files. Recommended for a better understanding of pipes.

- Computer Directories: https://en.wikipedia.org/wiki/Directory_(computing)
- Computer File: https://en.wikipedia.org/wiki/Computer_file
- File Descriptors: https://en.wikipedia.org/wiki/File_descriptor
- File Permissions: https://en.wikipedia.org/wiki/File-system_permissions
- (optional) Setuid: https://en.wikipedia.org/wiki/Setuid

##### Redirections
- Redirection: https://en.wikipedia.org/wiki/Redirection_(computing)

##### Pipes
Pipes are another example of inter-process communication.

- Pipeline: https://en.wikipedia.org/wiki/Pipeline_(Unix)
- Unix Pipes: https://wiki.osdev.org/Unix_Pipes
- Pipes in C: https://www.geeksforgeeks.org/c/pipe-system-call/
- (optional) Message Passing: https://en.wikipedia.org/wiki/Message_passing

##### Signals
Signals are used for inter-process communication. You will mostly use it to handle CTRL-C, CTRL-D and CTRL-\\.

- Signal: https://en.wikipedia.org/wiki/Signal_(IPC)
- Unix Signals: https://www.math.stonybrook.edu/~ccc/dfc/dfc/signals.html
- Signals in C: https://www.geeksforgeeks.org/c/signals-c-language/
- Kill: https://en.wikipedia.org/wiki/Kill_(command)
- Signal Codes: https://dsa.cs.tsinghua.edu.cn/oj/static/unix_signal.html
- (optional) Inter-process Communication: https://en.wikipedia.org/wiki/Inter-process_communication

#### GNU Readline
GNU Readline library helps you prompt and read input from the user. It also provides various command-line editing tools and command history utilities.

- GNU Readline: https://en.wikipedia.org/wiki/GNU_Readline
- GNU Readline Documentation: https://tiswww.case.edu/php/chet/readline/readline.html
- Bash Command Line Editing: https://www.gnu.org/software/bash/manual/html_node/Command-Line-Editing.html
- Video: https://www.youtube.com/watch?v=Dxk5LUY37x4

### Team Work & Collaboration
This is the first collaborative project in the (old) 42 curriculum. It's essential to learn team work, pair programming and collaboration techniques.

- Software Development Life Cycle: https://www.geeksforgeeks.org/software-engineering/software-development-life-cycle-sdlc/
- Team Programming: https://en.wikipedia.org/wiki/Team_programming
- Pair Programming: https://en.wikipedia.org/wiki/Pair_programming

### Testing & Debugging
This is your opportunity to finally properly learn about software testing, since it has so many cases and is very error-prone.

#### Testing
- Software Testing: https://en.wikipedia.org/wiki/Software_testing
- Unit Testing: https://en.wikipedia.org/wiki/Unit_testing
- Integration Testing: https://en.wikipedia.org/wiki/Integration_testing
- (optional) Test-Driven Development: https://en.wikipedia.org/wiki/Test-driven_development

#### Tools
- GNU Debugger: https://en.wikipedia.org/wiki/GNU_Debugger
- Strace: https://en.wikipedia.org/wiki/Strace

## Miscellaneous

### Color Codes
Use of color codes is allowed. So why not use them to make your shell pretty?

- ANSI Color Codes: https://gist.github.com/JBlond/2fea43a3049b38287e5e9cefc87b2124

## Common Pitfalls
### GNU Readline Memory Leaks
Sometimes, using GNU Readline may cause leftover "still reachable memory" errors in Valgrind. Although we are not required to fix them, the subject document implies that it is possible to fix it. At the time of writing this, I don't know how to fix it. I will update this section with an explanation of a possible fix, as soon as I implement it myself.

(more coming soon)
