/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_error_check.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 17:55:53 by muayna            #+#    #+#             */
/*   Updated: 2026/01/24 04:12:59 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

int check_cd_args_count(t_list *args)
{
	if(ft_lstsize(args) > 2)
	{
		write(2, "-babashell: cd: too many arguments\n", 36);
		return 1;
	}
	return 0;
}

int check_envp_home_path(t_envp_list *list)
{
	if(find_value_on_envp(&list, "HOME") == NULL)
	{
		write(2, "-babashell: HOME not set\n", 26);
		return 1;
	}
	return 0;
}

int is_there_path(void *content, int chdir_value)
{
	if(access((char *)content, F_OK))
	{
		write(2, "babashell: no such file or directory: ", 39);
		ft_putstr_fd(content, 2);
		write(1, "\n", 1);
		return 1;	
	}
	if(chdir_value < 0)
	{
		write(2, "babashell: permission denied: ", 31);
		ft_putstr_fd(content, 2);
		write(2, "\n", 1);
		return 1;		
	}
	return 0;
}

int is_oldpwd_set(t_envp_list *list, char *input)
{
	if(!ft_strncmp(input, "-", ft_strlen(input)) && find_value_on_envp(&list, "OLDPWD") == NULL)
	{
		write(2, "babashell: cd: OLDPWD not set\n", 31);
		return 1;
	}
	return 0;
}