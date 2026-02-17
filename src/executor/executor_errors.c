/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:35:45 by muayna            #+#    #+#             */
/*   Updated: 2026/02/17 16:28:52 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "../../lib/garbage_collector/gb.h"
#include "enums/e_redir_type.h"
#include "fcntl.h"
#include "modules/ast.h"
#include "modules/builtin.h"
#include "modules/executor.h"
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

void	print_exec_error_message(char **args, char **envp, int interactive)
{
	char	*not_found_args[3];

	if (errno == ENOENT || args[0][0] == '\0')
	{
		if (interactive == 1)
		{
			not_found_args[0] = "/usr/lib/command-not-found";
			not_found_args[1] = args[0];
			not_found_args[2] = NULL;
			execve(not_found_args[0], not_found_args, envp);
		}
		error_message("minishell", args[0], "command not found\n");
	}
	else
		error_message("minishell", args[0], NULL);
}

void	print_signal_message(int sig, int status)
{
	if (sig == SIGQUIT)
		ft_putstr_fd("Quit", STDERR_FILENO);
	else if (sig == SIGSEGV)
		ft_putstr_fd("Segmentation fault", STDERR_FILENO);
	else if (sig == SIGFPE)
		ft_putstr_fd("Floating point exception", STDERR_FILENO);
	else if (sig == SIGILL)
		ft_putstr_fd("Illegal instruction", STDERR_FILENO);
	else if (sig == SIGBUS)
		ft_putstr_fd("Bus error", STDERR_FILENO);
	else if (sig == SIGTERM)
		ft_putstr_fd("Terminated", STDERR_FILENO);
	else if (sig == SIGKILL)
		ft_putstr_fd("Killed", STDERR_FILENO);
	else if (sig == SIGUSR1)
		ft_putstr_fd("User defined signal 1", STDERR_FILENO);
	else if (sig == SIGUSR2)
		ft_putstr_fd("User defined signal 2", STDERR_FILENO);
	else if (sig == SIGPIPE)
		ft_putstr_fd("Broken pipe", STDERR_FILENO);
	if (has_core_dump(status) > 0)
		ft_putstr_fd(" (core dumped)", STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
}

void	check_builtin_status(t_ast *ast, t_shell *shell, int shell_print_exit,
		char ***args)
{
	if (isbuiltin((char *)(ast->data->ast_command.arguments->content)))
	{
		run_builtin(ast, shell, shell_print_exit);
		ast_free_tree(ast);
		sort_malloc(1, 1);
		clear_arr(&(shell->env->envp_char));
		rl_clear_history();
		ft_malloc(1, 1);
		clear_arr(args);
		exit(shell->exit_status);
	}
}

void	close_child_user_fd(void)
{
	int	fds;

	fds = 3;
	while (fds < 500)
	{
		close(fds);
		fds++;
	}
}
