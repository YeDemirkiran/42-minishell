/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 22:00:11 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/21 10:26:36 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <libft/linked_list.h>

#include "structs/s_ast.h"

#include "split_word_utils.h"

static int	split_word_advance_loop(t_list **args,
	t_list **node, const char *ifs)
{
	t_list	*tmp;

	if (does_need_split((char *)((*node)->content), ifs) != 1)
	{
		*node = (*node)->next;
		return (1);
	}
	tmp = split_words((char *)((*node)->content), ifs);
	if (errno != 0)
	{
		perror("minishell");
		return (0);
	}
	else if (tmp != NULL)
	{
		ft_lst_replace(args, *node, tmp, free);
		*node = tmp->next;
	}
	else
	{
		tmp = (*node)->next;
		ft_lstdel_node(args, *node, free);
		*node = tmp;
	}
	return (1);
}

/**
 * @brief Takes a list of arguments, and performs word splitting
 * on all of them.
 * @return 0 on failure, 1 on success, 2 on NULL parameters.
 */
static int	split_args(t_list **args, const char *ifs)
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
		{
			if (split_word_advance_loop(args, &node, ifs) != 1)
				return (0);
		}
	}
	return (1);
}

static int	word_split_recursive(t_ast *ast, const char *ifs)
{
	if (ast->type == AST_COMMAND)
	{
		if (split_args(&(ast->data->ast_command.arguments), ifs) == 0)
			return (0);
	}
	if (ast->left != NULL)
	{
		if (word_split_recursive(ast->left, ifs) != 1)
			return (0);
	}
	if (ast->right != NULL)
	{
		if (word_split_recursive(ast->right, ifs) != 1)
			return (0);
	}
	return (1);
}

/**
 * @brief Takes a syntax tree and recursively splits words in their arguments.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	perform_word_split(t_ast *ast, t_shell shell)
{
	char	*ifs;
	int		result;

	if (ast == NULL)
		return (2);
	ifs = get_ifs(shell);
	if (ifs == NULL)
		return (0);
	result = word_split_recursive(ast, ifs);
	free(ifs);
	return (result);
}
