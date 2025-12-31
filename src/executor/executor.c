/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2025/12/31 12:14:37 by muayna           ###   ########.fr       */
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


void run_command(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out)
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
	wait(NULL);
	clear_arr(&args);
}

void	executor(t_ast *ast, t_god_struct_envp **list, int fd_in, int fd_out)
{
	int pipefd[2];

	if(ast->type == AST_COMMAND)
		run_command(ast, list, fd_in, fd_out);
	else if (ast->type == AST_PIPE)
	{
		pipe(pipefd);
		executor(ast->left, list, fd_in, pipefd[1]);
		close(pipefd[1]);
		executor(ast->right, list, pipefd[0], fd_out);
		close(pipefd[0]);
	}
}

