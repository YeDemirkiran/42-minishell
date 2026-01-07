/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2026/01/07 12:53:12 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <readline/readline.h>

#include <libft/linked_list.h>
#include <libft/string.h>
#include <libft/predicate.h>
#include <get_next_line.h>
#include <minishell_lb.h>
#include <gb.h>

#include "macros/m_colors.h"
#include "structs/s_ast.h"
#include "structs/s_shell.h"

#include "modules/lexer.h"
#include "modules/ast.h"
#include "modules/parser.h"
#include "modules/expansion.h"
#include "modules/executor.h"
#include "modules/builtin.h"
#include "modules/signal.h"

#include "../lexer/token.h"
#include "read_input.h"
#include "history.h"
#include "get_pid.h"

#define RL_START "\001"
#define RL_END "\002"

/**
 * @brief Prints out an error message with perror in this format:
 *
 * shell_name: [optional_field: ] perror() message
 */
static void	shell_error_message(t_shell shell, char *extra_field)
{
	char	*shell_name;

	shell_name = "minishell";
	if (shell.args != NULL && shell.args[0] != NULL)
		shell_name = shell.args[0];
	ft_putstr_fd(shell_name, STDERR_FILENO);
	if (extra_field != NULL)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(extra_field, STDERR_FILENO);
	}
	ft_putstr_fd(": ", STDERR_FILENO);
	perror(NULL);
}

/**
 * @brief Prints an optional exit messsage depending on 'interactive'
 * and exits with status.
 */
static void	shell_exit(int status, t_shell shell)
{
	sort_malloc(1, 1);
	clear_arr(&(shell.env->envp_char));

	if (shell.interactive == 1)
	{
		rl_clear_history();
		ft_putstr_fd("exit\n", STDERR_FILENO);
	}
	exit(status);
}

/**
 * @brief Determines whether the current session is interactive.
 * @return 0 if non-interactive, 1 if interactive, -1 on error.
 */
static int	is_interactive(void)
{
	int	interactive;

	interactive = 0;
	if (isatty(STDIN_FILENO) == 1)
		interactive++;
	else if (errno != ENOTTY)
		return (-1);
	if (isatty(STDERR_FILENO) == 1)
		interactive++;
	else if (errno != ENOTTY)
		return (-1);
	errno = 0;
	return (interactive == 2);
}

static int	errno_to_exit_code(void)
{
	int	code;

	code = 0;
	if (errno == EACCES)
		code = 126;
	else if (errno = ENOENT)
		code = 127;
	return (code);
}

/**
 * @brief Opens the script from argv's second argument (index 1)
 * and redirects it to STDIN via dup2, and modifies the given shell struct
 * according to it.
 * @return 0 on failure, 1 on success.
 */
static int	open_stdin_script(t_shell *shell, int argc, char **argv)
{
	int	fd;
	int	dup_result;

	shell->interactive = 0;
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
	{
		shell_error_message(*shell, argv[1]);
		return (errno_to_exit_code());
	}
	else if (fd != STDIN_FILENO)
	{
		dup_result = dup2(fd, STDIN_FILENO);
		close(fd);
		if (dup_result == -1)
		{
			shell_error_message(*shell, argv[1]);
			return (0);
		}
	}
	shell->argc = argc - 1;
	shell->args = argv + 1;
	return (1);
}

/**
 * @brief Takes a pointer to a shell struct and initializes it.
 * Argument processing and script file opening also happens here.
 * @return 0 on failure, 1 on success.
 */
static int	init_shell(t_shell *shell, int argc, char **argv, char **envp)
{
	shell->pid = get_pid();
	shell->interactive = -1;
	shell->exit_status = 0;
	shell->argc = argc;
	shell->args = argv;
	shell->env = ft_malloc(sizeof(t_god_struct_envp), 0);
	shell->env->envp_list = create_envp_list(envp);
	shell->env->envp_char = envp_list_to_char_arr(shell->env->envp_list);
	if (argc > 1)
		return (open_stdin_script(shell, argc, argv));
	if (shell->interactive == -1)
		shell->interactive = is_interactive();
	if (shell->interactive == 1)
		rl_outstream = stderr;
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	t_list	*tokens;
	t_ast	*ast;
	char *prompt;
	int		init_result;

	input = NULL;
	signal(SIGINT, on_sigint);
	init_result = init_shell(&shell, argc, argv, envp);
	if (init_result != 1)
		shell_exit(init_result, shell);
	while (1)
	{
		prompt = rl_header(&(shell.env->envp_list), shell.env->envp_char);
		input = read_input(prompt, "> ", shell.interactive);
		if (input == NULL)
		{
			if (errno != 0)
			{
				perror("minishell: fatal error");
				shell_exit(EXIT_FAILURE, shell);
			}
			break ;
		}
		if (shell.interactive == 1)
			add_line_to_history(input);
		tokens = lexical_analyze(input);
		free(input);
		ast = parse(tokens, shell);
		if (expansion(ast, shell) != 1)
		{
			free_tokens(&tokens);
			ast_free_tree(ast);
			continue ;
		}
		if(ast->type == AST_COMMAND && isbuiltin((char *)(ast->data->ast_command.arguments->content)))
			run_builtin(ast, &(shell.env->envp_char), &(shell.env->envp_list));
		else
			shell.exit_status = executor(ast, &(shell.env), 0, 1);
		free_tokens(&tokens);
		ast_free_tree(ast);
		sort_malloc(1, 1);
	}
	shell_exit(EXIT_SUCCESS, shell);
}
