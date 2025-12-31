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

static t_envp_list *sort_for_enpvlist(t_envp_list *list)
{
	t_envp_list *small;
	t_envp_list *tmp;
	int i;

	tmp = list;
	small = tmp;
	while(tmp != NULL)
	{
		i = 0;
		if((small->id_name[i] - tmp->id_name[i]) > 0)
			small = tmp;
		else if((small->id_name[i] - tmp->id_name[i]) == 0)
		{
			while(small->id_name[i] && tmp->id_name[i])
			{
				if((small->id_name[i] - tmp->id_name[i]) > 0)
				{
					small = tmp;
					break ;
				}
				else if(small->id_name[i] - tmp->id_name[i] < 0)
					break ;
				i++;
			}
		}
		tmp = tmp->next;
	}
	return small;
}

static t_envp_list *find_node_back(t_envp_list *source, t_envp_list *find)
{
	t_envp_list *tmp;

	tmp = source;
	while(tmp != NULL && tmp->next != find)
		tmp = tmp->next;
	return tmp;
}

static t_envp_list *envp_list_copy(t_envp_list *list)
{
	t_envp_list *new_list;
	t_envp_list *tmp;
	t_envp_list *new_list_root;

	tmp = list;
	if(list == NULL)
		return NULL;
	new_list = malloc(sizeof(t_envp_list));
	new_list_root = new_list;
	while(tmp != NULL)
	{
		new_list->id_name = ft_strdup(tmp->id_name);
		new_list->value = ft_strdup(tmp->value);
		new_list->isvaluempty = tmp->isvaluempty;
		if(tmp->next != NULL)
		{
			new_list->next = malloc(sizeof(t_envp_list));
			new_list = new_list->next;
		}
		tmp = tmp->next;
	}
	new_list->next = NULL;
	return new_list_root;
}

static void print_variable_on_export(t_envp_list *list)
{
	int i;
	int size_list;
	t_envp_list *sorted;
	t_envp_list *back_of_sorted;

	size_list = size_of_envp_list(list);
	i = 0;
	while(i < size_list)
	{
		sorted = sort_for_enpvlist(list);
		back_of_sorted = find_node_back(list, sorted);
		if(sorted->isvaluempty == 0)
			printf("declare -x %s=\"%s\"\n", sorted->id_name, sorted->value);
		else
			printf("declare -x %s\n", sorted->id_name);
		if(back_of_sorted != NULL && back_of_sorted->next != NULL)
			back_of_sorted->next = sorted->next;
		else
			list = sorted->next;
		free(sorted->id_name);
		free(sorted->value);
		free(sorted);
		i++;
	}
}

int export(t_envp_list **list, t_ast *ast)
{
	t_envp_list *copy_list;

	if(ast->data->ast_command.arguments->next == NULL)
	{
		copy_list = envp_list_copy((*list));
		print_variable_on_export(copy_list);
	}
	else
		if(add_variable_envp_list(list, ast))
			return 1;
	return 0;
}
