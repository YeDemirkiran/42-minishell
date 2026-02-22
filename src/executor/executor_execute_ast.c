/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_execute_ast.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:49:28 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 15:01:06 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"
#include "modules/executor.h"
#include "modules/ast.h"
#include "gb.h"
#include <minishell_lb.h>
#include "sys/wait.h"

static void	run_pipe(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return ;
	shell->root_shell_type = AST_PIPE;
	exec_utils(ast->left, shell, fd_in, pipefd[1]);
	close(pipefd[1]);
	shell->root_shell_type = AST_PIPE;
	exec_utils(ast->right, shell, pipefd[0], fd_out);
	close(pipefd[0]);
}

static void	run_if(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	int	status;

	shell->root_shell_type = AST_IF;
	exec_utils(ast->left, shell, fd_in, fd_out);
	status = wait_for_pipeline(shell);
	shell->exit_status = status;
	if (status == 0)
	{
		shell->root_shell_type = AST_IF;
		exec_utils(ast->right, shell, fd_in, fd_out);
	}
}

static void	run_or(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	int	status;

	shell->root_shell_type = AST_OR;
	exec_utils(ast->left, shell, fd_in, fd_out);
	status = wait_for_pipeline(shell);
	shell->exit_status = status;
	if (status != 0)
	{
		shell->root_shell_type = AST_OR;
		exec_utils(ast->right, shell, fd_in, fd_out);
	}
}

static void	run_subshell(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	pid_t	pid_subshell;
	int		status;

	shell->root_shell_type = AST_SUBSHELL;
	pid_subshell = fork();
	if (pid_subshell == 0)
	{
		exec_utils(ast->left, shell, fd_in, fd_out);
		status = wait_for_pipeline(shell);
		ast_free_tree(shell->root_ast);
		clear_arr(&shell->env->envp_char);
		ft_malloc(1, 1);
		sort_malloc(1, 1);
		exit(status);
	}
	waitpid(pid_subshell, &status, 0);
	shell->exit_status = WEXITSTATUS(status);
}

void	exec_utils(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	pid_t	pid;

	if (ast == NULL)
		return ;
	if (ast->type == AST_COMMAND)
	{
		pid = run_command(ast, shell, fd_in, fd_out);
		create_pid_list(shell->env->pid_list, pid);
	}
	else if (ast->type == AST_PIPE)
		run_pipe(ast, shell, fd_in, fd_out);
	else if (ast->type == AST_IF)
		run_if(ast, shell, fd_in, fd_out);
	else if (ast->type == AST_OR)
		run_or(ast, shell, fd_in, fd_out);
	else if (ast->type == AST_SUBSHELL)
		run_subshell(ast, shell, fd_in, fd_out);
}
