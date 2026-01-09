/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2026/01/09 22:29:54 by muayna           ###   ########.fr       */
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
	//wait(NULL);
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

int	executor(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out)
{
	t_list	*lst;
	t_list	*root;
	int		exit_status;
	
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
	return (WEXITSTATUS(exit_status));
}

