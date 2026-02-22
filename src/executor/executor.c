/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 15:18:58 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <structs/s_redirection.h>
#include <structs/s_shell.h>
#include <enums/e_redir_type.h>
#include <modules/expansion.h>
#include <modules/signal.h>
#include <libft.h>

/**
 * ? This func close all parent fds in child (without system fd 0,1,2).
 * *	Mean when use for example cat | ls,
		cat need input but does not have input,
		ls working fine and will be finished.
 * *	System waiting cat because cat input door(pipefd[0]) is open,
		if you use this func. close all parent fds,
		mean close(pipefd) force way.
 * *	After that system looking input door and system see input door is closed,
		and system kill the cat func.
 */

static void	run_exec_dup_usage_for_norm(int *std, int select_type)
{
	if (select_type == 0)
	{
		std[0] = dup(STDIN_FILENO);
		std[1] = dup(STDOUT_FILENO);
		std[2] = dup(STDERR_FILENO);
	}
	else if (select_type == 1)
	{
		dup2(std[0], STDIN_FILENO);
		dup2(std[1], STDOUT_FILENO);
		dup2(std[2], STDERR_FILENO);
		close(std[0]);
		close(std[1]);
		close(std[2]);
	}
}

static int	run_redir_loop(t_list *redir, t_shell *shell)
{
	while (redir != NULL)
	{
		if (run_redirs(&redir) != 1)
		{
			shell->exit_status = 1;
			return (0);
		}
	}
	return (1);
}

static void	run_exec(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	int		std[3];
	t_list	*redir;

	if (ast->type == AST_COMMAND && ast->data->ast_command.arguments != NULL
		&& isbuiltin((char *)(ast->data->ast_command.arguments->content)))
	{
		if (expansion(ast, *shell) != 1)
			return ;
		run_exec_dup_usage_for_norm(std, 0);
		if (fd_in != STDIN_FILENO)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != STDERR_FILENO)
			dup2(fd_out, STDOUT_FILENO);
		redir = ast->data->ast_command.redirs;
		if (run_redir_loop(redir, shell) != 1)
			return ;
		if (shell->root_ast->type == AST_SUBSHELL)
			run_builtin(ast, shell, 0);
		else
			run_builtin(ast, shell, 1);
		run_exec_dup_usage_for_norm(std, 1);
	}
	else
		shell->exit_status = executor(ast, shell, 0, 1);
}

/**
 * * 		For run command for executor
 * @return	Forks pid
 */
pid_t	run_command(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	pid_t							pid;
	t_fork_variable_list_for_norm	norm_solver;
	char							**args;

	if (expansion(ast, *shell) != 1)
		return (-1);
	args = list_to_char_arr(ast->data->ast_command.arguments);
	if (args != NULL && isbuiltin(args[0]) && (shell->root_shell_type == AST_OR
			|| shell->root_shell_type == AST_IF))
	{
		clear_arr(&args);
		run_exec(ast, shell, fd_in, fd_out);
		return (-1);
	}
	norm_solver.args = &args;
	norm_solver.ast = ast;
	pid = fork();
	install_command_signals();
	if (pid == 0)
		run_command_fork(norm_solver, shell, fd_in, fd_out);
	if (args != NULL)
		clear_arr(&args);
	return (pid);
}

int	executor(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	if (ast == NULL || shell == NULL)
		return (-1);
	update_special_parameter(ast, shell);
	exec_utils(ast, shell, fd_in, fd_out);
	return (wait_for_pipeline(shell));
}
