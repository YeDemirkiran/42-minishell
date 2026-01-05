/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/31 12:29:35 by muayna            #+#    #+#             */
/*   Updated: 2025/12/31 12:29:35 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <gb.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>

t_envp_list *copy_envplist(t_envp_list *source)
{
	t_envp_list *tmp;
	t_envp_list *new_list;
	t_envp_list *new_list_root;

	tmp = source;
	if(source == NULL)
		return NULL;
	new_list = sort_malloc(sizeof(t_envp_list), 0);
	new_list_root = new_list;
	while(tmp != NULL)
	{
		new_list->id_name = sort_term_strdup(tmp->id_name);
		new_list->value = sort_term_strdup(tmp->value);
		new_list->isvaluempty = tmp->isvaluempty;
		if(tmp->next != NULL)
		{
			new_list->next = sort_malloc(sizeof(t_envp_list), 0);
			new_list = new_list->next;
		}
		tmp = tmp->next;
	}
	new_list->next = NULL;
	return new_list_root;
}

static t_envp_list *find_node_back_enplist(t_envp_list *source, t_envp_list *target)
{
	t_envp_list *tmp;

	tmp = source;
	if(source == NULL || target == NULL)
		return NULL;
	if(source == target)
		return tmp;
	while(tmp != NULL)
	{
		if(tmp->next == target)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}

static t_envp_list *find_smallest_node(t_envp_list *list)
{
	t_envp_list *small;
	t_envp_list *tmp;

	tmp = list;
	small = tmp;
	while(tmp != NULL)
	{
		if(ft_strcmp(tmp->id_name, small->id_name) < 0)
			small = tmp;
		tmp = tmp->next;
	}
	return small;
}

static void print_list_utils(t_envp_list ***list, t_envp_list **back_node, t_envp_list **small)
{
	if((*small)->next == NULL)
		(*back_node)->next = NULL;
	else if((*small) == (*back_node))
		(**list) = (*small)->next;
	else 
		(*back_node)->next = (*small)->next;
}

void print_list(t_envp_list **list)
{
	t_envp_list *small;
	t_envp_list *back_node_small;
	int list_size;

	list_size = size_of_envp_list((*list));
	while(list_size > 0)
	{
		small = find_smallest_node((*list));
		back_node_small = find_node_back_enplist((*list), small);
		if(small->isvaluempty == 0)
			printf("declare -x %s=\"%s\"\n",small->id_name, small->value);
		else
			printf("declare -x %s\n",small->id_name);
		print_list_utils(&list, &back_node_small, &small);
		list_size--;
	}
}
