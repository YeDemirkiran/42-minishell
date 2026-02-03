/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2026/01/29 15:59:02 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "stdio.h"
#include <libft.h>
#include <errno.h>
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
#include "modules/utils.h"
/**
* ? This func close all parent fds in child (without system fd 0,1,2).
* *	Mean when use for example cat | ls, cat need input but does not have input, ls working fine and will be finished.
* *	System waiting cat because cat input door(pipefd[0]) is open, if you use this func. close all parent fds, mean close(pipefd) force way.
* *	After that system looking input door and system see input door is closed, and system kill the cat func.
*/
static void close_child_user_fd(void)
{
	int fds;

	fds = 3;
	while (fds < 500)
	{
		close(fds);
		fds++;
	}
}

static void check_builtin_status(t_ast *ast, t_shell *shell, int shell_print_exit)
{
	if(isbuiltin((char *)(ast->data->ast_command.arguments->content)))
	{
		run_builtin(ast, shell, shell_print_exit);
		exit(shell->exit_status);
	}
}

static void print_exec_error_message(char **args, char **envp, int interactive)
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

static void check_redirs_status_in_fork(t_list *redirs)
{
	int		redir_result;

	while (redirs != NULL)
	{
		redir_result = run_redirs(&redirs);
		if (redir_result != 1)
			exit(EXIT_FAILURE);
	}
}
/**
* ? This func. create pid_list for executor
* ! Example : cat | ls
* *	If you don't use like this structure, cat wait input and ls does not working.
* *	But if you use this structure, system run 2 command in same time, ls working and finished, ls output write on terminal. Cat still wait input but ls is finished, you can see ls output in terminal.
*/
static void	create_pid_list(t_pid_list *root_pid_list, pid_t pid)
{
	if(root_pid_list->pid == 0)
	{
		root_pid_list->pid = pid;
		root_pid_list->next = NULL;
	}
	else
	{
		while(root_pid_list->next != NULL)
			root_pid_list = root_pid_list->next;
		root_pid_list->next = sort_malloc(sizeof(t_pid_list), 0);
		if(root_pid_list->next == NULL)
			return ;
		root_pid_list->next->pid = pid;
		root_pid_list = root_pid_list->next;
		root_pid_list->next = NULL;
	}
}

/**
 * * 		For run command for executor
 * @return	Forks pid
*/
pid_t run_command(t_ast *ast, t_shell *shell, int fd_in, int fd_out, int ast_root_type)
{
	pid_t pid;
	t_list *redirs;
	char **args;

	args = list_to_char_arr(ast->data->ast_command.arguments);
	if(args == NULL)
		return -1;
	if(ast_root_type == AST_IF && isbuiltin(args[0]))
	{
		run_builtin(ast, shell, 0);
		pid = -1;
	}
	else
	{
		printf("DEBUG FORK ELSE \n");
		pid = fork();
		install_command_signals();
		if(pid == 0)
		{
			install_default_signals();
			dup2(fd_in, 0);
			dup2(fd_out, 1);
			close_child_user_fd();
			redirs = ast->data->ast_command.redirs;
			check_redirs_status_in_fork(redirs);
			if (ast->data->ast_command.arguments == NULL)
				exit(EXIT_SUCCESS);
			check_builtin_status(ast, shell, 0);
			execve(command_path(ast, (*shell)), args, shell->env->envp_char);
			print_exec_error_message(args, shell->env->envp_char,
				shell->interactive);
			exit(errno_to_exit_code());
		}
	}
	clear_arr(&args);
	return pid;
}

void exec_utils(t_ast *ast, t_shell *shell, int fd_in, int fd_out, int ast_root_type)
{
	int pipefd[2];
	pid_t	pid;

	if (ast == NULL)
		return ;
	if(ast->type == AST_COMMAND)
	{
		pid = run_command(ast, shell, fd_in, fd_out, ast_root_type);
		create_pid_list(shell->env->pid_list, pid);
	}
	else if (ast->type == AST_PIPE)
	{
		if(pipe(pipefd) == -1)
			return ;
		exec_utils(ast->left, shell, fd_in, pipefd[1], AST_PIPE);
		close(pipefd[1]);
		exec_utils(ast->right, shell, pipefd[0], fd_out, AST_PIPE);
		close(pipefd[0]);
	}
	else if (ast->type == AST_IF)
	{
		exec_utils(ast->left, shell, fd_in, fd_out, AST_IF);
		printf("DEBUG : %d\n", shell->exit_status);
		if(shell->exit_status == 0)
			exec_utils(ast->right, shell, fd_in, fd_out, AST_IF);
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

int	executor(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	t_pid_list	*tmp_pid_list;
	int			exit_status;
	int			sig;

	if(ast == NULL || shell == NULL)
		return -1;
	update_special_parameter(ast, shell);
	exec_utils(ast, shell, fd_in, fd_out, 0);
	exit_status = 0;
	tmp_pid_list = shell->env->pid_list;
	if(tmp_pid_list->next != NULL)
	{
		while (tmp_pid_list != NULL)
		{
			if(tmp_pid_list->pid != -1)
				waitpid(tmp_pid_list->pid, &exit_status, 0);
			tmp_pid_list = tmp_pid_list->next;
		}
	}
	if (WIFSIGNALED(exit_status))
	{
		sig = WTERMSIG(exit_status);
		print_signal_message(sig, exit_status);
		return (128 + sig);
	}
	shell->env->pid_list->pid = 0;
	return (WEXITSTATUS(exit_status));
}

