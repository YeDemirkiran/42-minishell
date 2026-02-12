/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2026/02/07 15:17:07 by muayna           ###   ########.fr       */
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
 * * 		For run command for executor
 * @return	Forks pid
*/
pid_t run_command(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
	pid_t pid;
	t_list *redirs;
	char **args;

	args = list_to_char_arr(ast->data->ast_command.arguments);
	if(args == NULL)
		return -1;
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
	clear_arr(&args);
	return pid;
}

void exec_utils(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
    int pipefd[2];
    int status;
    pid_t pid;
    pid_t pid_subshell;

    if (ast == NULL)
        return;
    if (ast->type == AST_COMMAND)
    {
        pid = run_command(ast, shell, fd_in, fd_out);
        create_pid_list(shell->env->pid_list, pid);
    }
    else if (ast->type == AST_PIPE)
    {
        if (pipe(pipefd) == -1)
            return;
        shell->root_shell_type = AST_PIPE;
        exec_utils(ast->left, shell, fd_in, pipefd[1]);
        close(pipefd[1]);
        shell->root_shell_type = AST_PIPE;
        exec_utils(ast->right, shell, pipefd[0], fd_out);
        close(pipefd[0]);
    }
    else if (ast->type == AST_IF)
    {
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
    else if (ast->type == AST_OR)
    {
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
    else if (ast->type == AST_SUBSHELL)
    {
        pid_subshell = fork();
        if(pid_subshell == 0)
        {
            exec_utils(ast->left, shell, fd_in, fd_out);
            exit(shell->exit_status);
        }
        else
        {
            waitpid(pid_subshell, &status, 0);
            shell->exit_status = status;
            exec_utils(ast->right, shell, fd_in, fd_out);
        }
    }
}

int executor(t_ast *ast, t_shell *shell, int fd_in, int fd_out)
{
    if (ast == NULL || shell == NULL)
        return (-1);

    update_special_parameter(ast, shell);
    exec_utils(ast, shell, fd_in, fd_out);
    return (wait_for_pipeline(shell));
}

