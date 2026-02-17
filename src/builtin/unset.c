/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 17:42:56 by muayna            #+#    #+#             */
/*   Updated: 2026/02/18 01:04:56 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/garbage_collector/s_gb.h"
#include "stdio.h"
#include "structs/s_ast.h"
#include "unistd.h"
#include <dirent.h>
#include <libft.h>
#include <minishell_lb.h>
#include <modules/builtin.h>

static t_envp_list	*find_node_back_enplist(t_envp_list *source,
		t_envp_list *target)
{
	t_envp_list	*tmp;

	tmp = source;
	if (source == NULL || target == NULL)
		return (NULL);
	if (source == target)
		return (tmp);
	while (tmp != NULL)
	{
		if (tmp->next == target)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

static t_envp_list	*unset_variable_on_envplist(t_envp_list **list,
		char *variable_name)
{
	t_envp_list	*unsetted_node;
	t_envp_list	*back_unsetted_node;

	if (variable_name == NULL)
		return (NULL);
	unsetted_node = find_node_address(list, variable_name);
	if (unsetted_node == NULL)
		return (NULL);
	back_unsetted_node = find_node_back_enplist((*list), unsetted_node);
	if (back_unsetted_node == NULL)
		return (NULL);
	if (back_unsetted_node == (*list))
		(*list) = (*list)->next;
	else if (unsetted_node->next == NULL)
		back_unsetted_node->next = NULL;
	else
		back_unsetted_node->next = unsetted_node->next;
	return (unsetted_node);
}

static t_gb	*find_node_on_gb(t_gb *gb, void *target)
{
	t_gb	*tmp;

	tmp = gb;
	while (tmp != NULL)
	{
		if (tmp->address_pointer == target)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	unset(t_envp_list **list, t_ast *ast)
{
	t_envp_list	*unsetted_node;
	t_gb		**root_gb;
	t_gb		*target_node;
	t_list		*args;

	root_gb = ft_malloc(0, 5);
	args = ast->data->ast_command.arguments;
	if (args == NULL || args->next == NULL)
		return ;
	args = args->next;
	while (args != NULL)
	{
		unsetted_node = unset_variable_on_envplist(list, (char *)args->content);
		if (unsetted_node == NULL)
		{
			args = args->next;
			continue ;
		}
		target_node = find_node_on_gb((*root_gb), unsetted_node);
		free(target_node->address_pointer);
		target_node->address_pointer = NULL;
		args = args->next;
	}
}
