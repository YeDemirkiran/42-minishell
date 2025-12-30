/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 15:31:52 by muayna            #+#    #+#             */
/*   Updated: 2025/12/30 22:34:38 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

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

int isbuiltin(const char *command)
{
	char **b_names;
	b_names = ft_split("cd pwd echo env export", ' ');
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
	if(!ft_strncmp(command, b_names[3], ft_strlen(command)))
	{
		clear_arr(&b_names);
		return 1;
	}
	if(!ft_strncmp(command, b_names[4], ft_strlen(command)))
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
			pwd();
		else if(!ft_strncmp("env", command, len))
			env(envp_list);
		else if(!ft_strncmp("export", command, len))
			export(envp_list, ast);
	}
	(*envp_char) = envp_list_to_char_arr((*envp_list));
	clear_arr(&tmp);
}

void run_redirs(t_list **redirs)
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
	if (fd >= 0)
		dup2(fd, redir->target_fd);
	(*redirs) = (*redirs)->next;
	close(fd);
}

char *command_path(t_ast *ast)
{
	char *cmd;

	cmd = (char *)ast->data->ast_command.arguments->content;
	if(!ft_strncmp("/bin/", cmd, 5))
		return ft_strdup(cmd);
	cmd = sort_term_strjoin("/bin/", cmd);
	return cmd;
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
		i++;
		tmp = tmp->next;
	}
	return args;
}
