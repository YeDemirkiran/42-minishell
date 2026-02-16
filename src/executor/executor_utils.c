/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 15:31:52 by muayna            #+#    #+#             */
/*   Updated: 2026/01/29 17:14:12 by muayna           ###   ########.fr       */
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
#include "modules/utils.h"
#include "modules/ast.h"

int isbuiltin(const char *command)
{
	if(!ft_strcmp(command, "cd"))
		return 1;
	else if(!ft_strcmp(command, "export"))
		return 1;
	else if(!ft_strcmp(command, "env"))
		return 1;
	else if(!ft_strcmp(command, "echo"))
		return 1;
	else if(!ft_strcmp(command, "exit"))
		return 1;
	else if(!ft_strcmp(command, "pwd"))
		return 1;
	else if(!ft_strcmp(command, "unset"))
		return 1;
	return 0;
}

void run_builtin(t_ast *ast, t_shell *shell, int print_exit_value)
{
	char	**tmp;
	char	*command;
	size_t	len;

	tmp = shell->env->envp_char;
	command = (char *)(ast->data->ast_command.arguments->content);
	len = ft_strlen(command);
	if(isbuiltin(command))
	{
		update_special_parameter(ast, shell);
		if(!ft_strncmp("cd", command, len))
		shell->exit_status = cd(&shell->env->envp_list, ast);
		else if(!ft_strncmp("pwd", command, len))
			pwd();
		else if(!ft_strncmp("env", command, len))
			env(&shell->env->envp_list);
		else if(!ft_strncmp("export", command, len))
			shell->exit_status = export(&shell->env->envp_list, ast);
		else if(!ft_strncmp("unset", command, len))
			unset(&shell->env->envp_list, ast);
		else if(!ft_strncmp("echo", command, len))
			shell->exit_status = echo(ast);
		else if(!ft_strncmp("exit", command, len))
			exit_builtin(ast, shell, print_exit_value);
	}
	shell->env->envp_char = envp_list_to_char_arr(shell->env->envp_list);
	clear_arr(&tmp);
}

int run_redirs(t_list **redirs)
{
	int fd;
	t_redirection *redir;

	fd = -1;
	redir = (t_redirection *)(*redirs)->content;
	if (redir->redir_type == REDIR_INPUT || redir->redir_type == REDIR_HEREDOC)
		fd = open(redir->path, O_RDONLY);
	else if (redir->redir_type == REDIR_OUTPUT)
		fd = open(redir->path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (redir->redir_type == REDIR_APPEND)
		fd = open(redir->path, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (redir->redir_type == REDIR_HEREDOC)
		unlink(redir->path);
	if (fd < 0)
	{
		error_message("minishell", redir->path, NULL);
		return (0);
	}
	if (fd >= 0)
		dup2(fd, redir->target_fd);
	(*redirs) = (*redirs)->next;
	close(fd);
	return (1);
}

char *command_path(t_ast *ast, t_shell shell)
{
	char *cmd;
	char *path;
	char **splitted_path;
	int i;

	i = 0;
	cmd = (char *)ast->data->ast_command.arguments->content;
	if(ft_strchr(cmd, '/'))
		return cmd;
	else
	{
		path = find_value_on_envp(&shell.env->envp_list, "PATH");
		if(path == NULL)
			path = sort_term_strdup(DEFAULT_PATH);
		else
		{
			splitted_path = ft_split(path, ':');
			while(splitted_path[i])
			{
				path = sort_term_strjoin(splitted_path[i], sort_term_strjoin("/", ast->data->ast_command.arguments->content));
				if(access(path, F_OK) == 0)
					break ;
				i++;
			}	
		}
	}
	clear_arr(&splitted_path);
	return path;
}

char **list_to_char_arr(t_list *list)
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
		if(args[i] == NULL)
		{
			clear_arr(&args);
			return NULL;
		}
		i++;
		tmp = tmp->next;
	}
	return args;
}
