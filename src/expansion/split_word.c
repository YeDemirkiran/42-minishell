/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 22:00:11 by yademirk          #+#    #+#             */
/*   Updated: 2025/12/25 22:52:50 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>

#include <libft/linked_list.h>

#include "structs/s_ast.h"

/**
 * @brief Takes a list of arguments, and performs word splitting
 * on all of them.
 * @return 0 on failure, 1 on success, 2 on NULL parameters.
 */
static int	split_word_args(t_list **args)
{
	t_list	*node;
	t_list	*tmp;

	if (args == NULL || *args == NULL)
		return (2);
	node = *args;
	while (node != NULL)
	{
		if (node->content == NULL)
		{
			tmp = node->next;
			ft_lstdel_node(args, node, NULL);
			node = tmp;
		}
		else
			node = node->next;
	}
	return (1);
}

/**
 * @brief Takes a syntax tree and recursively splits words in their arguments.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	peform_word_split(t_ast *ast)
{
	if (ast == NULL)
		return (2);
	if (ast->type == AST_COMMAND)
	{
		if (split_word_args(&(ast->data->ast_command.arguments)) != 1)
			return (0);
	}
	if (ast->left != NULL)
	{
		if (peform_word_split(ast->left) != 1)
			return (0);
	}
	if (ast->right != NULL)
	{
		if (peform_word_split(ast->right) != 1)
			return (0);
	}
	return (1);
}
