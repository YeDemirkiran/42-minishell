/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:55:29 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 15:05:55 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include "modules/executor.h"
#include "modules/ast.h"
#include "modules/signal.h"
#include "modules/utils.h"
#include "gb.h"
#include <minishell_lb.h>

static void	all_free_in_fork(t_shell *shell)
{
	ast_free_tree(shell->root_ast);
	clear_arr(&shell->env->envp_char);
	ft_malloc(1, 1);
	sort_malloc(1, 1);
}

void	run_command_fork(t_fork_variable_list_for_norm norm_solver,
		t_shell *shell, int fd_in, int fd_out)
{
	t_list	*redirs;
	t_ast	*ast;
	char	**args;

	ast = norm_solver.ast;
	args = (*norm_solver.args);
	install_default_signals();
	dup2(fd_in, 0);
	dup2(fd_out, 1);
	close_child_user_fd();
	redirs = ast->data->ast_command.redirs;
	check_redirs_status_in_fork(redirs);
	if (ast->data->ast_command.arguments == NULL)
	{
		all_free_in_fork(shell);
		clear_arr(norm_solver.args);
		exit(EXIT_SUCCESS);
	}
	check_builtin_status(ast, shell, 0, &args);
	execve(command_path(ast, (*shell)), args, shell->env->envp_char);
	print_exec_error_message(args, shell->env->envp_char, shell->interactive);
	all_free_in_fork(shell);
	clear_arr(norm_solver.args);
	exit(errno_to_exit_code());
}
