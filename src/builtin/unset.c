/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 17:42:56 by muayna            #+#    #+#             */
/*   Updated: 2026/01/09 17:44:32 by muayna           ###   ########.fr       */
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

static void redirection_for_norm(t_unset_god_list *godlist, t_envp_list ***list)
{
	if((*(*godlist).root_longterm_gb)->address_pointer == (*godlist).target_node)
		(*(*godlist).root_longterm_gb) = (*(*godlist).root_longterm_gb)->next;
	else if((*godlist).tmp->next == NULL);
	else
		(*godlist).back_tmp->next = (*godlist).tmp->next;
	(*godlist).target_node_back = find_node_back_enplist((**list), (*godlist).target_node);
	if((*godlist).target_node == (**list))
		(**list) = (**list)->next;
	else if((*godlist).target_node->next == NULL)
		(*godlist).target_node_back->next = NULL;
	else
		(*godlist).target_node_back->next = (*godlist).target_node->next;
	free((*godlist).target_node->id_name);
	free((*godlist).target_node->value);
	free((*godlist).target_node);
	if((*godlist).tmp->next != NULL)
		free((*godlist).tmp);
}

void unset(t_envp_list **list, t_ast *ast)
{
	t_unset_god_list godlist;

	godlist.root_longterm_gb = ft_malloc(0, 5);
	godlist.tmp = (*godlist.root_longterm_gb);
	godlist.back_tmp = godlist.tmp;
	godlist.args = ast->data->ast_command.arguments->next;
	if(godlist.root_longterm_gb == NULL || godlist.args == NULL)
		return ;
	while(godlist.args != NULL)
	{
		godlist.target_node = find_node_address(list, godlist.args->content);
		godlist.tmp = (*godlist.root_longterm_gb);
		while(godlist.tmp != NULL && godlist.target_node != NULL)
		{
				if(godlist.tmp->address_pointer == godlist.target_node)
				{
					redirection_for_norm(&godlist, &list);
					break ;
				}
				godlist.back_tmp = godlist.tmp;
				godlist.tmp = godlist.tmp->next;
		}
		godlist.args = godlist.args->next;
	}	
}
