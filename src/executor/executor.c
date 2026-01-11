/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2026/01/12 01:06:03 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <libft.h>
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "fcntl.h"
#include "sys/wait.h"
#include "../../lib/garbage_collector/gb.h"

#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "enums/e_redir_type.h"
#include <minishell_lb.h>
#include "modules/executor.h"
#include "modules/builtin.h"

/*This function cut \n and \r in char* of end of the line */
// void delete_new_line(char ***s1)
// {
// 	char **str;
// 	int i;

// 	str = (*s1);
// 	i = 0;
// 	while(str[i] != NULL)
// 	{
// 		str[i] = cut_wrong_chracter(str[i]);
// 		i++;
// 	}
// }

/*
* This func close all parent fds in child (without system fd 0,1,2).
* Mean when use for example cat | ls, cat need input but does not have input, ls working fine and will be finished. 
* System waiting cat because cat input door(pipefd[0]) is open, if you use this func. close all parent fds, mean close(pipefd) force way.
* After that system looking input door and system see input door is closed, and system kill the cat func.
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

pid_t run_command(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out)
{
	pid_t pid;
	t_list *redirs;
	char **args;

	args = list_to_char_arr(ast->data->ast_command.arguments);
	pid = fork();
	if(pid == 0)
	{
		dup2(fd_in, 0);
		dup2(fd_out, 1);
		close_child_user_fd();
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

void exec_utils(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out)
{
	t_pid_list *org_list_pid;
	int pipefd[2];
	pid_t	pid;

	org_list_pid = (*list)->pid_list;
	if (ast == NULL)
		return ;
	if(ast->type == AST_COMMAND)
	{
		pid = run_command(ast, list, fd_in, fd_out);
		if((*list)->pid_list->pid == 0)
		{
			org_list_pid->pid = pid;
			org_list_pid->next = NULL;
		}
		else
		{
			while(org_list_pid->next != NULL)
				org_list_pid = org_list_pid->next;
			org_list_pid->next = sort_malloc(sizeof(t_pid_list), 0);
			org_list_pid->next->pid = pid;
			org_list_pid = org_list_pid->next;
			org_list_pid->next = NULL;
		}

	}
	else if (ast->type == AST_PIPE)
	{
		pipe(pipefd);
		exec_utils(ast->left, list, fd_in, pipefd[1]);
		close(pipefd[1]);
		exec_utils(ast->right, list, pipefd[0], fd_out);
		close(pipefd[0]);
	}
	return ;
}

int	executor(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out)
{
	t_pid_list	*tmp_pid_list;
	int		exit_status;
	
	exec_utils(ast, list, fd_in, fd_out);
	exit_status = 0;
	tmp_pid_list = (*list)->pid_list;
	while (tmp_pid_list != NULL)
	{
		waitpid(tmp_pid_list->pid, &exit_status, 0);
		tmp_pid_list = tmp_pid_list->next;
	}
	(*list)->pid_list->pid = 0;
	return (WEXITSTATUS(exit_status));
}

