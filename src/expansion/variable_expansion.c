/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 13:22:54 by yademirk          #+#    #+#             */
/*   Updated: 2026/02/03 16:26:08 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

/* ================================ HEADERS ================================ */

// Standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Internal libraries
#include <libft/string.h>
#include <libft/predicate.h>

// Project headers
#include "structs/s_ast.h"
#include "enums/e_redir_type.h"
#include "structs/s_redirection.h"
#include "modules/ast.h"

// Local headers
#include "expansion_data.h"
#include "expansion_data_utils.h"
#include "variable_expansion_utils.h"

/* =============================== FUNCTIONS =============================== */

/**
 * @brief Takes a string, performs expansion on it if it's needed and
 * replaces the old one with the new one.
 */
int	expand_and_replace_string(char **str_ptr, t_shell shell, int ignore_quotes)
{
	char	*new;
	t_list	*data;

	if (str_ptr == NULL || *str_ptr == NULL)
		return (2);
	errno = 0;
	data = get_expansion_data(*str_ptr, shell, ignore_quotes);
	if (data == NULL)
		return (errno == 0);
	new = get_expanded_word(*str_ptr, data);
	ft_lstclear(&data, free_expansion_data);
	if (new == NULL && errno != 0)
		return (0);
	free(*str_ptr);
	*str_ptr = new;
	return (1);
}

/**
 * @brief Takes a list node, checks if the content needs
 * to be expanded and does so.
 * @return 0 on failure, 1 on success, 2 on NULL node.
 */
static int	expand_word_list(t_list *node, t_shell shell)
{
	char	*word;

	if (node == NULL || node->content == NULL)
		return (2);
	while (node != NULL)
	{
		word = (char *)(node->content);
		if (expand_and_replace_string(&word, shell, 1) == 0)
			return (0);
		node->content = word;
		node = node->next;
	}
	return (1);
}

static int	expand_redir_list(t_list *node, t_shell shell)
{
	t_redirection	*redir;
	char			*word;

	if (node == NULL || node->content == NULL)
		return (2);
	while (node != NULL)
	{
		redir = (t_redirection *)(node->content);
		node = node->next;
		if (redir->path == NULL)
			continue ;
		word = redir->path;
		if (expand_and_replace_string(&word, shell, 1) == 0)
			return (0);
		redir->path = word;
	}
	return (1);
}

/**
 * @brief Takes an AST node, checks if it's of type AST_COMMAND
 * and then performs expansion on it.
 * @return 0 on failure, 1 on success, 2 on NULL ast parameter.
 */
static int	expand_vars_in_command(t_ast *ast, t_shell shell)
{
	t_list	*args;
	t_list	*redirs;

	if (ast == NULL || ast->type != AST_COMMAND || ast->data == NULL)
		return (2);
	args = ast->data->ast_command.arguments;
	if (args != NULL)
		if (expand_word_list(args, shell) != 1)
			return (0);
	redirs = ast->data->ast_command.redirs;
	if (redirs != NULL)
		if (expand_redir_list(redirs, shell) != 1)
			return (0);
	return (1);
}

/**
 * @brief Takes a syntax tree and recursively expands arguments in it.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	perform_variable_expansion(t_ast *root, t_shell shell)
{
	if (root == NULL)
		return (2);
	if (root->type == AST_COMMAND)
	{
		if (expand_vars_in_command(root, shell) != 1)
			return (0);
	}
	if (root->left != NULL)
	{
		if (perform_variable_expansion(root->left, shell) != 1)
			return (0);
	}
	if (root->right != NULL)
	{
		if (perform_variable_expansion(root->right, shell) != 1)
			return (0);
	}
	return (1);
}

/* ================================== END ================================== */
