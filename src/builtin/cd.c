/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:06:13 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/24 20:26:33 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <libft.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

char *cd_shortcut(t_envp_list **list, char *input)
{
	if(input == NULL)
		return find_value_on_envp(list, "HOME");
	return input;
}

void cd(t_envp_list **list, t_ast *ast)
{
    char buf[1024];
    char *old_pwd;

	if(ft_lstsize(ast->data->ast_command.arguments) > 2)
	{
		printf("-babashell: cd: too many arguments\n");
		return ;
	}
    getcwd(buf, 1024);
    update_envp_value(list, "OLDPWD", buf);
	if(ast->data->ast_command.arguments->next == NULL)
	{
		if(chdir(find_value_on_envp(list, "HOME")) != 0)
		{
			printf("-babashell: cd: HOME not set\n");
			return ;
		}
	}
	else
   		chdir(cd_shortcut(list, (char *)(ast->data->ast_command.arguments->next->content)));
    getcwd(buf, 1024);
    update_envp_value(list, "PWD", buf);
}
