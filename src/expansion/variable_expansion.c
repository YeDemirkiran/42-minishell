/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 13:22:54 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/21 07:28:34 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <libft/string.h>
#include <libft/predicate.h>

#include "structs/s_ast.h"
#include "enums/e_redir_type.h"
#include "structs/s_redirection.h"
#include "modules/ast.h"

#include "expansion_data.h"
#include "expansion_data_utils.h"

/**
 * @brief Takes a list of expansion data blocks and returns the total length of
 * the whole string that will be generated.
 * @note This function is used to correctly allocate memory beforehand.
 */
static size_t	get_new_word_length(t_list *expansion_data)
{
	size_t		len;
	t_expansion	*data;

	len = 0;
	while (expansion_data != NULL && expansion_data->content != NULL)
	{
		data = (t_expansion *)(expansion_data->content);
		if (data->type == EXPANSION_WORD)
			len += data->data.word.end - data->data.word.start;
		else if (data->type == EXPANSION_VARIABLE)
			len += ft_strlen(data->data.variable_value);
		expansion_data = expansion_data->next;
	}
	return (len);
}

/**
 * @brief Helper function to initialize the get_expanded_word function.
 * @return 0 on failure, 1 on success.
 */
static int	get_expanded_word_init(size_t *len, char **word,
	t_list *expansion_data)
{
	*len = get_new_word_length(expansion_data) + 1;
	if (*len == 1)
		return (0);
	*word = malloc(sizeof(char) * *len);
	if (*word == NULL)
		return (0);
	return (1);
}

/**
 * @brief Takes a list of expansion data blocks,
 * builds the expanded version of a string and returns it.
 */
static char	*get_expanded_word(char *str, t_list *expansion_data)
{
	char		*word;
	size_t		i;
	t_expansion	*data;
	size_t		len;

	if (get_expanded_word_init(&len, &word, expansion_data) != 1)
		return (NULL);
	i = 0;
	while (expansion_data != NULL && expansion_data->content != NULL)
	{
		data = (t_expansion *)(expansion_data->content);
		if (data->type == EXPANSION_WORD)
		{
			ft_strlcpy(word + i, str + data->data.word.start,
				data->data.word.end - data->data.word.start + 1);
			i += data->data.word.end - data->data.word.start;
		}
		else if (data->type == EXPANSION_VARIABLE
			&& data->data.variable_value != NULL)
			i += ft_strlcpy(word + i, data->data.variable_value, len - i);
		expansion_data = expansion_data->next;
	}
	return (word);
}

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
