# Shell Tests
Test your shell against these cases and compare their results.

Some of these cases may not be required by the subject. However, you are free to fix them.

## Requirements and Normal Cases (Happy Path)

### Variables

#### Environment Variables
- \$PATH: A colon-separated list of directories in which the shell looks for commands.

#### Special Variables
- $_ (underscore): It does two things: First, it's the name (or path if fully given) of the used command, set as an environment variable and passed to the process. Second, it's the name of the last expanded argument in a list, changed after executing the process so both of them are separated.

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
- Retrieved signals. If it's a signal, the exit status is '127 + signal number'.

## Edge Cases
### Built-ins
#### cd
- cd using pipes
	- Expected: Doesn't change the directory (works inside the fork)
	- Current: (none)

#### echo
- echo $((1 / 0)) [NOT REQUIRED]
	- Expected: Returns error message "bash: division by zero".
	- Current: (none)

### Pipes
- (to be added)

### Redirection
#### < (Herefile)
- (to be added)

#### > (Write)
- (to be added)

#### >> (Append)
- (to be added)

### Heredoc (<<)
- (to be added)

### Variables
#### Environment Variables
##### $PATH
- If PATH is empty, or there is an empty (null) path provided amongs the other paths, the current directory is included in the path.

### Exit Status
- (to be added)
