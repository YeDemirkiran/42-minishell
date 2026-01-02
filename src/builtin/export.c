/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:19:32 by muayna            #+#    #+#             */
/*   Updated: 2025/12/30 22:31:37 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>
/*
static char* take_variable_value(t_list *arguments)
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
	}
	return variable_value;
}*/

static char* take_variable_name(t_list *arguments)
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
	variable_name = ft_strdup(splitted_command[0]);
	clear_arr(&splitted_command);
	return variable_name;
}

static void print_list(t_envp_list *list)
{
	t_envp_list *tmp;

	tmp = list;
	while(tmp != NULL)
	{
		if(tmp->isvaluempty == 0)
			printf("declare -x %s=\"%s\"\n", tmp->id_name, tmp->value);
		else
			printf("declare -x %s\n", tmp->id_name);
		tmp = tmp->next;
	}
}

static int check_error_utils(char *variable_name)
{
	int i;

	i = 0;
	if(variable_name[0] == '=' || ft_isdigit(variable_name[0]))
	{
		printf("-babashell: export: '%s': not a valid identifier\n", variable_name);
		return 1;
	}
	while(variable_name[i])
	{
		if(variable_name[i] == '_');
		else if(!ft_isalnum(variable_name[i]))
		{
			printf("-babashell: export: '%s': not a valid identifier\n", variable_name);
			return 1;
		}
		i++;
	}
	return 0;
}

static int check_error_variables(t_list *args)
{
	t_list *tmp;
	char *variable_name;
	int return_value;

	return_value = 0;
	tmp = args;
	tmp = tmp->next;
	while(tmp != NULL)
	{
		variable_name = take_variable_name(tmp);
		if(check_error_utils(variable_name))
			return_value = 1;
		free(variable_name);
		tmp = tmp->next;
	}
	return return_value;
}

void export(t_envp_list **list, t_ast *ast)
{
	t_list *command;

	command = ast->data->ast_command.arguments;
	if(command->next == NULL)
		print_list((*list));
	else
	{
		check_error_variables(command);
	}
	return ;
}
