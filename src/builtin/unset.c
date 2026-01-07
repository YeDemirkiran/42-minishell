/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 17:42:56 by muayna            #+#    #+#             */
/*   Updated: 2026/01/07 19:23:29 by muayna           ###   ########.fr       */
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
	t_gb *rootgb;
	t_gb *rootgb_back;
	t_envp_list *unsetted_node;
	t_envp_list *unsetted_node_back;

	rootgb = ft_malloc(0, 5);
	char *arg;
	arg = ast->data->ast_command.arguments->next->content;
	unsetted_node = find_node_address(list, arg);
	unsetted_node_back = find_node_back_enplist((*list), unsetted_node);
	rootgb_back = rootgb;
	while(rootgb != NULL)
	{
		if(rootgb->address_pointer == unsetted_node)
		{
			printf("finded, gb: %p - envp : %p",rootgb->address_pointer, unsetted_node);
			rootgb_back->next = rootgb->next;
			unsetted_node_back->next = unsetted_node->next;
			free(unsetted_node->id_name); 
			free(unsetted_node->value); 
			free(unsetted_node); 
		}
		rootgb_back = rootgb;
		rootgb = rootgb->next;
	}
}