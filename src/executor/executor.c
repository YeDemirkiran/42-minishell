/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2026/01/11 22:26:48 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#define _GNU_SOURCE
#include "stdio.h"
#include <libft.h>
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "fcntl.h"
#include "sys/wait.h"
#include "signal.h"
#include "../../lib/garbage_collector/gb.h"

#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "enums/e_redir_type.h"
#include <minishell_lb.h>
#include "modules/executor.h"
#include "modules/builtin.h"
#include "modules/signal.h"

pid_t run_command(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out)
{
	pid_t pid;
	t_list *redirs;
	char **args;

	args = list_to_char_arr(ast->data->ast_command.arguments);
	pid = fork();
	install_command_signals();
	if(pid == 0)
	{
		install_default_signals();
		dup2(fd_in, 0);
		dup2(fd_out, 1);
		redirs = ast->data->ast_command.redirs;
		while (redirs != NULL)
			run_redirs(&redirs);
		if(isbuiltin((char *)(ast->data->ast_command.arguments->content)))
		{
			run_builtin(ast, &(*list)->envp_char, &(*list)->envp_list);
			exit(EXIT_SUCCESS);
		}
		execve(command_path(ast), args, (*list)->envp_char);
		perror("-babashell: command not found");
		exit(EXIT_FAILURE);
	}
	clear_arr(&args);
	return pid;
}

void	exec_utils(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out, t_list **lst_pid)
{
	int pipefd[2];
	pid_t	*pid;

	if (ast == NULL)
		return ;
	if(ast->type == AST_COMMAND)
	{
		pid = malloc(sizeof(pid_t));
		*pid = run_command(ast, list, fd_in, fd_out);
		ft_lstadd_back(lst_pid, ft_lstnew(pid));
	}
	else if (ast->type == AST_PIPE)
	{
		pipe(pipefd);
		exec_utils(ast->left, list, fd_in, pipefd[1], lst_pid);
		close(pipefd[1]);
		exec_utils(ast->right, list, pipefd[0], fd_out, lst_pid);
		close(pipefd[0]);
	}
	return ;
}

#ifdef WCOREDUMP

int	has_core_dump(int status)
{
	return (WCOREDUMP(status));
}

#else

int	has_core_dump(int status)
{
	return (status & 0x80);
}

#endif

static void	print_signal_message(int sig, int status)
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

int	executor(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out)
{
	t_list	*lst;
	t_list	*root;
	int		exit_status;
	int		sig;

	lst = NULL;
	exec_utils(ast, list, fd_in, fd_out, &lst);
	root = lst;
	exit_status = 0;
	while (lst != NULL)
	{
		waitpid(*((pid_t *)(lst->content)), &exit_status, 0);
		lst = lst->next;
	}
	ft_lstclear(&root, free);
	if (WIFSIGNALED(exit_status))
	{
		sig = WTERMSIG(exit_status);
		print_signal_message(sig, exit_status);
		return (128 + sig);
	}
	return (WEXITSTATUS(exit_status));
}

