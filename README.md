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
