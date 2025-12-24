/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 22:56:23 by muayna            #+#    #+#             */
/*   Updated: 2025/12/24 14:44:38 by muayna           ###   ########.fr       */
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

static void run_redirs(t_list **redirs)
{
	int fd;
	t_redirection *redir;

	fd = -1;
	redir = (t_redirection *)(*redirs)->content;
	if (redir->redir_type == REDIR_INPUT)
		fd = open(redir->path, O_RDONLY);
	else if (redir->redir_type == REDIR_OUTPUT)
		fd = open(redir->path, O_WRONLY | O_CREAT , 0777);
	if (fd >= 0)
		dup2(fd, redir->target_fd);
	(*redirs) = (*redirs)->next;
	close(fd);
}

static char *command_path(t_ast *ast)
{
	char *cmd;

	cmd = (char *)ast->data->ast_command.arguments->content;
	if(!ft_strncmp("/bin/", cmd, 5))
		return ft_strdup(cmd);
	cmd = ft_strjoin("/bin/", cmd);
	return cmd;
}

static char **list_to_char_arr(t_list *list)
{
	char **args;
	int size_args;
	t_list *tmp;
	int i;

	i = 0;
	tmp = list;
	size_args = ft_lstsize(list) + 1;
	args = malloc(sizeof(char *) * size_args);
	if(args == NULL)
		return NULL;
	args[size_args - 1] = NULL;
	while(tmp != NULL)
	{
		args[i] = ft_strdup((char *)tmp->content);
		i++;
		tmp = tmp->next;
	}
	return args;
}

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

int isbuiltin(const char *command)
{
	char **b_names;
	b_names = ft_split("cd pwd echo", ' ');
	if(!ft_strncmp(command, b_names[1], ft_strlen(command)))
	{
		clear_arr(&b_names);
		return 1;
	}
	if(!ft_strncmp(command, b_names[0], ft_strlen(command)))
	{
		clear_arr(&b_names);
		return 1;
	}
	clear_arr(&b_names);
	return 0;
}

void run_builtin(t_ast *ast, char ***envp_char, t_envp_list **envp_list)
{
	char	**tmp;
	char	*command;
	size_t	len;

	tmp = (*envp_char);
	command = (char *)(ast->data->ast_command.arguments->content);
	len = ft_strlen(command);
	if(isbuiltin(command))
	{
		if(!ft_strncmp("cd", command, len))
			cd(envp_list, ast);
		else if(!ft_strncmp("pwd", command, len))
			pwd(envp_list);
		else if(!ft_strncmp("env", command, len))
			env(envp_list);
	}
	(*envp_char) = envp_list_to_char_arr((*envp_list));
	clear_arr(&tmp);
}

void run_command(t_ast *ast, char **envp, int fd_in, int fd_out)
{
	pid_t pid;
	t_list *redirs;
	char **args;
	char *cmd;

	pid = 1;
	args = list_to_char_arr(ast->data->ast_command.arguments);
	cmd = command_path(ast);
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
			run_builtin(ast, &envp, NULL);
			return ;
		}
		execve(cmd, args, envp);
		perror("-babashell: command not found");
		free(cmd);
		clear_arr(&args);
		exit(1);
	}
	wait(NULL);
	free(cmd);
	clear_arr(&args);
}

void	executor(t_ast *ast, char **envp, int fd_in, int fd_out)
{
	int pipefd[2];

	if(ast->type == AST_COMMAND)
		run_command(ast, envp, fd_in, fd_out);
	else if (ast->type == AST_PIPE)
	{
		pipe(pipefd);
		executor(ast->left, envp, fd_in, pipefd[1]);
		close(pipefd[1]);
		executor(ast->right, envp, pipefd[0], fd_out);
		close(pipefd[0]);
	}
	(void)envp;
}

