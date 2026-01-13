/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yademirk <yademirk@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 14:57:28 by yademirk          #+#    #+#             */
/*   Updated: 2026/01/13 15:59:05 by yademirk         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <libft/string.h>
#include <libft/predicate.h>

#include "structs/s_ast.h"
#include "structs/s_redirection.h"
#include "modules/ast.h"

#include "expansion_data.h"
#include "expansion_data_utils.h"
#include "tilde_expansion.h"
#include "split_word.h"
#include "remove_quotes.h"

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
 * @brief Takes a list node, checks if the content needs
 * to be expanded and does so.
 * @return 0 on failure, 1 on success, 2 on NULL node.
 */
static int	expand_word_list(t_list *node, t_shell shell)
{
	char	*word;
	t_list	*data;

	if (node == NULL || node->content == NULL)
		return (2);
	while (node != NULL)
	{
		word = (char *)(node->content);
		data = get_expansion_data(word, shell);
		if (data != NULL)
		{
			word = get_expanded_word(word, data);
			ft_lstclear(&data, free_expansion_data);
			free(node->content);
			node->content = (void *)word;
		}
		else if (errno != 0)
			return (0);
		node = node->next;
	}
	return (1);
}

static int	expand_redir_list(t_list *node, t_shell shell)
{
	t_list			*data;
	t_redirection	*redir;
	char			*word;

	if (node == NULL || node->content == NULL)
		return (2);
	redir = (t_redirection *)(node->content);
	if (redir->path == NULL)
		return (2);
	while (node != NULL)
	{
		data = get_expansion_data(redir->path, shell);
		if (data != NULL)
		{
			word = get_expanded_word(redir->path, data);
			ft_lstclear(&data, free_expansion_data);
			free(redir->path);
			redir->path = word;
		}
		else if (errno != 0)
			return (0);
		node = node->next;
	}
	return (1);
}

/**
 * @brief Takes an AST node, checks if it's of type AST_COMMAND
 * and then performs expansion on it.
 * @return 0 on failure, 1 on success, 2 on NULL ast parameter.
 */
static int	expand_command_arguments(t_ast *ast, t_shell shell)
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

//TODO: Remove
// static void	print_command_args(t_ast *ast)
// {
// 	t_list	*lst;

// 	if (ast == NULL || ast->type != AST_COMMAND || ast->data == NULL)
// 		return ;
// 	lst = ast->data->ast_command.arguments;
// 	if (lst == NULL)
// 		return ;
// 	printf("CMD: %s, Args: ", (char *)(lst->content));
// 	while (lst != NULL)
// 	{
// 		printf("%s", (char *)(lst->content));
// 		if (lst->next != NULL)
// 			printf(", ");
// 		lst = lst->next;
// 	}
// 	printf("\n");
// }

/**
 * @brief Takes a syntax tree and recursively expands arguments in it.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
static int	perform_expansion(t_ast *root, t_shell shell)
{
	if (root == NULL)
		return (2);
	if (root->type == AST_COMMAND)
	{
		if (expand_command_arguments(root, shell) != 1)
			return (0);
	}
	if (root->left != NULL)
	{
		if (perform_expansion(root->left, shell) != 1)
			return (0);
	}
	if (root->right != NULL)
	{
		if (perform_expansion(root->right, shell) != 1)
			return (0);
	}
	return (1);
}

/**
 * @brief Takes a string, performs expansion on it if it's needed and
 * replaces the old one with the new one.
 */
int	expand_and_replace_string(char **str_ptr, t_shell shell)
{
	char	*new;
	t_list	*data;

	if (str_ptr == NULL || *str_ptr == NULL)
		return (2);
	errno = 0;
	data = get_expansion_data(*str_ptr, shell);
	if (data == NULL)
	{
		if (errno != 0)
			return (0);
		else
			return (1);
	}
	new = get_expanded_word(*str_ptr, data);
	ft_lstclear(&data, free_expansion_data);
	if (new == NULL && errno != 0)
		return (0);
	free(*str_ptr);
	*str_ptr = new;
	return (1);
}

/**
 * @brief Starts the process of full expansion, word splitting
 * and quote removal.
 * @return 0 on failure, 1 on success, 2 on NULL root parameter.
 */
int	expansion(t_ast *root, t_shell shell)
{
	if (root == NULL)
		return (2);
	// printf("\n(DEBUG: Starting shell step 3: Expansion)\n");
	// printf("(DEBUG: Step 3.1: Variable Substiton)\n");
	if (perform_tilde_expansion(root, &(shell.env->envp_list)) != 1)
		return (0);
	if (perform_expansion(root, shell) != 1)
		return (0);
	//printf("(DEBUG: Step 3.2: Word Splitting)\n");
	if (perform_word_split(root) != 1)
		return (0);
	//printf("(DEBUG: Step 3.3: Quote Removal)\n");
	perform_quote_removal(root);
	// printf("(DEBUG: Step 3.4: Printing New Variables)\n");
	// ast_traverse_preorder(root, print_command_args);
	return (1);
}
