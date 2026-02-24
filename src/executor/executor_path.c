/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:27:04 by muayna            #+#    #+#             */
/*   Updated: 2026/02/22 15:05:37 by yademirk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"
#include "modules/executor.h"
#include "gb.h"
#include <minishell_lb.h>
#include <libft.h>

static char	*find_correct_path(t_ast *ast, char *source_path)
{
	char	**splitted_path;
	char	*path;
	int		i;

	i = 0;
	splitted_path = ft_split(source_path, ':');
	while (splitted_path[i])
	{
		path = sort_term_strjoin(splitted_path[i], sort_term_strjoin("/",
					ast->data->ast_command.arguments->content));
		if (access(path, F_OK) == 0)
			break ;
		i++;
	}
	clear_arr(&splitted_path);
	return (path);
}

char	*command_path(t_ast *ast, t_shell shell)
{
	char	*cmd;
	char	*path;

	cmd = (char *)ast->data->ast_command.arguments->content;
	if (ft_strchr(cmd, '/'))
		return (cmd);
	else
	{
		path = find_value_on_envp(&shell.env->envp_list, "PATH");
		if (path == NULL)
			path = sort_term_strdup(DEFAULT_PATH);
		else
			path = find_correct_path(ast, path);
	}
	return (path);
}
