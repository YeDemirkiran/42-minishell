/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filename_expansion_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 19:53:23 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/17 23:32:52 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>

// Project headers
#include "structs/s_ast.h"

// Local headers
#include "glob_bonus.h"

/* =============================== FUNCTIONS =============================== */

static void	filename_replace_arg(t_list **args, t_list **current,
	t_list *word_list)
{
	t_list	*next;

	next = (*current)->next;
	ft_lst_replace(args, *current, word_list, free);
	*current = next;
}

/**
 * @brief Takes a list of arguments, expands filenames in it
 * and then replaces the old word with the new word(list).
 */
static int	filename_expand_args(t_list **args)
{
	char	*str;
	t_glob	glob;
	t_list	*current;

	if (args == NULL || *args == NULL)
		return (1);
	current = *args;
	while (current != NULL)
	{
		if (current->content == NULL)
		{
			current = current->next;
			continue ;
		}
		str = (char *)(current->content);
		glob = ft_glob(str);
		if (glob.result == GLOB_SUCCESS)
			filename_replace_arg(args, &current, glob.word_list);
		else if (glob.result == GLOB_FAILURE)
			return (0);
		else
			current = current->next;
	}
	return (1);
}

/**
 * @brief Recursively performs filename expansion on a syntax tree,
 * if the node's type is AST_COMMAND.
 */
int	perform_filename_expansion(t_ast *ast)
{
	if (ast == NULL)
		return (2);
	if (ast->type == AST_COMMAND)
		if (filename_expand_args(&(ast->data->ast_command.arguments)) != 1)
			return (0);
	if (ast->left != NULL)
		if (perform_filename_expansion(ast->left) != 1)
			return (0);
	if (ast->right != NULL)
		if (perform_filename_expansion(ast->right) != 1)
			return (0);
	return (1);
}

/* ================================== END ================================== */
