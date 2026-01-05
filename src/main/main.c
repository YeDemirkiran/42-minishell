/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid Date        by                   #+#    #+#             */
/*   Updated: 2026/01/05 18:11:38 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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

#define SHELL_NAME "babashell"
#define RL_START "\001"
#define RL_END "\002"

static void	shell_exit(int status)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
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

static t_shell	init_shell(int argc, char **argv, char **envp)
{
	t_shell	shell;

	shell.argc = argc;
	shell.args = argv;
	shell.env = ft_malloc(sizeof(t_god_struct_envp), 0);
	shell.env->envp_list = create_envp_list(envp);
	shell.env->envp_char = envp_list_to_char_arr(shell.env->envp_list);
	shell.exit_status = 0;
	shell.pid = get_pid();
	shell.interactive = is_interactive();
	rl_outstream = stderr;
	return (shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	t_list	*tokens;
	t_ast	*ast;
	char *prompt;

	input = NULL;
	signal(SIGINT, on_sigint);
	shell = init_shell(argc, argv, envp);
	while (1)
	{
		prompt = rl_header(&(shell.env->envp_list), shell.env->envp_char);
		input = read_input(prompt, "> ");
		if (input == NULL)
		{
			if (errno != 0)
			{
				perror("minishell: fatal error");
				shell_exit(EXIT_FAILURE);
			}
			break ;
		}
		add_line_to_history(input);
		tokens = lexical_analyze(input);
		free(input);
		ast = parse(tokens);
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
	sort_malloc(1, 1);
	clear_arr(&(shell.env->envp_char));
	rl_clear_history();
	shell_exit(EXIT_SUCCESS);
}
