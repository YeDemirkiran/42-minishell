/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 17:42:56 by muayna            #+#    #+#             */
/*   Updated: 2026/01/08 15:31:02 by muayna           ###   ########.fr       */
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
	char *arg;
	t_gb **rootgb;
	t_gb *rootgb_back;
	t_gb *tmp;
	t_envp_list *unsetted_node;
	t_envp_list *unsetted_node_back;

	rootgb = ft_malloc(0, 5);
	tmp = (*rootgb);
	rootgb_back = rootgb;
	while(tmp != NULL)
	{
		if(tmp->address_pointer == tmp)
		{
			
		}
		rootgb_back = rootgb;
		tmp = tmp->next;
	}
	printf("%p %p", tmp, (*rootgb));
}
