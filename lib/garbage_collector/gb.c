/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gb.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 15:15:03 by muayna            #+#    #+#             */
/*   Updated: 2025/11/16 11:37:09 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "s_gb.h"

static void	destroy(t_gb **root)
{
	t_gb	*list;
	t_gb	*temp;

	list = *root;
	while (list != NULL)
	{
		temp = list->next;
		if (list->address_pointer != NULL)
			free(list->address_pointer);
		free(list);
		list = temp;
	}
	*root = NULL;
}

static t_gb	*create_empty_node(void)
{
	t_gb	*node;

	node = malloc(sizeof(t_gb));
	if (node == NULL)
		return (NULL);
	node->next = NULL;
	return (node);
}

void	*ft_malloc(size_t size, int destroy_list)
{
	static t_gb	*root;
	static t_gb	*last_node;

	if (destroy_list == 1)
	{
		destroy(&root);
		last_node = NULL;
		return (NULL);
	}
	if (root == NULL)
	{
		root = create_empty_node();
		if (root == NULL)
			return (NULL);
		last_node = root;
	}
	else
	{
		last_node->next = create_empty_node();
		if (last_node->next == NULL)
			return (NULL);
		last_node = last_node->next;
	}
	last_node->address_pointer = malloc(size);
	return (last_node->address_pointer);
}
