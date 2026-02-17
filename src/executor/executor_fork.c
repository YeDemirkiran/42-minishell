/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:55:29 by muayna            #+#    #+#             */
/*   Updated: 2026/02/17 19:28:34 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../../lib/garbage_collector/gb.h"
#include "enums/e_redir_type.h"
#include "fcntl.h"
#include "modules/ast.h"
#include "modules/builtin.h"
#include "modules/executor.h"
#include "modules/expansion.h"
#include "modules/signal.h"
#include "modules/utils.h"
#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "sys/wait.h"
#include "unistd.h"
#include <errno.h>
#include <libft.h>
#include <minishell_lb.h>
#include <readline/readline.h>

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
		exit(EXIT_SUCCESS);
	check_builtin_status(ast, shell, 0, &args);
	execve(command_path(ast, (*shell)), args, shell->env->envp_char);
	print_exec_error_message(args, shell->env->envp_char, shell->interactive);
	ft_malloc(1, 1);
	sort_malloc(1, 1);
	ast_free_tree(shell->root_ast);
	clear_arr(&shell->env->envp_char);
	exit(errno_to_exit_code());
}
