/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 17:42:56 by muayna            #+#    #+#             */
/*   Updated: 2026/01/09 17:14:15 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include <modules/builtin.h>
#include <libft.h>
#include <dirent.h>
#include "unistd.h"

#include "structs/s_ast.h"
#include <minishell_lb.h>
#include "../lib/garbage_collector/s_gb.h"

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

void unset(t_envp_list **list, t_ast *ast)
{
	t_gb **root_longterm_gb;
	t_gb *tmp;
	t_gb *back_tmp;
	t_envp_list *target_node;
	t_envp_list *target_node_back;
	t_list *args;
	t_list *args_tmp;

	root_longterm_gb = ft_malloc(0, 5);
	tmp = (*root_longterm_gb);
	back_tmp = tmp;
	args = ast->data->ast_command.arguments->next;
	if(root_longterm_gb == NULL || args == NULL)
		return ;
	while(args != NULL)
	{
		target_node = find_node_address(list, args->content);
		tmp = (*root_longterm_gb);
		while(tmp != NULL && target_node != NULL)
		{
			if(tmp->address_pointer == target_node)
			{
				if((*root_longterm_gb)->address_pointer == target_node)
					(*root_longterm_gb) = (*root_longterm_gb)->next;
				else if(tmp->next == NULL);
				else
					back_tmp->next = tmp->next;
				target_node_back = find_node_back_enplist((*list), target_node);
				if(target_node == (*list))
					(*list) = (*list)->next;
				else if(target_node->next == NULL)
				{
					target_node_back->next = NULL;
				}
				else
					target_node_back->next = target_node->next;
				free(target_node->id_name);
				free(target_node->value);
				free(target_node);
				if(tmp->next != NULL)
					free(tmp);
				break ;
			}
			back_tmp = tmp;
			tmp = tmp->next;
		}
		args = args->next;
	}	
}
