/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 15:31:52 by muayna            #+#    #+#             */
/*   Updated: 2026/02/17 17:54:30 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/garbage_collector/gb.h"
#include "enums/e_redir_type.h"
#include "fcntl.h"
#include "modules/ast.h"
#include "modules/builtin.h"
#include "modules/executor.h"
#include "modules/utils.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "sys/wait.h"
#include "unistd.h"
#include <libft.h>
#include <minishell_lb.h>

int	isbuiltin(const char *command)
{
	if (!ft_strcmp(command, "cd"))
		return (1);
	else if (!ft_strcmp(command, "export"))
		return (1);
	else if (!ft_strcmp(command, "env"))
		return (1);
	else if (!ft_strcmp(command, "echo"))
		return (1);
	else if (!ft_strcmp(command, "exit"))
		return (1);
	else if (!ft_strcmp(command, "pwd"))
		return (1);
	else if (!ft_strcmp(command, "unset"))
		return (1);
	return (0);
}

void	run_builtin(t_ast *ast, t_shell *shell, int print_exit_value)
{
	char	**tmp;
	char	*command;

	tmp = shell->env->envp_char;
	command = (char *)(ast->data->ast_command.arguments->content);
	if (isbuiltin(command))
	{
		update_special_parameter(ast, shell);
		if (!ft_strcmp("cd", command))
			shell->exit_status = cd(&shell->env->envp_list, ast);
		else if (!ft_strcmp("pwd", command))
			pwd();
		else if (!ft_strcmp("env", command))
			env(&shell->env->envp_list);
		else if (!ft_strcmp("export", command))
			shell->exit_status = export(&shell->env->envp_list, ast);
		else if (!ft_strcmp("unset", command))
			unset(&shell->env->envp_list, ast);
		else if (!ft_strcmp("echo", command))
			shell->exit_status = echo(ast);
		else if (!ft_strcmp("exit", command))
			exit_builtin(ast, shell, print_exit_value);
	}
	shell->env->envp_char = envp_list_to_char_arr(shell->env->envp_list);
	clear_arr(&tmp);
}

int	run_redirs(t_list **redirs)
{
	int				fd;
	t_redirection	*redir;

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

char	**list_to_char_arr(t_list *list)
{
	char	**args;
	int		size_args;
	t_list	*tmp;
	int		i;

	i = 0;
	tmp = list;
	size_args = ft_lstsize(list) + 1;
	args = malloc(sizeof(char *) * size_args);
	if (args == NULL)
		return (NULL);
	args[size_args - 1] = NULL;
	while (tmp != NULL)
	{
		args[i] = ft_strdup((char *)tmp->content);
		if (args[i] == NULL)
		{
			clear_arr(&args);
			return (NULL);
		}
		i++;
		tmp = tmp->next;
	}
	return (args);
}

void	check_redirs_status_in_fork(t_list *redirs)
{
	int	redir_result;

	while (redirs != NULL)
	{
		redir_result = run_redirs(&redirs);
		if (redir_result != 1)
			exit(EXIT_FAILURE);
	}
}