/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muayna <muayna@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 01:02:40 by muayna            #+#    #+#             */
/*   Updated: 2026/02/18 01:02:40 by muayna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include "structs/s_ast.h"
#include "unistd.h"
#include <dirent.h>
#include <gb.h>
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

static t_envp_list	*find_smallest_node(t_envp_list *list)
{
	t_envp_list	*small;
	t_envp_list	*tmp;

	tmp = list;
	small = tmp;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->id_name, small->id_name) < 0)
			small = tmp;
		tmp = tmp->next;
	}
	return (small);
}

static void	print_list_utils(t_envp_list ***list, t_envp_list **back_node,
		t_envp_list **small)
{
	if ((*small)->next == NULL)
		(*back_node)->next = NULL;
	else if ((*small) == (*back_node))
		(**list) = (*small)->next;
	else
		(*back_node)->next = (*small)->next;
}

void	print_list(t_envp_list **list)
{
	t_envp_list	*small;
	t_envp_list	*back_node_small;
	int			list_size;

	list_size = size_of_envp_list((*list));
	while (list_size > 0)
	{
		small = find_smallest_node((*list));
		back_node_small = find_node_back_enplist((*list), small);
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(small->id_name, STDOUT_FILENO);
		if (small->isvaluempty == 0)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(small->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
		print_list_utils(&list, &back_node_small, &small);
		list_size--;
	}
}
