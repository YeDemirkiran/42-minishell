*This project has been created as part of the 42 curriculum by yademirk, muayna.*

# Description
A small-scale Linux shell program, intended to mimic the original Bash (Bourne Again SHell). Given as a mandatory common core project by 42 Schools.

The task is to create a small but functioning shell that mimics the behavior of the original Bourne Again SHell, first released in 1989. This project is the first team collaboration project of the 42 cursus, and must be done by 2 students. The students should collaborate, communicate and build efficiently, and manage their time well to deliver the project within the deadline.

# Instructions
The installation is pretty simple.

1. Clone the repository (`git clone URL [optional-name]`)
2. Navigate inside the folder (`cd 42-minishell`)
3. Run `make` or `make bonus`:
	- If you want only the mandatory part, run `make`. This will create an executable called `minishell`.
	- If you want bonus capabilities too (&&, ||, () and *), run `make bonus`. The executable will be named `minishell_bonus`.
4. Run the executable either with `./minishell` or `./minishell_bonus`

That's about it for basic usage. For more abilities:

- `make clean`: Cleans the generated binary files, leaves out the executable. Works on both mandatory and bonus versions.
- `make fclean`: Same as `make clean`, but removes the executables too.
- `make re`: Runs `make fclean` first, and then rebuilds the mandatory executable.
- `make reb`: Same as `make re`, but outputs the bonus executable.

# Resources

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

## AI Usage
The use of AI in this project is as follows:
- Getting ideas about overall project organization, common pitfalls
- Resource gathering and questions about badly documented libraries and functions where Google was of no use
- A final, static analysis of certain functions and code that's done after it has been thoroughly tested as an extra measure.
- Automating tedious and boring tasks that give no valuable knowledge or experience to the developers (arbitrary file generation, extrapolation of the already known test cases)
- Grammar and spelling check for documentation

Check out the [RESOURCES](RESOURCES.md) file for an additional extensive list of links.

# Features
This project implements both required mandatory and bonus features. On top of these, it also features
extra abilities that are not within the project requirements but could be accomplished using the allowed functions and resources.

## Mandatory
- User prompt
- Single and double quotes
- Simple commands
- Redirections (<, >, <<, >>)
- Pipelines
- Expansion (variable expansion, field splitting, quote removal)
- Builtins (cd, pwd, echo, env, export, unset, exit)
- Environment variables
	- Default variables like `$HOME`, `$PATH` and `$USER` are expected. However, the shell still works fine even without them.
- Exit codes and the special parameter `$?`
	- `$?` expands to these: The last executed command's exit code, parser error code on syntax errors, error code on misused shell abilities, or the last received signal (128 + signal number)
	- All programs return with an exit code.
- Command history navigation using up and down scroll buttons
	- Extra: Thanks to the Readline library, other keyboard controls are available, such as Ctrl-R search in the history. However, this is not required by the original project, and nor has it been implemented by us. It's merely a side effect of the Readline library.
- Signal handling and controls (Ctrl-C, Ctrl-D, Ctrl-\\)
	- Ctrl-C: Sends the SIGINT signal. If in prompt or here-document mode, clears the current prompt and displays a new one. If a command is running, sends the SIGINT signal to it to early termination.
	- Ctrl-D: Sends EOF. If in prompt mode (and the prompt is fully empty), exits the shell. If a program is running, sends EOF to it. Depending on the context of the program, the program may take this EOF and end its input prompt. (For example, running `cat` and then pressing Ctrl-D causes it to exit).
	- Ctrl-\: Sends the SIGQUIT signal. This signal is ignored in prompt mode. When a command is running, the signal is sent to the running program. Depending on whether the program accepts or ignores SIGQUIT, it may exit.

## Bonus
- Conditionals (&& and ||)
	- `&&`: IF token. Executes the command on its left, and only if it returns 0 (success), executes the rightmost command and returns it exit  code.
	- `||`: OR token. Executes the command on its left, and only if it DOESN'T return 0 (on failure), executes the rightmost command and returns its exit code.
- Subshells (Parentheses `()`)
	- Executes the command list inside the parentheses in a cloned environment (fork). This environment is identical to the parent environment, however, changes in the child environment doesn't affect the parent environment.
	- It can also be used to group command lists.
	- Examples:
		- `(ls)`: Simple fork that runs `ls` inside the cloned environment.
		- `(export PATH='newpath' && echo $PATH)`: Changes PATH's value and prints it to standard output. This only affects the cloned environment, doesn't change PATH in the main shell environment.
		- `(exit)`: Immediately exits the cloned environment. It doesn't cause the main shell to exit, and can be safely used.
		- `(exit 27)`: Same as above but sets $? to 27.
		- `(exit && exit && exit && exit)`: Same as above but now it's a long chain.
		- `(cd [ANY PATH])`: Changes the current path of the cloned environment, doesn't affect the parent.
		- `cmd1 && (cmd2 && cmd3)`: Executes cmd1 first, and if it succeeds, only then executes the subshell. Can be used like this for grouping.
- Wildcards & Filename Expansion (the * character, also known as globbing)
	- Matches patterns with the files and directories listed inside the current directory
	- Examples:
		- `*`: Expands to all files and directories, except for hidden files
		- `******`: Same as above since it matches any character
		- `F*`: Matches all filenames that start with capital F
		- `F*e`: Matches all filenames that start with capital F and lowercase e
		- `*t*`: Matches all filenames that have at least one lowercase t in it
		- `*/`: Matches all directories
		- `.*`: Matches all hidden files and directories
		- `.*/`: Matches all hidden directories
		- `./*`: Matches all files and directories but adds a ./ prefix
		- `'*'*`: Searches for filenames that end with the * character literally. 

## Extra Features
Like we said before, there are extra features implemented in this project that are not required neither by the mandatory nor the bonus parts.
These are added simply for fun, for the sake of it, within the bounds of allowed functions. These include:

- Flags:
	- `-v`, `--version`, `--info`: Displays version and author information.
	- `-h`, `--help`: Displays the usage message.
	- `-s`: Stops processing arguments. Which means, every argument after this is taken as a positional parameter.
- Script Execution
	- Usage: `./minishell filename [positional_args]`
	- Minishell can read and execute scripts, as long as they are compatible with the Minishell syntax. It's very limited but can be useful.
	- Binary files are detected early and return an error upon execution.
- Interactive Mode
	- Consistent with the script execution mode, the shell instance decides whether it's interactive
	or non-interactive based on:
		- If it reads from a file
		- If it reads from a pipe
		- If the current session is not a TTY
	- In interactive mode, prompts and other related stuff are printed to standard error. This behavior is prevented in the non-interactive mode (except for the actual errors)
	- Non-interactive mode also doesn't use Readline, which means keyboard controls like Ctrl-C won't work. However, the shell instance will keep accepting the incoming signals.
- Unclosed quotes continue the prompt on a new line, and is repeated after all quotes are matched.
- Special Parameters
	- `$$`: The PID of the shell
	- `$#`: The number of positional parameters
	- `$SHLVL`: The current level of this shell instance. It starts with 1 and increases with each instance of any shell program.
	- `$_`: The last argument of the last executed command.
- Positional Parameters
	- `$0` to `$9`: Expands to the given arguments. `$0` expands to the program name (most likely `./minishell`)
- Redirections with File Descriptors
	- All redirections allow the use of File Descriptors. When present, the given file
	is opened at that FD instead of the standard FDs.
	- Example: `cat 3<Makefile` opens the Makefile in FD 3. Since `cat` doesn't read FD 3, it will stay open and wait for further standard input stream.
- Tilde Expansion (~)
	- Tildes are expanded when they are the only character in a string (or when they are the first character, and the next character starts with an /)
- cd
	- The cd builtin has extra features.
		- `cd`: When run with no arguments, cd changes into the user's home directory. If `$HOME` is not set, this is inhibited and an error message is returned instead.
		- `cd -`: Changes into the previous directory. Uses the value in the `$OLDPWD` variable.

# Architecture - Overview
The project can be separated into 3 parts:

1. Initialization
2. The Main Loop (REPL)
3. Cleanup and exit

## Initialization
Things like early argument processing, file descriptor handling, interactive mode detection, target script file detection happen here. If deemed necessary, the program exits early.

## The Main Loop (REPL)
Most of the program is here. In order:

1. Prepare prompt string
2. Present the prompt string to the user
3. Wait for input
4. Take and process the input (also add it to history if interactive)
5. Lexically analyze the input (the lexer)
6. Feed the tokens from the lexer to the parser and build an AST (Abstract Syntax Tree)
7. Feed the AST to the executor
8. Decide if the command is a builtin or a normal command and prepare it according to that
9. Expansion on the arguments and redirection parameters
10. Execute the command
11. Return the command's exit code
12. Return to the main loop
13. Restart the loop

In any of these steps, further measurements are taken to detect user errors, misuses and syntax errors. Any error recovers gracefully and either restarts the loop, or skips to the Exit part.

## Cleanup and Exit
If an exit signal is received (Ctrl-D, exit command, etc), the program enters the Cleanup and Exit phase.

1. Cleanup: Clean any leftover memory stuff
2. Exit: Exit with the latest exit code (or with the exit command's optional argument if it's provided)

# Development Process
The early development of Minishell began in September 2025. It started off as a simple prompt taker. No parsing and execution were performed in the initial state.

- November 2025: The actual development began as I partnered with Muhammed Emin Ayna (muayna). The first two weeks were reserved for planning, resource gathering and tasking
- Muayna decided to handle the backend of the program (execution, builtins, signals), meanwhile yademirk decided to develop the frontend (lexer, parser, expansion)
- Parser and executor were completed around late December 2025 and early January 2026. At that point, the shell was in a usable state, apart from weird edge cases and lack of variable expansion.
- Variable expansion was added by yademirk around mid-January 2026.
- We started developing the bonus features around mid-to-late January 2026.
- The bonus features were completed in early February 2026.
- Edge cases and bugs were fixed in mid February 2026.
- The project is aimed to be finished in February 2026.

## What We Learned
Minishell, so far, has been the most difficult and challenging, yet fun and educational programming project of our entire lives. We learned in-depth about how shells and Bash work, how to handle processes and redirections, parse commands, build parser tree structures and more. But most importantly, it taught about organizing, planning, testing, communication, and being a good teammate.

# Collaborators
## Main Developers
- Muhammed Emin Ayna (muayna)
	- Builtins, signals, execution
- Yaşam Ensar Demirkıran (yademirk)
	- Lexer, parser, expansion
