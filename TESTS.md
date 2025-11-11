# Shell Tests
Test your shell against these cases and compare their results.

Some of these cases may not be required by the subject. However, you are free to fix them.

## Requirements and Normal Cases (Happy Path)

### Redirection
#### '<' Operator (Infile)
The '<' doesn't just redirect the target file to stdin, it does more. The full syntax is 'num<word', and the input is redirected to the file descriptor 'num'. The Bash syntax allows multiple infile redirections, even if they all share the same num. The last coming redirection to the same number overrides previous redirections.

#### '<<' Operator (Heredoc)
Same with the Infile operator, but its syntax is 'num<<EOF'. It reads input from stdin until it encounters the word choosen as EOF, and redirects it to the number.

#### '>' Operator (Outfile)
The '>' doesn't just redirect stdout to the target file, it does more. The full syntax is 'num>word', and the output is redirected to the file descriptor 'num'. The Bash syntax allows multiple outfile redirections, even if they all share the same num. The last coming redirection to the same number overrides previous redirections.

#### '>>' Operator (Append)
Same with the Outfile operator, but it doesn't open the files in truncate mode. Instead, it appends the changes to the end of the file.

### Variables
#### Environment Variables
- \$PATH: A colon-separated list of directories in which the shell looks for commands.

#### Special Variables
- $_ (underscore): It does two things: First, it's the name (or path if fully given) of the used command, set as an environment variable and passed to the process. Second, it's the name of the last expanded argument in a list, changed after executing the process so both of them are separated.
- $?: Expand to the exit status of the most recently executed foreground pipeline.

### Builtins
You have to implement a total of 7 shell builtins (man builtins).

NOTE: No option doesn't mean no arguments, they are seperate things! For example, you should still be able to provide an exit code to the exit command.

#### echo
You have to implement the 'echo' builtin with the '-n' option.

#### cd
You have to implement the 'cd' builtin with only a relative or an absolute path.

Optional: Providing no argument to cd goes to the $HOME path. Providing the '-' argument goes to the directory you were previously in.

#### pwd
You have to implement the 'pwd' builtin with no options.

#### export
You have to implement the 'export' builtin with no options.

#### unset
You have to implement the 'unset' builtin with no options.

#### env
You have to implement the 'env' builtin with no options or arguments.

#### exit
You have to implement the 'exit' builtin with no options.

### History
Your shell should have a working history. Bash stores the history in the file HISTFILE, and stores up to HISTFILESIZE numbers of history.

- HISTSIZE: The number of commands in history. If set to 0, history utilities will be disabled. If set to a negative number, history is infinite. Default value is 500.
- HISTFILE: The path of the history file is stored in this variable. Bash dumps the current history to this file upon exit, capped by the maximum lines of history (HISTFILESIZE).

### Line Editing
Using GNU Readline grants you command line editing tools and controls.

- Tab: Autocomplete
	- Expected: Autocompletes words, either by expanding them to commands or files in the current directory.
	- Current: Autocompletes filenames as expected, but doesn't autocomplete commands.
- ↑ (Up Arrow): History up
	- Expected: Goes up in the history and replaces the current line with that line.
	- Current: Doesn't work yet (no history).
- ↓ (Bottom Arrow): History down
	- Expected: Goes down in the history and replaces the current line with that line.
	- Current: Doesn't work yet (no history).
- Ctrl + R: Search in history
	- Expected: Replaces the current prompt with an interactive 'search mode' field. It searches the commands in history as you type.
	- Current: Ctrl + R works and activates the search field, but it can't find anything in history because there is no history yet.
- Ctrl + C: Cancels the current command and presents a new prompt
	- Current: Cancels the current prompt but doesn't cancel the running process
- Ctrl + D: Closes the current Bash instance, possibly also closing the terminal-emulator
	- Current: Doesn't work

#### Optional Line Editing Tools
GNU Readline grants you extra line editing capabilities. These come by default when you use Readline, so you don't have to implement them on your own. However, it's a good idea to test them and check if they work correctly and as expected.

- Ctrl + F:  Move the cursor one character to the right
	- Status: Works
- Ctrl + B: Move the cursor one character to the left
	- Status: Works
- Alt + F: Move the cursor one word to the right
	- Status: Works
- Alt + B: Move the cursor one word to the left
	- Status works
- Ctrl + A: Move the cursor to the beginning of the current commandline
	- Status: Works
- Ctrl + E: Move the cursor to the end of the current commandline
	- Status: Works
- Ctrl + S: Put the terminal to sleep
	- Status: Works
- Ctrl + Q: Wake the terminal; buffered keypresses are then processed
	- Status: Works
- Ctrl + W: Remove one word to the left of the cursor
	- Status: Works
- Ctrl + Z: Stop a foregrounded process
	- Status: Doesn't work. Stops the minishell instance instead.

### Exit Status
Holds either:
- The exit status of the last executed command in a command chain in a variable ($?), or,
- Retrieved signals. If it's a signal, the exit status is '128 + signal number'.

## Edge Cases

### Input
- Very long argument
	- Bash: Returns "bash: /usr/lib/command-not-found: Argument list too long" error. (Bash uses /usr/lib/command-not-found when the given command is not found.)
	- Minishell: (none)
- Very long argument list
	- Bash: Works slow but still works.
	- Minishell: (none)

### Quoting
(to be added)

### Built-ins
#### echo
- No arguments (just echo)
	- Bash: Prints empty newline
	- Minishell: (none)
- No arguments with option -n (echo -n)
	- Bash: Prints nothing
	- Minishell: (none)
- Multiple arguments
	- Bash: Prints them all
	- Minishell: (none)
- Option -n after arguments
	- Bash: Ignores -n as an option, prints it as it is
	- Minishell: (none)
- Multiple -n (seperate, like echo -n -n -n -n)
	- Bash: Same as singular -n
	- Minishell: (none)
- Multiple -n (single flag, like echo -nnnnnn)
	- Bash: Same as singular -n
	- Minishell: (none)
- Multiple -n (combined, like echo -nn -n -nnn -n -n -nn)
	- Bash: Same as singular -n
	- Minishell: (none)
- Multiple -n between arguments (like echo -nn -n arg -nnn -n -n arg -nn arg)
	- Bash: Treats -n's as options until the first word, then the -n's after that are printed too.
	- Minishell: (none)
- Option --n, singular and multiple, all cases
	- Bash: Isn't used as -n option, printed as they are
	- Minishell: (none)

#### cd
- cd using pipes
	- Bash: Doesn't change the directory (works inside the forked process)
	- Minishell: (none)

- cd using spaces or dots as dirname (cd "  ", cd "......" etc)
	- Bash: Treats it as a valid dirname, goes there if it exists
	- Minishell: (none)

- cd empty argument (cd "")
	- Bash: Nothing happens.
	- Minishell: (none)

- cd very long argument (1000+ chars)
	- Bash: Returns "bash: cd: File name too long" error.
	- Minishell: (none)

- cd multiple arguments (no matter the size & count)
	- Bash: Returns "bash: cd: too many arguments" error
	- Minishell: (none)

- cd no arguments
	- Bash: Goes to the $HOME dir
	- Minishell: (none)

- cd no arguments with $HOME unset
	- Bash: Returns "bash: cd: HOME not set" error
	- Minishell: (none)

- cd with $CDPATH set, and two directories with the same name on $CDPATH and the current directory
	- Bash: Goes to the CDPATH version of the directory, ignores the local directory. Also prints the full name of the directory it switched to. So, works like $PATH and commands.
	- Minishell: (none)

#### pwd
- pwd with arguments
	- Bash: Arguments ignored, does its job as usual
	- Minishell: (None)

#### export
- no arguments
	- Bash: Lists all ENV variables
	- Minishell: (none)
- Single argument with no value and no assignment sign (example: export VAR)
	- Bash: Doesn't print anything and doesn't set up an ENV variable, effectively ignored.
	- Minishell: (none)
- Single argument with no value and no assignment sign with an already existing variable
	- Bash: Doesn't print anything and doesn't change the existing ENV variable, effectively ignored.
	- Minishell: (none)
- Single argument with no value after the assignment sign (example: export VAR=)
	- Bash: Sets up an ENV variable with the same name and key, works like normal.
	- Minishell: (none)
- Single argument with no value after the assignment sign with an already existing variable
	- Bash: Sets the existing variable to be empty (VAR=""), but doesn't remove it.
	- Minishell: (none)
- Single argument with empty (Example: export VAR="", VAR=" " etc)
	- Bash: Sets up an ENV variable with the same name and key, works like normal.
	- Minishell: (none)
- Multiple arguments with assignments
	- Bash: Sets up each argument an ENV variable with the same name and key, works like singular.
	- Minishell: (none)

#### unset
- With no args:
	- Bash: Nothing happens.
	- Minishell: (none)
- Trying on non-existing variables
	- Bash: Nothing happens.
	- Minishell: (none)
- Multiple arguments
	- Bash: Unsets all arguments if they exist
	- Minishell: (none)
- On errors
	- Bash: Tries to unset each arg, prints a seperate error message for each of them if they fail
	- Minishell: (none)
- Unsetting readonly
	- Bash: Throws error "bash: unset: (var_name): cannot unset: readonly variable"
	- Minishell: (none)

#### env
(to be added)

#### exit
- exit with more than 1 argument
	- Bash: Prints "exit" first, then prints "bash: exit: too many arguments". Doesn't actually exit the shell.
	- Minishell: (none)
- Non-numeric argument
	- Bash: Prints "exit" first, then prints "bash: exit: a: numeric argument required", but then actually exits with code 2.
	- Minishell: (none)
- Out-of-range exit code (not between 0-255)
	- Bash: Exits, wraps the exit status around 255. -1 becomes 255, -2 becomes 254, 1000 becomes 232 and so on.
	- Minishell: (none)
- Very large numeric value (over MAX_LONG: 9223372036854775807)
	- Bash: Same as non-numeric argument error.
	- Minishell (none)

### Pipes
- Only the | symbol
	- Bash: Throws "bash: syntax error near unexpected token `|'"
	- Minishell: (none)
- Only the | symbol, single quoted ('|')
	- Bash: Says "|: command not found"
	- Minishell: (none)
- Only the | symbol, double quoted ("|")
	- Bash: Same as single quoted
	- Minishell: (none)
- Empty pipe in pipeline (cat Makefile | | wc -l)
	- Bash: Throws "bash: syntax error near unexpected token `|'"
	- Minishell: (none)
- No spaces between pipes (cat Makefile|sort|wc -l)
	- Bash: Works like normal pipes, no error.
	- Minishell: (none)
- Nothing right to the last pipe char ("cat Makefile | ")
	- Bash: Prompts new line ("> "), continues to take input for the pipeline
	- Minishell: (none)
- Long pipe count:
	- 25
		- Bash: Works as expected
		- Minishell: (none)
	- 50
		- Bash: Works as expected
		- Minishell: (none)
	- 100
		- Bash: Works as expected
		- Minishell: (none)
	- 500
		- Bash: Works, but slow (0.5s delay)
		- Minishell: (none)
	- 1000
		- Bash: Works, but slow (0.5s delay)
		- Minishell: (none)
	- 2500
		- Bash: Worksi, but slow (1s delay)
		- Minishell: (none)

### Redirection
#### < (Infile)
- Just <
	- Bash: Says "bash: syntax error near unexpected token `newline'"
	- Minishell: (none)
- Just < but single quoted ('<')
	- Bash: Says "<: command not found"
	- Minishell: (none)
- Just < but double quoted ("<")
	- Bash: Says "<: command not found"
	- Minishell: (none)
- < with an argument but only that (e.g. "< Makefile")
	- Bash: Does nothing if the file exists. Throws "bash: (file_name): No such file or directory" otherwise.
	- Minishell: (none)
- < (filename) <
	- Bash: Same as syntax error
	- Minishell: (none)
- Multiple valid <
	- Bash: Opens and redirects each file to stdin, up to the last one. But only the last one is processed because it stays as stdin.
	- Minishell: (none)

#### > (Outfile)
- Just >
	- Bash: Says "bash: syntax error near unexpected token `newline'"
	- Minishell: (none)
- Just > but single quoted ('>')
	- Bash: Says ">: command not found"
	- Minishell: (none)
- Just > but double quoted (">")
	- Bash: Says ">: command not found"
	- Minishell: (none)
- > with an argument but only that (e.g. "> Makefile")
	- Bash: Overwrites the files if it exists, creates a new file if it doesn't. Since we didn't execute a command, it does nothing else.
	- Minishell: (none)
- > (filename) >
	- Bash: Same as syntax error, doesn't create or truncate any file.
	- Minishell: (none)
- Multiple valid >
	- Bash: Opens/creates and redirects stdout to each, up to the last one. But only the last one is processed because it stays as stdout.
	- Minishell: (none)
- > with full syntax but no command (fd>file)
	- Bash: Overwrites the files if it exists, creates a new file if it doesn't. Since we didn't execute a command, it does nothing else.
	- Minishell: (none)
- > with full syntax but no command, single quoted ('fd>file')
	- Bash: It takes it as a command, says "couldn't be found" if it doesn't exist.
	- Minishell: (none)
- > with full syntax but no command, double quoted ("fd>file")
	- Bash: The same as single quoted.
	- Minishell: (none)

#### Heredoc (<<)
- (to be added)

#### >> (Append)
- (to be added)

### Pipelines & Redirection Combination
- One pipe, second command using infile
	- Bash: Infile takes precedence over pipe's in
	- Minishell: (none)
- One pipe, first command using outfile
	- Bash: Outfile takes precedence over pipe's out
	- Minishell: (none)
### Variables
#### Environment Variables
##### $PATH
- Empty $PATH
	- Bash: If PATH is empty, or there is an empty (null) path provided amongs the other paths, the current directory is included in the path.
	- Minishell: (none)

##### $SHLVL
SHLVL is always set to the current level of the nested shells. It starts from 1. If you type bash and enter, it will be 2. Then 3, then 4, and so on.

#### Special Variables

### History
- Nested Bash instances
	- Bash: Nested instances use .bash_history, ignore the parent shell's history. Upon exiting, they save their history to .bash_history.
	- Minishell: (none)

### Exit Status
- (to be added)
