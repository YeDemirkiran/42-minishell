/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 12:01:22 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 14:45:37 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include <readline/readline.h>

#include "modules/utils.h"
#include "modules/expansion.h"

#include "args.h"
#include "shell_init.h"

void	init_main(t_shell *shell, int argc, char **argv, char **envp)
{
	int	init_result;

	if (process_args((const char **)argv, argc) == 0)
		exit(EXIT_SUCCESS);
	init_result = init_shell(shell, argc, argv, envp);
	if (init_result != 0)
		shell_exit(init_result, *shell);
	rl_catch_signals = 0;
	rl_event_hook = rl_event;
}

static int	should_run_builtin(t_ast *ast)
{
	t_list	*args;

	if (ast->type != AST_COMMAND)
		return (0);
	args = ast->data->ast_command.arguments;
	if (args == NULL)
		return (0);
	if (args->content == NULL)
		return (0);
	if (isbuiltin((char *)(args->content)) == 0)
		return (0);
	return (1);
}

static int	run_redir_loop(t_list *redir, t_shell *shell, int *std)
{
	while (redir != NULL)
	{
		if (run_redirs(&redir) != 1)
		{
			shell->exit_status = 1;
			dup2(std[0], STDIN_FILENO);
			dup2(std[1], STDOUT_FILENO);
			dup2(std[2], STDERR_FILENO);
			close(std[0]);
			close(std[1]);
			close(std[2]);
			return (0);
		}
	}
	return (1);
}

void	run_exec(t_ast *ast, t_shell *shell)
{
	int		std[3];
	t_list	*redir;

	if (ast == NULL)
		return ;
	if (should_run_builtin(ast) != 0)
	{
		if (expansion(ast, *shell) != 1)
			return ;
		std[0] = dup(STDIN_FILENO);
		std[1] = dup(STDOUT_FILENO);
		std[2] = dup(STDERR_FILENO);
		redir = ast->data->ast_command.redirs;
		if (run_redir_loop(redir, shell, std) != 1)
			return ;
		run_builtin(ast, shell, 1);
		dup2(std[0], STDIN_FILENO);
		dup2(std[1], STDOUT_FILENO);
		dup2(std[2], STDERR_FILENO);
		close(std[0]);
		close(std[1]);
		close(std[2]);
	}
	else
		shell->exit_status = executor(ast, shell, 0, 1);
}
