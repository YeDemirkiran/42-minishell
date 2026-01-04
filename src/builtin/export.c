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

static int add_variable_on_envplist(t_envp_list **list, t_list *command)
{
	t_list *tmp;
	t_envp_list *envp_tmp;

	envp_tmp = (*list);
	tmp = command;
	while(envp_tmp->next != NULL)
		envp_tmp = envp_tmp->next;
	while(tmp != NULL)
	{
		if(is_variable_name_correct(take_variable_name(tmp)))
		{
			tmp = tmp->next;
			continue;
		}
		envp_tmp->next = ft_malloc(sizeof(t_envp_list), 0);
		envp_tmp = envp_tmp->next;
		envp_tmp->id_name = ft_strdup(take_variable_name(tmp));
		envp_tmp->value = ft_strdup(take_variable_value(tmp));
		envp_tmp->next = NULL;
		tmp = tmp->next;
	}
	return 0;
}

void export(t_envp_list **list, t_ast *ast)
{
	t_list *command;
	t_envp_list *copy_list;

	command = ast->data->ast_command.arguments;
	if(command->next == NULL)
	{
		copy_list = copy_envplist((*list));
		print_list(&copy_list);
	}
	else
	{
		command = command->next;
		add_variable_on_envplist(list, command);
	}
	return ;
}
