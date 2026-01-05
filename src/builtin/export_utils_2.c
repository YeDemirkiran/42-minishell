/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 07:28:13 by muayna            #+#    #+#             */
/*   Updated: 2026/01/05 07:28:13 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

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
}

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

static int is_variable_name_correct(char *variable_name)
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

static void add_variable_on_envplist_utils(t_envp_list **list, t_list *command)
{
	(*list) = (*list)->next;
	(*list)->id_name = take_variable_name(command);
	(*list)->value = take_variable_value(command);
	if(ft_strchr(command->content, '='))
		(*list)->isvaluempty = 0;
	else
		(*list)->isvaluempty = 1;
	(*list)->next = NULL;
}

static int issamevariable(t_envp_list **list, t_list *command)
{
	char *value_name;

	value_name = take_variable_name(command);
	if(isvaluein_envplist(list, value_name))
		return 1;
	else
		return 0;
}

int add_variable_on_envplist(t_envp_list **list, t_list *command)
{
	t_list *tmp;
	t_envp_list *envp_tmp;

	envp_tmp = (*list);
	tmp = command;
	while(envp_tmp->next != NULL)
		envp_tmp = envp_tmp->next;
	while(tmp != NULL)
	{
		if(issamevariable(list, tmp) || is_variable_name_correct(take_variable_name(tmp)))
		{
			update_envp_value(list, take_variable_name(tmp), take_variable_value(tmp));
			tmp = tmp->next;
			continue;
		}
		envp_tmp->next = ft_malloc(sizeof(t_envp_list), 0);
		add_variable_on_envplist_utils(&envp_tmp, tmp);
		tmp = tmp->next;
	}
	return 0;
}
