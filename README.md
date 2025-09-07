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
GNU Bash Manual: https://www.gnu.org/software/bash/manual/
Bash Wikipedia Article: https://en.wikipedia.org/wiki/Bash_(Unix_shell)
Shell Terminology: https://www.ibm.com/docs/en/aix/7.1.0?topic=concepts-shells-terminology
Shell Expansion: https://www.spsanderson.com/steveondata/posts/2024-10-18/

# Common Pitfalls

## GNU Readline Memory Leaks
Sometimes, using GNU Readline may cause leftover "still reachable memory" errors in Valgrind. Although we are not required to fix them, the subject document implies that it is possible to fix it. At the time of writing this, I don't know how to fix it. I will update this section with an explanation of a possible fix, as soon as I implement it myself.

(more coming soon)
