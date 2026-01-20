/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 17:54:05 by muayna            #+#    #+#             */
/*   Updated: 2026/01/20 17:54:29 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

char* take_variable_value(t_list *arguments)
{
	char *variable_value;
	char *value;

	if(arguments == NULL)
		return NULL;
	value = (ft_strchr(arguments->content, '='));
	if(value == NULL)
		return NULL;
	else
	{
		value++;
		variable_value = ft_strdup((value));
		if(variable_value == NULL)
			return NULL;
	}
	return variable_value;
}

char* take_variable_name(t_list *arguments)
{
	char **splitted_command;
	char *variable_name;

	if(arguments == NULL)
		return NULL;
	if(((char *)arguments->content)[0] == '=')
	{
		return ft_strdup(arguments->content);
	}
	splitted_command = ft_split(arguments->content, '=');
	if(splitted_command == NULL)
		return NULL;
	variable_name = ft_strdup(splitted_command[0]);
	clear_arr(&splitted_command);
	if(variable_name == NULL)
		return NULL;
	return variable_name;
}