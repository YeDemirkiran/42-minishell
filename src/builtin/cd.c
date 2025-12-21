/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:06:13 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/22 01:20:02 by muayna           ###   ########.fr       */
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

	if(ft_arrlen((const char **)ast->data->ast_command.arguments) > 2)
	{
		printf("-babashell: cd: too many arguments\n");
		return ;
	}
    getcwd(buf, 1024);
    update_envp_value(list, "OLDPWD", buf);
    //printf("DEBUG OLDPWD %s\n", find_value_on_envp(list, "OLDPWD"));
    chdir(cd_shortcut(list, ast->data->ast_command.arguments[1]));
    getcwd(buf, 1024);
    update_envp_value(list, "PWD", buf);
    printf("DEBUG PWD %s\n", find_value_on_envp(list, "PWD"));
}
