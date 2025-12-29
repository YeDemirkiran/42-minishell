/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 21:06:13 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/29 15:36:23 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

char *cd_shortcut(t_envp_list **list, char *input)
{
	char *old_pwd;

	if(input[0] == '-' && input[1] == '\0')
	{
		old_pwd = find_value_on_envp(list, "OLDPWD");
		printf("%s\n", old_pwd);
		return old_pwd;
	} 
	return input;
}

int cd(t_envp_list **list, t_ast *ast)
{
    char buf[1024];
    char *old_pwd;
	int check;

	if(check_cd_args_count(ast->data->ast_command.arguments) || check_envp_home_path((*list)))
		return 1;
    getcwd(buf, 1024);
	if(ast->data->ast_command.arguments->next == NULL)
		check =	chdir(find_value_on_envp(list, "HOME"));	
	else
   		check =	chdir(cd_shortcut(list, (char *)(ast->data->ast_command.arguments->next->content)));
	if(check < 0)
		if(is_there_path(ast->data->ast_command.arguments->next->content, check))
			return -1;
	update_envp_value(list, "OLDPWD", buf);
    getcwd(buf, 1024);
    update_envp_value(list, "PWD", buf);
	return 0;
}
