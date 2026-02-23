/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:06:13 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/23 08:13:34 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"
#include <libft.h>
#include <minishell_lb.h>
#include <modules/builtin.h>

char	*cd_shortcut(t_envp_list **list, char *input)
{
	char	*old_pwd;

	if (!ft_strncmp(input, "-", ft_strlen(input)))
	{
		old_pwd = find_value_on_envp(list, "OLDPWD");
		if (old_pwd != NULL)
			ft_putendl_fd(old_pwd, STDOUT_FILENO);
		return (old_pwd);
	}
	return (input);
}

static void	update_envp_value_for_cd(t_envp_list **list)
{
	char	*check_fwd;

	check_fwd = getcwd(NULL, 1024);
	if(check_fwd != NULL)
	{
		update_envp_value(list, "OLDPWD", check_fwd);
		free(check_fwd);
	}
	check_fwd = getcwd(NULL, 1024);
	if(check_fwd != NULL)
	{
		update_envp_value(list, "PWD", check_fwd);
		free(check_fwd);
	}
}

int	cd(t_envp_list **list, t_ast *ast)
{
	int		check;

	if (check_cd_args_count(ast->data->ast_command.arguments))
		return (1);
	if (ast->data->ast_command.arguments->next == NULL)
	{
		if (check_envp_home_path((*list)))
			return (1);
		check = chdir(find_value_on_envp(list, "HOME"));
	}
	else if (is_oldpwd_set((*list),
			ast->data->ast_command.arguments->next->content))
		return (1);
	else
		check = chdir(cd_shortcut(list,
					(char *)(ast->data->ast_command.arguments->next->content)));
	if (check < 0
		&& is_there_path(ast->data->ast_command.arguments->next->content,
			check))
		return (1);
	update_envp_value_for_cd(list);
	return (0);
}
